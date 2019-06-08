#ifndef __PSP_H__
#define __PSP_H__

typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Size;

/*
 * ELF header.
 */
typedef struct 
{
	unsigned char	e_ident[16];	// File identification.  				// 0
	Elf32_Half	e_type;				// File type. */          				// 0x10
	Elf32_Half	e_machine;			// Machine architecture. 				// 0x12
	Elf32_Word	e_version;			// ELF format version. 					// 0x14
	Elf32_Addr	e_entry;			// Entry point. 						// 0x18
	Elf32_Off	e_phoff;			// Program header file offset. 			// 0x1C
	Elf32_Off	e_shoff;			// Section header file offset. 			// 0x20
	Elf32_Word	e_flags;			// Architecture-specific flags. 		// 0x24
	Elf32_Half	e_ehsize;			// Size of ELF header in bytes. 		// 0x28
	Elf32_Half	e_phentsize;		// Size of program header entry.		// 0x2A
	Elf32_Half	e_phnum;			// Number of program header entries. 	// 0x2C
	Elf32_Half	e_shentsize;		// Size of section header entry. 		// 0x2E
	Elf32_Half	e_shnum;			// Number of section header entries. 	// 0x30
	Elf32_Half	e_shstrndx;			// Section name strings section. 		// 0x32
} __attribute__((packed)) Elf32_Ehdr;

/*
 * Section header.
 */
typedef struct 
{
	Elf32_Word	sh_name;		// Section name (index into the	section header string table). // 0
	Elf32_Word	sh_type;		/* Section type. */
	Elf32_Word	sh_flags;		/* Section flags. */
	Elf32_Addr	sh_addr;		/* Address in memory image. */
	Elf32_Off	sh_offset;		/* Offset in file. */
	Elf32_Size	sh_size;		/* Size in bytes. */
	Elf32_Word	sh_link;		/* Index of a related section. */
	Elf32_Word	sh_info;		/* Depends on section type. */
	Elf32_Size	sh_addralign;	/* Alignment in bytes. */
	Elf32_Size	sh_entsize;		/* Size of each entry in section. */
} __attribute__((packed))Elf32_Shdr;

/*
 * Program header.
 */

typedef struct 
{
	Elf32_Word	p_type;			/* Entry type. */
	Elf32_Off	p_offset;		/* File offset of contents. */
	Elf32_Addr	p_vaddr;		/* Virtual address in memory image. */
	Elf32_Addr	p_paddr;		/* Physical address (not used). */
	Elf32_Size	p_filesz;		/* Size of contents in file. */
	Elf32_Size	p_memsz;		/* Size of contents in memory. */
	Elf32_Word	p_flags;		/* Access permission flags. */
	Elf32_Size	p_align;		/* Alignment in memory and file. */
} __attribute__((packed)) Elf32_Phdr;

typedef struct 
{
	uint16_t attribute;
	uint8_t module_ver_lo;	
	uint8_t module_ver_hi;
	char	modname[28];
} __attribute__((packed)) PspModuleInfo;

extern uint32_t psp_tag;
extern uint8_t psp_keys[16];
extern uint8_t psp_code;
extern char pspemu_path[36];
extern char psptrans_path[37];

extern void psp_func1(void *unk, uint8_t *elf_buf);

int sys_psp_set_umdfile(char *file, char *id, int prometheus);
int sys_psp_set_decrypt_options(int decrypt_patch, uint32_t tag, uint8_t *keys, uint8_t code, uint32_t tag2, uint8_t *keys2, uint8_t code2);
int sys_psp_read_header(int fd, char *buf, uint64_t nbytes, uint64_t *nread);
int sys_psp_read_umd(int unk, void *buf, uint64_t sector, uint64_t ofs, uint64_t size);
int sys_psp_prx_patch(uint32_t *unk, uint8_t *elf_buf, void *link_register);
int sys_psp_set_emu_path(char *path);
int sys_psp_post_savedata_initstart(int result, void *param);
int sys_psp_post_savedata_shutdownstart(void);

#ifdef DEBUG
	int sys_psp_sony_bug(uint32_t *mips_registers, void *unk, uint32_t mips_PC);
	//int sys_generic_debug(uint64_t unk, uint32_t *ptr, void *sp);
#endif

#endif /* __PSP_H__ */
