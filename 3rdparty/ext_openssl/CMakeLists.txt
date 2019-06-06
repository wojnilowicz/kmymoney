# With "no-deprecated" nettle shows undefined reference to `ERR_load_crypto_strings'
# With  "no-engine"  mariadb shows undefined EVP_MD_CTX_free
# qt needs: aes, des, dhe, rsa, dsa, ec, rc2, rc4, sha1, sha224, sha256, sha384, md2, md5, pbkdf2, pbes2, pkcs5, pkcs12 hmac, oid
# mariadb needs: sha1, sha224, sha256, sha384, sha512, rpemd160, md5, aes
# postgresql needs: sha1, sha224, sha256, sha384, sha512, md5
# posgtresql fails with "undefined _BIO_s_socket" when no-sock
if(MINGW)
  set(platform_specific_option <SOURCE_DIR>/Configure mingw64)
else()
  set(platform_specific_option <SOURCE_DIR>/config)
endif ()

ExternalProject_Add(
  ext_openssl
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.openssl.org/source/openssl-1.1.1c.tar.gz
  URL_MD5 15e21da6efe8aa0e0768ffd8cd37a5f6

  CONFIGURE_COMMAND ${platform_specific_option}
    --prefix=${CMAKE_INSTALL_PREFIX}
    --openssldir=${CMAKE_INSTALL_PREFIX}/ssl
    --release
    no-afalgeng
    no-capieng
    no-cms
    no-comp
    no-dgram
    no-ec
    no-ec2m
    no-gost
    no-hw-padlock
    no-psk
    no-tests
    no-static-engine
    no-ocsp
    no-rfc3779
    no-sctp
    no-srp
    no-srtp
    no-ts
    no-dtls
    no-aria
    no-bf
    no-blake2
    no-camellia
    no-cast
    no-chacha
    no-cmac
    no-idea
    no-md4
    no-mdc2
    no-ocb
    no-poly1305
    no-scrypt
    no-seed
    no-siphash
    no-sm2
    no-sm3
    no-sm4

  INSTALL_COMMAND $(MAKE) install_sw
  ${CLEANUP_COMMAND}
)
