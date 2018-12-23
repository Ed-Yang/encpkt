#ifndef _ENCPKT_H
#define _ENCPKT_H

#include <stdio.h>
#include <fcntl.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PKT_MAGIC       0x43434343 // '++++'
#define PKT_MAX_DSIZE   2000
#define PKT_BLK_SIZE    16
#define PKT_ALIGN16(a) (1 + (((a)-1) | (16 - 1)))

#pragma pack(1)

typedef struct PKT_HDR_T
{
    uint8_t     major; 
    uint8_t     minor;
    uint16_t    data_len;
    uint32_t    magic;
    uint32_t reserved1;
    uint32_t reserved2;
} PKT_HDR_T;

#pragma pack()


int pkt_encode(PKT_HDR_T *packet, uint16_t pkt_len, const uint8_t *data, const uint16_t len, const char *psk);
uint16_t pkt_decode(uint8_t *data, const uint16_t len, const PKT_HDR_T *packet, const uint16_t pkt_len, const char *psk);

/* do_sha256: for openssl version test */
bool do_sha256(uint8_t *data, size_t len, uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* _ENCPKT_H */


