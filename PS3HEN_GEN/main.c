#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <inttypes.h>

#if !defined(WIN32)
#include <unistd.h>
#define _chsize    ftruncate
#endif

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

int main(int argc, char **argv)
{
	if(argc<5)
	{
		printf("Usage:stackframe.bin stage2 stage0 sprx\n");
		return -1;
	}
	
	FILE *sp=fopen(argv[1],"rb+");
	FILE *stage2=fopen(argv[2],"rb");
	FILE *stage0=fopen(argv[3],"rb");
	FILE *sprx=fopen(argv[4],"rb");
	
	if(!sp || !stage2 || !stage0 || !sprx)
	{
		printf("error opening file(s)!\n");
		return -1;
	}
	
	struct stat buffer;
	int status;
	uint32_t truncate_len=0x110000;

	status = stat(argv[1], &buffer);
	if(status == 0)
	{
		if(stat(argv[2],&buffer)==0)
		{
			if(buffer.st_size>0x1fff8)
			{
				//printf("stage2 too big!EXITING!\n");
				printf("stage2 is too big by 0x%lX bytes! EXITING!\n", (uint64_t)(buffer.st_size - 0x1fff8));
				return -1;
			}
			uint8_t *stage2_buf=(uint8_t *)malloc(0x1fff8);
			uint64_t size_stage2=swap64(buffer.st_size);
			memset(stage2_buf,0,0x1fff8);
			fseek(sp,0x80000,SEEK_SET);
			fwrite(stage2_buf,0x1fff8,1,sp);
			
			fread(stage2_buf,buffer.st_size,1,stage2);
			fclose(stage2);
			fseek(sp,0x7fff8,SEEK_SET);
			fwrite(&size_stage2,8,1,sp);
			fwrite(stage2_buf,buffer.st_size,1,sp);
		//	truncate_len+=buffer.st_size;
			free(stage2_buf);
			if(stat(argv[3],&buffer)==0)
			{
				if(buffer.st_size>0xd000)
				{
					printf("stage0 too big!EXITING!\n");
					return -1;
				}
				uint64_t size_stage0=swap64(buffer.st_size);
				uint8_t *stage0_buf=(uint8_t *)malloc(0xd000);
				memset(stage0_buf,0xff,0xd000);
				fseek(sp,0x102000,SEEK_SET);
				fwrite(stage0_buf,0xd000,1,sp);
				
				fread(stage0_buf,buffer.st_size,1,stage0);
				fclose(stage0);
				fseek(sp,0x102000,SEEK_SET);
				fwrite(stage0_buf,buffer.st_size,1,sp);
				free(stage0_buf);
				fseek(sp,0x1008b8,SEEK_SET);
				fwrite(&size_stage0,8,1,sp);
				if(stat(argv[4],&buffer)==0)
				{
					if(buffer.st_size>0xfff8)
					{
						printf("sprx too big!EXITING!\n");
						return -1;
					}
					uint8_t *sprx_buf=(uint8_t *)malloc(0xfff8);
					memset(sprx_buf,0,0xfff8);
					fseek(sp,0x70000,SEEK_SET);
					fwrite(sprx_buf,0xfff8,1,sp);
					
					fread(sprx_buf,buffer.st_size,1,sprx);
					fclose(sprx);
					fseek(sp,0x70000,SEEK_SET);
					fwrite(sprx_buf,buffer.st_size,1,sp);
					free(sprx_buf);
				}
			}
		}
	}
	_chsize( fileno(sp), truncate_len);
	fclose(sp);
	printf("DONE!\n");
	return 0;
}