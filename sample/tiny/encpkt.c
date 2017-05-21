
#include "encpkt.h"
#include "md5/md5.h"
#include "tiny-AES128-C/aes.h"

/*
 * pkt_encode
 *  packet - output cipher text
 *  pkt_len - output buffer size
 *  data - input data
 *  len - input data length
 *  psk - pre-shared key string 
 *  return 
 *    success, total output data length
 */
int pkt_encode(char *packet, int pkt_len, const char *data, const uint16_t len, const char *psk)
{
	md5_state_t state;
	md5_byte_t digest[16];
    
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    char *dataptr;
    uint8_t *p;
    int i, total, n;
    char last_blk[16];

    /* generate digest as input key of aes */
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)psk, strlen(psk));
	md5_finish(&state, digest);

    dataptr = (char *)(pkt + 1);

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

/*
 * pkt_decode
 *  data - decoded packet
 *  out_len - output buffer size
 *  data - input cipher text
 *  len - input data buffer size
 *  psk - pre-shared key string 
 *  return 
 *    success, total output data length
 */
uint16_t pkt_decode(char *data, const uint16_t len, const char *packet, const int pkt_len, const char *psk)
{
	md5_state_t state;
	md5_byte_t digest[16];
    
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    char *dataptr;
    uint16_t data_len;
    uint8_t *p;
    int i, payload_len;
    char last_blk[PKT_BLK_SIZE];

    /* generate digest as input key of aes */
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)psk, strlen(psk));
	md5_finish(&state, digest);
    
    dataptr = (char *)(pkt + 1);

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

