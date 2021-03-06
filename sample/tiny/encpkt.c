
#include "encpkt.h"
#include "md5/md5.h"
#include "tiny-AES128-C/aes.h"

int pkt_encode(PKT_HDR_T *packet, uint16_t pkt_len, const uint8_t *data, const uint16_t len, const char *psk)
{
	md5_state_t state;
	md5_byte_t digest[16];
    
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    uint8_t *dataptr;
    uint8_t *p;
    int i, total, n;
    uint8_t last_blk[16];

    if (data == NULL || packet == NULL || psk == NULL)
        return 0;

    /* check input data size */
    if (PKT_ALIGN16(len) > PKT_MAX_DSIZE)
    {
        /* data too large */
        return 0;
    }

    /* check output buffer size */
    if ( (PKT_ALIGN16(len) + sizeof(PKT_HDR_T))  > pkt_len)
    {
        /* output buffer too small */
        return 0;
    }

    /* generate digest as input key of aes */
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)psk, strlen(psk));
	md5_finish(&state, digest);

    dataptr = (uint8_t *)(pkt + 1);

    memset((char *)pkt, 0, sizeof(PKT_HDR_T));
    pkt->magic = PKT_MAGIC;

    p = (uint8_t *) &pkt->data_len ;
    p[0] = (len & 0xff00) >> 8;
    p[1] = len & 0xff;

    total = sizeof(PKT_HDR_T);
    for (i = 0; i + PKT_BLK_SIZE <= len ; i += PKT_BLK_SIZE)
    {
        AES128_ECB_encrypt(data + i, digest, dataptr + i);  
        total += PKT_BLK_SIZE;
    }

    n = len % PKT_BLK_SIZE;
    if ((len % PKT_BLK_SIZE) != 0)
    {
        memset(last_blk, 0, sizeof(last_blk));
        memcpy(last_blk, data +i, len - i);
        AES128_ECB_encrypt(last_blk, digest, dataptr + i);
        total += PKT_BLK_SIZE;
    }

    return total ;
}

uint16_t pkt_decode(uint8_t *data, const uint16_t len, const PKT_HDR_T *packet, const uint16_t pkt_len, const char *psk)
{
	md5_state_t state;
	md5_byte_t digest[16];
    
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    uint8_t *dataptr;
    uint16_t data_len;
    uint8_t *p;
    int i, payload_len;
    uint8_t last_blk[PKT_BLK_SIZE];

    if (data == NULL || packet == NULL || psk == NULL)
        return 0;

    /* generate digest as input key of aes */
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)psk, strlen(psk));
	md5_finish(&state, digest);
    
    dataptr = (uint8_t *)(pkt + 1);

    if (pkt->magic != PKT_MAGIC)
        return 0;

    p = (uint8_t *) &pkt->data_len;
    data_len = (uint16_t)((*p << 8) | *(p+1));

    payload_len = pkt_len - sizeof(PKT_HDR_T);

    if (len < data_len) // return buffer too small
        return 0;

    for (i = 0; data_len >= (i + PKT_BLK_SIZE); i += PKT_BLK_SIZE)
    {
        AES128_ECB_decrypt(dataptr + i, digest, data + i);
    }

    if (data_len % PKT_BLK_SIZE)
    {
        AES128_ECB_decrypt(dataptr + i, digest, last_blk);
        memcpy(data + i, last_blk, data_len - i);
    }

    return data_len ;
}

