#ifndef SELF_H__
#define SELF_H__

#include <stdint.h>

#define SCE_MAGIC 		        0x53434500
#define ELF_MAGIC		          0x7F454C46
#define SPRX_EXT_MAGIC		    0x6F8A81B0879A3200ULL
#define SPRX_EXT_MAGIC2		    0x4A52A57F5957E800ULL
#define SPRX_EXT_MAGIC_MASK	  0xFFFFFFFFFFFFFF00ULL

typedef struct 
{
  uint32_t magic;               // 0
  uint32_t version;             // 4
  uint16_t flags;               // 8
  uint16_t type;                // A
  uint32_t metadata_offset;     // C
  uint64_t header_len;          // 10
  uint64_t elf_filesize;        // 18
  uint64_t unknown;             // 20
  uint64_t appinfo_offset;      // 28
  uint64_t elf_offset;          // 30
  uint64_t phdr_offset;         // 38
  uint64_t shdr_offset;         // 40
  uint64_t section_info_offset; // 48
  uint64_t sceversion_offset;   // 50
  uint64_t controlinfo_offset;  // 58
  uint64_t controlinfo_size;    // 60
  uint64_t padding;
} __attribute__((packed)) SELF;

typedef struct 
{
  uint64_t authid;    // 0
  uint32_t vendor_id; // 8
  uint32_t self_type; // c
  uint32_t version;   // 10
  uint8_t padding[12];
} __attribute__((packed)) APP_INFO;

typedef struct 
{
  uint32_t magic;
  uint8_t ident[12];
  uint16_t type;
  uint16_t machine;
  uint32_t version;
  uint64_t entry_point;
  uint64_t phdr_offset;
  uint64_t shdr_offset;
  uint16_t flags;
  uint32_t header_size;
  uint16_t phent_size;
  uint16_t phnum;
  uint16_t shent_size;
  uint16_t shnum;
  uint16_t shstrndx;
} __attribute__((packed)) ELF;

typedef struct 
{
  uint32_t type;
  uint32_t flags;
  uint64_t offset_in_file;
  uint64_t vitual_addr;
  uint64_t phys_addr;
  uint64_t segment_size;
  uint64_t segment_mem_size;
  uint64_t alignment;
} __attribute__((packed)) ELF_PHDR;

typedef struct 
{
  uint32_t name_idx;
  uint32_t type;
  uint64_t flags;
  uint64_t virtual_addr;
  uint64_t offset_in_file;
  uint64_t segment_size;
  uint32_t link;
  uint32_t info;
  uint64_t addr_align;
  uint64_t entry_size;
} __attribute__((packed)) ELF_SHDR;

typedef struct 
{
  uint64_t offset;
  uint64_t size;
  uint32_t compressed; // 2=compressed
  uint32_t unknown1;
  uint32_t unknown2;
  uint32_t encrypted;  // 1=encrypted
} __attribute__((packed)) SECTION_INFO;

typedef struct 
{
  uint32_t unknown1;
  uint32_t unknown2;
  uint32_t unknown3;
  uint32_t unknown4;
} __attribute__((packed)) SCEVERSION_INFO;

typedef struct 
{
  uint32_t type; // 1==control flags; 2==file digest        0
  uint32_t size; // 4
  union 
  {
    // type 1
    struct 
    {
      uint64_t control_flags; // 8
      uint8_t padding[32];    // 10
    } control_flags;

    // type 2
    struct 
    {
      uint64_t unknown;    // 8
      uint8_t digest1[20]; // 10
      uint8_t digest2[20]; // 24
      uint8_t padding[8];  // 38
    } file_digest;
  };
} __attribute__((packed)) CONTROL_INFO;


typedef struct 
{
  //uint8_t ignore[32];
  uint8_t key[16];
  uint8_t key_pad[16];
  uint8_t iv[16];
  uint8_t iv_pad[16];
} __attribute__((packed)) METADATA_INFO;

typedef struct 
{
  uint64_t signature_input_length;
  uint32_t unknown1;
  uint32_t section_count;
  uint32_t key_count;
  uint32_t signature_info_size;
  uint64_t unknown2;
} __attribute__((packed)) METADATA_HEADER;

typedef struct 
{
  uint64_t data_offset;
  uint64_t data_size;
  uint32_t type;       // 1 = shdr, 2 == phdr
  uint32_t program_idx;
  uint32_t unknown;
  uint32_t sha1_idx;
  uint32_t encrypted;  // 3=yes; 1=no
  uint32_t key_idx;
  uint32_t iv_idx;
  uint32_t compressed; // 2=yes; 1=no
} __attribute__((packed)) METADATA_SECTION_HEADER;

typedef struct 
{
  uint8_t sha1[20];
  uint8_t padding[12];
  uint8_t hmac_key[64];
} __attribute__((packed)) SECTION_HASH;

typedef struct 
{
  uint32_t unknown1;
  uint32_t signature_size;
  uint64_t unknown2;
  uint64_t unknown3;
  uint64_t unknown4;
  uint64_t unknown5;
  uint32_t unknown6;
  uint32_t unknown7;
} __attribute__((packed)) SIGNATURE_INFO;

typedef struct 
{
  uint8_t r[21];
  uint8_t s[21];
  uint8_t padding[6];
} __attribute__((packed)) SIGNATURE;


typedef struct 
{
  uint8_t *data;
  uint64_t size;
  uint64_t offset;
} SELF_SECTION;


typedef struct 
{	
	uint64_t magic;
	uint64_t nonce_mod;
	uint8_t keys_mod[16];
} __attribute__((packed)) SPRX_EXT_HEADER;

#endif /* SELF_H__ */
