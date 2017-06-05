#include "encpkt.h"
#include "getopt.h"

#ifdef WIN32
#include <winsock2.h>
#include <WS2tcpip.h> // socklen_t
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Crypt32.lib")

#define TEST_UDP_PORT   3333
#define TEST_START_SIZE 16

void fill_test_data(char *buf, int len)
{
    int i;

    for (i = 0; i < len; i++)
        buf[i] = '0' + i % 10;

    return;
}

int verify_test_data(char *buf, int len)
{
    int i;

    for (i = 0; i < len; i++)
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

void usage(char **argv)
{
    printf("%s -s -r -p <port> -c count\n", argv[0]);
    printf("    -s: enable sending\n");
    printf("    -r: enable receiving\n");
    printf("    -p port: udp port\n");
    printf("    -c count: number of transmit or receive (increment by 1)\n");
}

#ifdef WIN32
void init_winsock()
{
    WSADATA wsaData;
    int iResult;
    u_long iMode = 0;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");
    
}
#else
int closesocket(int fd)
{
    return close(fd);
}
#endif

int main(int argc, char **argv)
{
    int send_flag = 0, recv_flag = 0, verbose=0;
    int port = TEST_UDP_PORT;
    int count = 10;
    char *cvalue = NULL;
    int c;
    struct sockaddr_in addr, raddr;
    int sockfd;
    int i, size, n, rx_size;
    socklen_t fromlen;
    uint8_t test_buf[PKT_MAX_DSIZE];
    PKT_HDR_T *en_pkt;
    char rx_data[PKT_MAX_DSIZE+16], tx_data[PKT_MAX_DSIZE+16];
    uint8_t de_data[PKT_MAX_DSIZE];
    char psk[] = "this is the key";

#ifdef WIN32
    init_winsock();
#endif

    opterr = 0;

    while ((c = getopt(argc, argv, "vsrp:c:h?")) != -1)
    {
        switch (c)
        {
        case 's':
            send_flag = 1;
            break;
        case 'r':
            recv_flag = 1;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'v':
            verbose = 1;
            break;            
        case 'c':
            count = atoi(optarg);
            break;
        case 'h':
        case '?':
        default:
            usage(argv);
            return -1;
        }
    }

    if (argc < 2)
    {
        usage(argv);
        return -1;
    }

    printf("send_flag = %d, recv_flag = %d, count = %d, port = %d\n", 
        send_flag, recv_flag, count, port);

    if (verbose)
        printf("VERBOSE is ON\n");
    else
        printf("VERBOSE is OFF\n");
    
    /* create internal socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }   

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(port);
    
    if (recv_flag)
    {
        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("bind");
            closesocket(sockfd);
            return -1;
        }
    }

    for (i = 0, size = TEST_START_SIZE; i < count; i++, size++)
    {
        if (send_flag)
        {
            fill_test_data((char *)test_buf, size);
            
            en_pkt = (PKT_HDR_T *)tx_data;
            n = pkt_encode(en_pkt, PKT_MAX_DSIZE, test_buf, size, psk);

            if (n <= 0)
            {
                printf("encode error !! (size = %d, n = %d)\n", size, n);
                closesocket(sockfd);
                return -1;
            }

            if ((sendto(sockfd, (char *)en_pkt, n, 0, (struct sockaddr *) &addr, sizeof(addr))) != n)
            {
                perror("sendto");
                closesocket(sockfd);
                return -1;
            }           

            if (verbose)
                printf("sendto size %d n %d\n", size, n);
        }

        if (recv_flag)
        {             
            fromlen = sizeof(raddr);
            if ((rx_size=recvfrom(sockfd, (char *)rx_data, sizeof(rx_data), 0, 
                (struct sockaddr *)&raddr, &fromlen)) <= 0)
            {
                perror("recvfrom");
                closesocket(sockfd);
                return -1;
            }           
            
            n = pkt_decode(de_data, PKT_MAX_DSIZE, (PKT_HDR_T *)rx_data, rx_size, psk);

            if (n <= 0)
            {
                printf("deencode error !! (rx_size = %d, n = %d)\n", rx_size, n);
                closesocket(sockfd);
                return -1;
            }

            if (verbose)
                printf("recv rx_size %d n %d\n", rx_size, n);

            if (verify_test_data((char *)de_data, n) != 0)
            {
                printf("size %d - FAILED (n = %d) !!\n", size, n);
                closesocket(sockfd);
                return -1;                
            }
            else
            {
                if (verbose)
                    printf("size %d - OK (n = %d) \n", size, n);
            }            
        }
    }

    return 0;
}
