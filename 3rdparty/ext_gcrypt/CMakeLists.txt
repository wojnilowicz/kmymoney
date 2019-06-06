# xmlsec1 needs: sha1 hmac
# gwenhywfar needs: md5 rmd160 sha1 sha256 des blowfish aes
# kwallet needs: pkdf2 sha512
ExternalProject_Add(
  ext_gcrypt
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.8.4.tar.bz2
  URL_MD5 fbfdaebbbc6d7e5fbbf6ffdb3e139573

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-ciphers=blowfish,des,aes
    --enable-pubkey-ciphers=rsa # the smallest cipher because nothing uses gcrypt's pubkey ciphers
    --enable-digests=sha1,sha256,sha512,md5,rmd160
    --enable-kfds=pkdf5
    --disable-large-data-tests
    --enable-hmac-binary-check
    --disable-arm-crypto-support
    --disable-neon-support
    --disable-padlock-support
    --disable-doc

    CXXFLAGS=${CXXFLAGS//-w/} # if "-w" is given then "camellia-aesni-avx2-amd64.S:617:8: error: invalid alignment value" during build
    CFLAGS=${CFLAGS//-w/}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gpgerror
)

## Definitions for symmetric ciphers.
#available_ciphers="arcfour blowfish cast5 des aes twofish serpent rfc2268 seed"
#available_ciphers="$available_ciphers camellia idea salsa20 gost28147 chacha20"
#enabled_ciphers=""

## Definitions for public-key ciphers.
#available_pubkey_ciphers="dsa elgamal rsa ecc"
#enabled_pubkey_ciphers=""

## Definitions for message digests.
#available_digests="crc gostr3411-94 md2 md4 md5 rmd160 sha1 sha256 sha512"
#available_digests="$available_digests sha3 tiger whirlpool stribog blake2"
#enabled_digests=""

## Definitions for kdfs (optional ones)
#available_kdfs="s2k pkdf2 scrypt"
#enabled_kdfs=""
