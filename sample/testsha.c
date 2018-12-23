#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/err.h>

#ifdef WIN32
#include <winsock2.h>
#include <WS2tcpip.h> // socklen_t
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Crypt32.lib")

bool do_sha256(uint8_t *data, size_t len, uint8_t *digest)
{
	EVP_MD_CTX *mdctx = NULL;
    unsigned int hsize = EVP_MD_size(EVP_sha256());

	if((mdctx = EVP_MD_CTX_create()) == NULL)
		return false;

	if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
		goto handle_errors;

	if(1 != EVP_DigestUpdate(mdctx, data, len))
		goto handle_errors;

	if(1 != EVP_DigestFinal_ex(mdctx, digest, &hsize))
		goto handle_errors;

handle_errors:

    if (mdctx)
	    EVP_MD_CTX_destroy(mdctx);

    return true;
}

int main(int argc, char **argv)
{
    char psk[] = "this is the key";
    uint8_t digest[32];
    int rv;

    rv = do_sha256((uint8_t *)psk, strlen(psk), digest);

    fprintf(stderr, "do_sha256: rv = %d\n", rv);

    return 0;
}
