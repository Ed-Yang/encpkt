# md5/aes-128-ecb packet encoder

## Setup

- Install cmake
- Install openssl (1.1.x)

## Source Code

### MD5
https://sourceforge.net/projects/libmd5-rfc/

### AES 
* https://github.com/Ed-Yang/tiny-AES128-C.git (Modified) 
* https://github.com/kokke/tiny-AES128-C.git (Original)


## Win32

### Build

    * NOTE.  If openssl is not installed on default path, set the
      environment variable like:

      set OPENSSL_ROOT_DIR=\Packages\openssl

    > open Visual Studio 2017 Developer Command Prompt and optionaly
    > set the openssl environment variable as above.

    mkdir encpkt\Win32
    cd Win32
    cmake -G "NMake Makefiles" ..
    nmake

### Run

Under directory "encpkt\Win32":

    .\sample\tinytest
    .\sample\ssltest

## Linux

### Build

    mkdir encpkt/build
    cd build
    cmake ..
    make

### Run

Under directory "encpkt/build":

    ./sample/tinytest
    ./sample/ssltest

## API

```C
int pkt_encode(char *packet, int pkt_len, const char *data, const uint16_t len, const char *psk);
uint16_t pkt_decode(char *data, const uint16_t len, const char *packet, const int pkt_len, const char *psk);
```

