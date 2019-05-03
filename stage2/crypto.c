#include <lv2/lv2.h>
#include <lv2/libc.h>

#include "common.h"
#include "crypto.h"

// For whatever reason, if this function gets inlined automatically, it fucks everything
// Stupid compiler
static __attribute__ ((noinline)) void xtea_encrypt_block(uint32_t *k, uint32_t *in, uint32_t *out) 
{
	uint32_t sum, y, z;
	unsigned char i;

	sum = 0;
	y = in[0];	
	z = in[1];
	
	for (i = 0; i < 32; i++) 
	{
		y += (((z<<4) ^ (z>>5)) + z) ^ (sum + k[sum&3]);
		sum += 0x9e3779b9;
		z += (((y<<4) ^ (y>>5)) + y) ^ (sum + k[sum>>11 &3]);
	}
	
	out[0] = y;
	out[1] = z;
}

static inline void xor64(void *in, void *out)
{
	uint8_t *in8 = (uint8_t *)in;
	uint8_t *out8 = (uint8_t *)out;
	
	for (int i = 0; i < 8; i++)	
		out8[i] ^= in8[i];	
}

void xtea_ctr(uint8_t *key, uint64_t nounce, uint8_t *buf, int size)
{
	uint8_t ct[8];
	
	for (int i = 0; i < size; i += 8)
	{
		xtea_encrypt_block((uint32_t *)key, (uint32_t *)&nounce, (uint32_t *)ct);
		xor64(ct, buf+i);
		nounce = nounce+1;		
	}	
}


