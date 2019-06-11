#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SWAP32(x) ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | (((x) >> 24) & 0xff))

typedef struct {
	unsigned char	e_ident[16];	/* ident bytes */
	uint16_t	e_type;			/* file type */
	uint16_t	e_machine;		/* target machine */
	uint32_t	e_version;		/* file version */
	uint64_t	e_entry;		/* start address */
	uint64_t	e_phoff;		/* phdr file offset */
	uint64_t	e_shoff;		/* shdr file offset */
	uint32_t	e_flags;		/* file flags */
	uint16_t	e_ehsize;		/* sizeof ehdr */
	uint16_t	e_phentsize;		/* sizeof phdr */
	uint16_t	e_phnum;		/* number phdrs */
	uint16_t	e_shentsize;		/* sizeof shdr */
	uint16_t	e_shnum;		/* number shdrs */
	uint16_t	e_shstrndx;		/* shdr string index */
} __attribute__((packed)) Elf64_Ehdr;
	

typedef struct {
	uint32_t	p_type;		/* entry type */
	uint32_t	p_flags;	/* entry flags */
	uint64_t	p_offset;	/* file offset */
	uint64_t	p_vaddr;	/* virtual address */
	uint64_t	p_paddr;	/* physical address */
	uint64_t	p_filesz;	/* file size */
	uint64_t	p_memsz;	/* memory size */
	uint64_t	p_align;	/* memory/file alignment */
} __attribute__((packed)) Elf64_Phdr;

typedef struct {
	uint32_t	sh_name;	/* section name */
	uint32_t	sh_type;	/* SHT_... */
	uint64_t	sh_flags;	/* SHF_... */
	uint64_t	sh_addr;	/* virtual address */
	uint64_t	sh_offset;	/* file offset */
	uint64_t	sh_size;	/* section size */
	uint32_t	sh_link;	/* misc info */
	uint32_t	sh_info;	/* misc info */
	uint64_t	sh_addralign;	/* memory alignment */
	uint64_t	sh_entsize;	/* entry size if table */
} __attribute__((packed)) Elf64_Shdr ;

typedef struct {
	uint32_t magic;
	uint32_t version;
	uint16_t flags;
	uint16_t type;
	uint32_t metadata_offset;
	uint64_t header_len;
	uint64_t elf_filesize;
	uint64_t unknown;
	uint64_t appinfo_offset;
	uint64_t elf_offset;
	uint64_t phdr_offset;
	uint64_t shdr_offset;
	uint64_t section_info_offset;
	uint64_t sceversion_offset;
	uint64_t controlinfo_offset;
	uint64_t controlinfo_size;
	uint64_t padding;
} __attribute__((packed)) SELF;

typedef struct {
	uint64_t offset;
	uint64_t size;
	uint32_t compressed; // 2=compressed
	uint32_t unknown1;
	uint32_t unknown2;
	uint32_t encrypted; // 1=encrypted
} __attribute__((packed)) SECTION_INFO;

#define	SHF_WRITE		0x01		/* sh_flags */
#define	SHF_ALLOC		0x02
#define	SHF_EXECINSTR		0x04
#define	SHF_MERGE		0x10
#define	SHF_STRINGS		0x20
#define	SHF_INFO_LINK		0x40
#define	SHF_LINK_ORDER		0x80
#define	SHF_OS_NONCONFORMING	0x100
#define	SHF_GROUP		0x200
#define	SHF_TLS			0x400

static uint64_t swap64(uint64_t data)
{
	uint64_t ret = (data << 56) & 0xff00000000000000ULL;
	ret |= ((data << 40) & 0x00ff000000000000ULL);
	ret |= ((data << 24) & 0x0000ff0000000000ULL);
	ret |= ((data << 8) & 0x000000ff00000000ULL);
	ret |= ((data >> 8) & 0x00000000ff000000ULL);
	ret |= ((data >> 24) & 0x0000000000ff0000ULL);
	ret |= ((data >> 40) & 0x000000000000ff00ULL);
	ret |= ((data >> 56) & 0x00000000000000ffULL);
	return ret;
}

static void command2(char *cmd, char *arg1, char *arg2)
{
	char buf[2048];
	
	snprintf(buf, sizeof(buf), "%s %s %s", cmd, arg1, arg2);
	system(buf);
}

int main(int argc, char *argv[])
{
	FILE *f;
	uint8_t *self_buf;
	uint32_t elf_buf[0x100];
	SELF *self;
	SECTION_INFO *section_info;
	Elf64_Ehdr *elf_header;
	Elf64_Phdr *elf_phdr;
		
	if (argc < 2)
	{
		printf("Usage: %s self/sprx (Only encrypted!)\n", argv[0]);
		return -1;
	}
	
	f = fopen(argv[1], "rb");
	if (!f)
	{
		fprintf(stderr, "Cannot open %s\n", argv[1]);
		return -1;
	}
	
	self_buf = malloc(256*1024);
	fread(self_buf, 1, 256*1024, f);
	fclose(f);
	
	command2("scetool.exe -d", argv[1], "temp_dec");
	f = fopen("temp_dec", "rb");
	if (!f)
	{
		free(self_buf);
		fprintf(stderr, "Cannot open temporal decrypted file. Most probable reasons are that sce tools are not installed or self/sprx is invalid.\n");
		return -1;
	}
	
	self = (SELF *)self_buf;
	section_info = (SECTION_INFO *)(self_buf+swap64(self->section_info_offset));
	elf_header = (Elf64_Ehdr *)(self_buf+swap64(self->elf_offset));
	elf_phdr = (Elf64_Phdr *)(((uint8_t *)elf_header)+swap64(elf_header->e_phoff));
	
	for (int i = 0; section_info->size != 0; i++, section_info++, elf_phdr++)
	{		
		uint64_t size;
		
		printf("Section %d: ", i);
		
		size = swap64(section_info->size);
		if (size < 0x400)
		{
			printf("too small\n");
		}
		else
		{
			fseek(f, swap64(elf_phdr->p_offset), SEEK_SET);
			fread(elf_buf, 1, sizeof(elf_buf), f);
			printf("\np_offset=%llx\nsize:%llx\n", swap64(elf_phdr->p_offset), size);
			uint64_t hash = 0;
			
			fseek(f, swap64(elf_phdr->p_offset), SEEK_SET);
			fread(elf_buf, 1, sizeof(elf_buf), f);
			
			for (int j = 0; j < 0x100; j++)
			{
				hash ^= SWAP32(elf_buf[j]);
			}
			
			hash = (hash << 32) | (size&0xfffff000);
			printf("0x%016llx\n", (long long int)hash);
		}
	}
	
	free(self_buf);
	fclose(f);
	
	unlink("temp_dec");
	
	return 0;
}
