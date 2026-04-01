/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <memory.h>
#include <stddef.h>
#include <stdint.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
//typedef unsigned char BYTE;             // 8-bit byte
//typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
        uint8_t data[64];
        uint32_t datalen;
        unsigned long long bitlen;
        uint32_t state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const uint32_t k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const uint8_t data[])
{
        uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

        for (i = 0, j = 0; i < 16; ++i, j += 4)
                m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
        for ( ; i < 64; ++i)
                m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

        a = ctx->state[0];
        b = ctx->state[1];
        c = ctx->state[2];
        d = ctx->state[3];
        e = ctx->state[4];
        f = ctx->state[5];
        g = ctx->state[6];
        h = ctx->state[7];

        for (i = 0; i < 64; ++i) {
                t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
                t2 = EP0(a) + MAJ(a,b,c);
                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
        }

        ctx->state[0] += a;
        ctx->state[1] += b;
        ctx->state[2] += c;
        ctx->state[3] += d;
        ctx->state[4] += e;
        ctx->state[5] += f;
        ctx->state[6] += g;
        ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
        ctx->datalen = 0;
        ctx->bitlen = 0;
        ctx->state[0] = 0x6a09e667;
        ctx->state[1] = 0xbb67ae85;
        ctx->state[2] = 0x3c6ef372;
        ctx->state[3] = 0xa54ff53a;
        ctx->state[4] = 0x510e527f;
        ctx->state[5] = 0x9b05688c;
        ctx->state[6] = 0x1f83d9ab;
        ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len)
{
        uint32_t i;

        for (i = 0; i < len; ++i) {
                ctx->data[ctx->datalen] = data[i];
                ctx->datalen++;
                if (ctx->datalen == 64) {
                        sha256_transform(ctx, ctx->data);
                        ctx->bitlen += 512;
                        ctx->datalen = 0;
                }
        }
}

void sha256_final(SHA256_CTX *ctx, uint8_t hash[])
{
        uint32_t i;

        i = ctx->datalen;

        // Pad whatever data is left in the buffer.
        if (ctx->datalen < 56) {
                ctx->data[i++] = 0x80;
                while (i < 56)
                        ctx->data[i++] = 0x00;
        }
        else {
                ctx->data[i++] = 0x80;
                while (i < 64)
                        ctx->data[i++] = 0x00;
                sha256_transform(ctx, ctx->data);
                memset(ctx->data, 0, 56);
        }

        // Append to the padding the total message's length in bits and transform.
        ctx->bitlen += ctx->datalen * 8;
        ctx->data[63] = ctx->bitlen;
        ctx->data[62] = ctx->bitlen >> 8;
        ctx->data[61] = ctx->bitlen >> 16;
        ctx->data[60] = ctx->bitlen >> 24;
        ctx->data[59] = ctx->bitlen >> 32;
        ctx->data[58] = ctx->bitlen >> 40;
        ctx->data[57] = ctx->bitlen >> 48;
        ctx->data[56] = ctx->bitlen >> 56;
        sha256_transform(ctx, ctx->data);

        // Since this implementation uses little endian byte ordering and SHA uses big endian,
        // reverse all the bytes when copying the final state to the output hash.
        for (i = 0; i < 4; ++i) {
                hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
                hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
        }
}

#include <string.h>
#include <stdio.h>

int base64_encode(char const *in, size_t size, char *out);

void sha256b64buf(char *out, size_t out_size, const unsigned char *in, size_t in_size)
{
    unsigned char hash[SHA256_BLOCK_SIZE];
    SHA256_CTX cntx;
    sha256_init(&cntx);
    sha256_update(&cntx, in, in_size);
    sha256_final(&cntx, hash);
    if (out_size < 64) abort();
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        sprintf(out + 2 * i, "%02x", hash[i]);
    }
    out[2 * SHA256_BLOCK_SIZE] = 0;
    /*
    if (out_size >= 48) {
        int z = base64_encode((const char *) hash, SHA256_BLOCK_SIZE, out);
        out[z] = 0;
    } else {
        char buf[48];
        int z = base64_encode((const char *) hash, SHA256_BLOCK_SIZE, buf);
        buf[z] = 0;
        snprintf(out, out_size, "%s", buf);
    }
    */
}

void sha256b64str(char *out, size_t out_size, const unsigned char *str)
{
  size_t in_size = strlen((const char*) str);
    sha256b64buf(out, out_size, str, in_size);
}

static char const base64_encode_table[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * NAME:    base64_encode
 * PURPOSE: convert a char array into base64-encoded char array
 * ARGS:    in  - pointer to the input char array
 *          out - pointer to the output char array
 * RETURN:  number of chars converted
 * NOTE:    buffer out must contain enough space
 *          to put all chars (base64-encoded char array is larger than
 *          the original char array in ratio 4/3)
 */
int
base64_encode(char const *in, size_t size, char *out)
{
  unsigned int   ebuf;
  int            nw = size / 3;
  int            l = size - nw * 3;
  int            i;
  char const    *p = in;
  char          *s = out;

  for (i = 0; i < nw; i++) {
    ebuf  = *(unsigned const char*) p++ << 16;
    ebuf |= *(unsigned const char*) p++ << 8;
    ebuf |= *(unsigned const char*) p++;
    ebuf += (ebuf & ~0x3FFFF);
    ebuf += (ebuf & ~0x3FFFF);
    ebuf += (ebuf & ~0xFFF);
    ebuf += (ebuf & ~0xFFF);
    ebuf += (ebuf & ~0x3F);
    ebuf += (ebuf & ~0x3F);
    *s++ = base64_encode_table[ebuf >> 24];
    *s++ = base64_encode_table[(ebuf >> 16) & 0xFF];
    *s++ = base64_encode_table[(ebuf >> 8) & 0xFF];
    *s++ = base64_encode_table[ebuf & 0xFF];
  }
  if (l == 2) {
    /* make a 18-bit group */
    ebuf  = *(unsigned const char*) p++ << 10;
    ebuf |= *(unsigned const char*) p++ << 2;
    ebuf += (ebuf & ~0xFFF);
    ebuf += (ebuf & ~0xFFF);
    ebuf += (ebuf & ~0x3F);
    ebuf += (ebuf & ~0x3F);
    *s++ = base64_encode_table[(ebuf >> 16) & 0xFF];
    *s++ = base64_encode_table[(ebuf >> 8) & 0xFF];
    *s++ = base64_encode_table[ebuf & 0xFF];
    *s++ = '=';
  } else if (l == 1) {
    /* make a 12-bit group */
    ebuf = *(unsigned const char*) p++ << 4;
    ebuf += (ebuf & ~0x3F);
    ebuf += (ebuf & ~0x3F);    
    *s++ = base64_encode_table[(ebuf >> 8) & 0xFF];
    *s++ = base64_encode_table[ebuf & 0xFF];
    *s++ = '=';
    *s++ = '=';
  }
  return s - out;
}

/**
 * NAME:    base64_encode_str
 * PURPOSE: convert a string into base64-encoded string
 * ARGS:    in  - pointer to the input string
 *          out - pointer to the output string
 * RETURN:  strlen of the encoded string
 * NOTE:    buffer out must contain enough space
 *          to put all chars and '\0' terminator
 *          (base64-encoded string is larger than
 *          the original string in ratio 4/3)
 */
int
base64_encode_str(char const *in, char *out)
{
  int n = base64_encode(in, strlen(in), out);
  out[n] = 0;
  return n;
}

static char const base64_decode_table [] =
{
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,62,64,64,64,63,
  52,53,54,55,56,57,58,59,60,61,64,64,64,65,64,64,
  64,0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,
  15,16,17,18,19,20,21,22,23,24,25,64,64,64,64,64,
  64,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
  41,42,43,44,45,46,47,48,49,50,51,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
  64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
};

/**
 * NAME:    base64_decode
 * PURPOSE: convert base64-encoded chars to plain chars
 * ARGS:    in    - pointer to the input char array
 *          size  - size of the input char array
 *          out   - pointer to the resulting char array
 *          pflag - if NULL, no action, else
 *                  in case of decode error (invalid padding or alignment)
 *                  set to 1, else set to 0
 * RETURN:  number of chars converted
 * NOTE:    buffer out must contain enough space to
 *          accomodate all decoded chars
 */
int
base64_decode(char const *in, size_t size, char *out, int *pflag)
{
  unsigned char const *p = (unsigned char const*) in;
  char *s = out;
  int i;
  unsigned int b = 0;
  int ac = 0;

  if (pflag) *pflag = 0;
  for (i = 0; i < size; i++, p++) {
    if (base64_decode_table[*p] == 64) continue;
    if (*p == '=') {
      if (ac == 3) {
        /* last 16 bit situation */
        b = (b & 0x3F) | ((b & ~0x3F) >> 2);
        b = (b & 0xFFF) | ((b & ~0xFFF) >> 2);
        b >>= 2;
        *s++ = (b >> 8) & 0xFF;
        *s++ = b & 0xFF;
        return s - out;
      } else if (ac == 2 && p[1] == '=') {
        /* last 8 bit situation */
        b = (b & 0x3F) | ((b & ~0x3F) >> 2);
        b >>= 4;
        *s++ = b;
        return s - out;
      } else {
        /* something is wrong */
        if (pflag) *pflag = 1;
        return s - out;
      }
    }
    b = (b << 8) | base64_decode_table[*p];
    ac++;
    if (ac == 4) {
      b = (b & 0x3F) | ((b & ~0x3F) >> 2);
      b = (b & 0xFFF) | ((b & ~0xFFF) >> 2);
      b = (b & 0x3FFFF) | ((b & ~0x3FFFF) >> 2);
      *s++ = b >> 16;
      *s++ = (b >> 8) & 0xFF;
      *s++ = b & 0xFF;
      ac = 0;
    }
  }

  if (ac != 0) {
    //err("base64_decode: invalid alignment");
    if (pflag) *pflag = 1;
  }
  return s - out;
}

/**
 * NAME:    base64_decode_str
 * PURPOSE: convert base64-encoded string to plain form
 * ARGS:    in    - pointer to the input string
 *          out   - pointer to the resulting string
 *          pflag - if NULL, no action, else
 *                  in case of decode error (invalid padding or alignment)
 *                  set to 1, else set to 0
 * RETURN:  strlen of decoded string
 * NOTE:    buffer out must have enough space to accomodate
 *          all decoded chars and '\0' terminator
 */
int
base64_decode_str(char const *in, char *out, int *pflag)
{
  int n = base64_decode(in, strlen(in), out, pflag);
  out[n] = 0;
  return n;
}
