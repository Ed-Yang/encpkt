# MD5/AES-128-ecb packet encoder

A demo of utilizing MD5/AES to encrypt packets.

# Setup

## Common

- [CMake 64 bits](https://cmake.org/files/v3.8/cmake-3.8.1-win64-x64.msi)
- [OpenSSL-1.1.0e source](https://www.openssl.org/source/openssl-1.1.0e.tar.gz)

## Linux

- Rebuld OpenSSL
```
    wget https://www.openssl.org/source/openssl-1.1.0e.tar.gz
    tar xvf openssl-1.1-0e.tar.gz
    cd openssl-1.1-0e
    ./config
    make
    make test
    sudo make install
    
    * reboot Linux and vefiy OpenSSL installation

    openssl
    OpenSSL> version
    OpenSSL 1.1.0e  16 Feb 2017
    OpenSSL>     
```

## Windows

- [Visual Studio Community 2017](https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15)
- Rebuild OpenSSL
    - Install [ActivePerl](https://www.activestate.com/activeperl/downloads/thank-you?dl=http://downloads.activestate.com/ActivePerl/releases/5.24.1.2402/ActivePerl-5.24.1.2402-MSWin32-x64-401627.exe)
    - Optionall, Install 
        * [NASM 32-bits](http://www.nasm.us/pub/nasm/releasebuilds/2.13.01/win32/nasm-2.13.01-installer-x86.exe)
        * [NASM 64-bits](http://www.nasm.us/pub/nasm/releasebuilds/2.13.01/win64/nasm-2.13.01-installer-x64.exe)  
    
    In this sample, we use "no-asm", so there is no need to install it.   

    Open a privilege mode "VC Developer Command Window" (32 bits or 64 bits)
```
    * 32-bits
    perl Configure VC-WIN32 no-shared no-asm

    * 64-bits
    perl Configure VC-WIN64A no-shared no-asm

    nmake
    nmake install
```

## macOS

- Install OpenSSL

```
    brew install openssl
```



## Prepare the source tree

```
    git clone https://github.com/Ed-Yang/encpkt.git    
    cd encpkt
    git clone https://github.com/Ed-Yang/tiny-AES128-C.git
```

# Build

## Windows
    
    In standard installation, the path of "VC Developer Command Window" is:
    
    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
    
    Alternatively, you can open left down side search window to locate "VsDevCmd.bat" 

```
    Open a privilege mode "VC Developer Command Window" (vsDevCmd.bat)
    set OPENSSL_ROOT_DIR=C:\Program Files (x86)\OpenSSL
    mkdir Win32
    cd Win32
    cmake -G "NMake Makefiles" ..
    nmake
```

    * Output executions files are under "Win32\sample" directory

## Linux

```
    mkdir build
    cd build
    cmake ..
    make
```

    * Output executions files are under "build/sample" directory

## macOS

```
    mkdir xcode
    cd xcode
    cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DOPENSSL_LIBRARIES=/usr/local/opt/openssl/lib ..
    make
```   

    * Output executions files are under "build/sample" directory

# Run

    
## Encryption/decryption test

### Windows

    .\sample\tinytest
    .\sample\ssltest

### Linux/macOS

    ./sample/tinytest
    ./sample/ssltest

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

# API

```C
int pkt_encode(PKT_HDR_T *packet, uint16_t pkt_len, const uint8_t *data, const uint16_t len, const char *psk);
uint16_t pkt_decode(uint8_t *data, const uint16_t len, const PKT_HDR_T *packet, const uint16_t pkt_len, const char *psk);
```

# Reference

## MD5
    * https://sourceforge.net/projects/libmd5-rfc/

## AES 
    * https://github.com/kokke/tiny-AES128-C.git (Original)
    * https://github.com/Ed-Yang/tiny-AES128-C.git (Modified) 


#OpenSSL
#MD5
#AES


