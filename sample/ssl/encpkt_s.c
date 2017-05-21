
#include "encpkt.h"

#pragma warning( push )

/* win32, openssl: warning C4090: 'function': different 'const' qualifiers */
#pragma warning( disable : 4090)

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/err.h>

#pragma warning( pop )

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
    uint8_t digest[16];
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    char *dataptr;
    char *p;
    int total;
    EVP_CIPHER_CTX *ctx;
    int rv, en_len, pad_len;
    uint8_t pad[PKT_BLK_SIZE];

    /* generate digest as input key of aes */
    MD5(psk, strlen(psk), digest); 

    dataptr = (char *)pkt;

    memset((char *)pkt, 0, sizeof(PKT_HDR_T));
    pkt->magic = PKT_MAGIC;

    p = (char *) &pkt->data_len ;
    p[0] = (len & 0xff00) >> 8;
    p[1] = len & 0xff;

    total = sizeof(PKT_HDR_T);

    //ci = EVP_aes_128_cbc();

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) 
        return 0;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, digest, NULL))
    {
        return 0;
    }

    // EVP_CIPHER_CTX_set_padding() call must be done AFTER the EVP_DecryptInit_ex()
    rv = EVP_CIPHER_CTX_set_padding(ctx, 0); 
    rv = EVP_CIPHER_CTX_set_key_length(ctx, 16); // aes 128

    /* Provide the message to be encrypted, and obtain the encrypted output.
    * EVP_EncryptUpdate can be called multiple times if necessary
    */
    if (1 != EVP_EncryptUpdate(ctx, dataptr + total, &en_len, data, len))
    {
        printf("EVP_EncryptUpdate: len = %d en_len = %d\n", len, en_len);
        ERR_print_errors_fp(stderr);     
        return 0;
    }

    total += en_len;

    if (len % PKT_BLK_SIZE)
    {
        memset(pad, 0, sizeof(pad));
        pad_len = 16 - (len & 0x0f);

        if (1 != EVP_EncryptUpdate(ctx, dataptr + total, &en_len, pad, pad_len))
        {
            printf("EVP_EncryptUpdate: len = %d en_len = %d pad_len = %d\n", len, en_len, pad_len);
            ERR_print_errors_fp(stderr);     
            return 0;
        }

        total += en_len;   
    }

    /* Finalise the encryption. Further ciphertext bytes may be written at
    * this stage.
    */
    if (1 != EVP_EncryptFinal_ex(ctx, dataptr + total, &en_len)) 
    {
        printf("EVP_EncryptFinal_ex: len = %d en_len = %d\n", len, en_len);
        ERR_print_errors_fp(stderr);     
        return 0;
    }

    total += en_len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

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
    uint8_t digest[16];
    PKT_HDR_T *pkt = (PKT_HDR_T *)packet;
    char *dataptr;
    uint16_t data_len;
    uint8_t *p;
    int payload_len;
    EVP_CIPHER_CTX *ctx;    
    int rv, total, de_len;

    /* generate digest as input key of aes */
    MD5(psk, strlen(psk), digest); 
   
    dataptr = (char *)(pkt + 1);

    if (pkt->magic != PKT_MAGIC)
        return 0;

    p = (uint8_t *) &pkt->data_len;
    data_len = (uint16_t)((*p << 8) | *(p+1));

    payload_len = pkt_len - sizeof(PKT_HDR_T);

    if (len < data_len) // return buffer too small
        return 0;

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
        ERR_print_errors_fp(stderr);     
		return 0;
	}

	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	* and IV size appropriate for your cipher
	* In this example we are using 256 bit AES (i.e. a 256 bit key). The
	* IV size for *most* modes is the same as the block size. For AES this
	* is 128 bits */
	if ((rv=EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, digest, NULL /* iv */)) != 1)
	{
		ERR_print_errors_fp(stderr);        
		return 0;
	}

	/*
	 * edward, even for decrypt, it has to set padding as 0, or EVP_DecryptFinal_ex may
	 * fail at "padding oracle (evp_enc.c:527) */
	EVP_CIPHER_CTX_set_padding(ctx, 0);
	//rv = EVP_CIPHER_CTX_set_key_length(ctx, 16);

	/* Provide the message to be decrypted, and obtain the plaintext output.
	* EVP_DecryptUpdate can be called multiple times if necessary
	*/    
    total = 0;
	if ((rv = EVP_DecryptUpdate(ctx, data + total, &de_len, dataptr, payload_len)) != 1)
	{
        printf("EVP_DecryptUpdate: pkt_len = %d de_len = %d payload_len = %d\n", pkt_len, de_len, payload_len);
		ERR_print_errors_fp(stderr);
		return 0;
	}

	total += de_len;

	/* Finalise the decryption. Further plaintext bytes may be written at
	* this stage.
	*/
	if ((rv = EVP_DecryptFinal_ex(ctx, data + total, &de_len)) != 1)
	{
        printf("EVP_DecryptFinal_ex: pkt_len = %d de_len = %d\n", pkt_len, de_len);
		ERR_print_errors_fp(stderr);
		return 0;
	}
	total += de_len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

    return data_len ;
}

