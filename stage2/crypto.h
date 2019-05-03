#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <lv2/lv2.h>

typedef struct
{
    int mode;                   /*!<  encrypt/decrypt   */
    unsigned long sk[32];       /*!<  DES subkeys       */
}
des_context;

void xtea_ctr(uint8_t *key, uint64_t nounce, uint8_t *buf, int size);

void des_init(void);
void des_destroy(void);

void des_setkey_enc(des_context *ctx, const uint8_t key[8]);
void des_crypt_ecb(des_context *ctx, const uint8_t input[8], uint8_t output[8]);

#endif /* __CRYPTO_H__ */


