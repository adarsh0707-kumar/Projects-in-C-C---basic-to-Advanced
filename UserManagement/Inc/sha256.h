#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>
#include <stdint.h>

#define SHA256_BLOCK_SIZE 32 /* SHA256 outputs a 32 byte digest */

typedef struct
{
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

/* Convenience: hashes `input` and writes a 65-char null-terminated
   lowercase hex string into `out_hex` (caller must provide >=65 bytes). */
void sha256_hex(const char *input, char out_hex[65]);

#endif /* SHA256_H */
