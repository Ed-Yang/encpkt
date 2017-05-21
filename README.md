# md5/aes-128-ecb packet encoder

## Setup

- Install cmake
- Install openssl (1.1.x)
- (optional) visual studio community

### Prepare the source tree

    git clone https://github.com/Ed-Yang/encpkt.git    
    cd encpkt
    git clone https://github.com/Ed-Yang/tiny-AES128-C.git

## Reference source code

### MD5
https://sourceforge.net/projects/libmd5-rfc/

### AES 
* https://github.com/Ed-Yang/tiny-AES128-C.git (Modified) 
* https://github.com/kokke/tiny-AES128-C.git (Original)

## Build

### Windows

    * NOTE.  If openssl is not installed on default path, set the
      environment variable like:

      set OPENSSL_ROOT_DIR=\Packages\openssl

    open Visual Studio 2017 Developer Command Prompt and optionaly
    set the openssl environment variable as above.
    
    In standard installation, the path of batch file is:
    
    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
    
    Alternatively, you can open left down side search window to locate "VsDevCmd.bat" 

```
    vsDevCmd.bat
    mkdir Win32
    cd Win32
    cmake -G "NMake Makefiles" ..
    nmake
```

    * Output executions files are under "Win32\sample" directory

### Linux

```
    mkdir build
    cd build
    cmake ..
    make
```

    * Output executions files are under "build/sample" directory


## Run

    
## Encryption/decryption test

    .\sample\tinytest
    .\sample\ssltest

## UDP encrytption/decryption test

```
    sslpkt or tinypkt:

    arguments:
        -s: enable sending
        -r: enable receiving
        -p port: udp port
        -c count: number of transmit or receive (increment by 1)
```

### self send/receive test

```
    tinypkt -s -r -v
    sslpkt -s -r -v
```

### tiny receive/ssl send test (it can switch the role ...) 

``` 
    In one command window, run:
    
    tinypkt -r -v

    In another command window, run:
    
    sslpkt -s -v

    In first window, the output message:

    send_flag = 0, recv_flag = 1, count = 10, port = 3333
    VERBOSE is ON
    recv rx_size 32 n 16
    size 16 - OK (n = 16)
    recv rx_size 48 n 17
    size 17 - OK (n = 17)
    recv rx_size 48 n 18
    size 18 - OK (n = 18)
    recv rx_size 48 n 19
    size 19 - OK (n = 19)
    recv rx_size 48 n 20
    size 20 - OK (n = 20)
    recv rx_size 48 n 21
    size 21 - OK (n = 21)
    recv rx_size 48 n 22
    size 22 - OK (n = 22)
    recv rx_size 48 n 23
    size 23 - OK (n = 23)
    recv rx_size 48 n 24
    size 24 - OK (n = 24)
    recv rx_size 48 n 25
    size 25 - OK (n = 25)
```

## API

```C
int pkt_encode(char *packet, int pkt_len, const char *data, const uint16_t len, const char *psk);
uint16_t pkt_decode(char *data, const uint16_t len, const char *packet, const int pkt_len, const char *psk);
```

