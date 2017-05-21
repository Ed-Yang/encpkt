#include <stdio.h>
#include "encpkt.h"

void fill_test_data(char *buf, int len)
{
    int i ;

    for (i=0; i < len; i++)
        buf[i] = '0' + i % 10;

    return ;
}

int verify_test_data(char *buf, int len)
{
    int i ;

    for (i=0; i < len; i++)
        if (buf[i] != ('0' + i % 10))
            return -1;

    return 0;
}

void hex_dump(uint8_t *buf, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        if ((i % 4) == 3)
            printf("%02x ", (unsigned char)buf[i]);
        else
            printf("%02x", (unsigned char)buf[i]);

        if (((i + 1) % 32) == 0)
        {
            printf("\n");
        }        
    }

    printf("\n");
}

/* Main program */
int
main(int argc, char *argv[])
{
    const char psk[]="this is the secret"; // pre-shared key
    PKT_HDR_T *en_pkt;
    char *de_data;
    char data[PKT_MAX_DSIZE];
    int i;
    int en_size, de_size;


    en_pkt = (PKT_HDR_T *) malloc(sizeof(PKT_HDR_T) + PKT_MAX_DSIZE);
    de_data = malloc(PKT_MAX_DSIZE);

    for (i = 64; i < PKT_MAX_DSIZE; i += 400)
    {
        memset(data, 0, sizeof(data));
        fill_test_data(data, i);
        en_size = pkt_encode((char *)en_pkt, PKT_MAX_DSIZE, data, i, psk);

        if (en_size == 0)
        {
            printf("FAILED at %d -- encode \n", i);
            continue;
        }

        hex_dump((uint8_t *)en_pkt, en_size);

        de_size = pkt_decode(de_data, PKT_MAX_DSIZE, (char *)en_pkt, en_size, psk);
        if (verify_test_data(de_data, i) < 0)
        {
            printf("FAILED at %d\n", i);
        }
        else
        {
            printf("OK at %d\n", i);
        }
    }

    free(en_pkt);
    free(de_data);

    return 0;
}

