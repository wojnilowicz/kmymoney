#  *
#  * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
#  *
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License as
#  * published by the Free Software Foundation; either version 2 of
#  * the License, or (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  *

# With "no-deprecated" nettle shows undefined reference to `ERR_load_crypto_strings'
# With  "no-engine"  mariadb shows undefined EVP_MD_CTX_free
# qt needs: aes, des, dhe, rsa, dsa, ec, rc2, rc4, sha1, sha224, sha256, sha384, md2, md5, pbkdf2, pbes2, pkcs5, pkcs12 hmac, oid
# mariadb needs: sha1, sha224, sha256, sha384, sha512, rpemd160, md5, aes
# postgresql needs: sha1, sha224, sha256, sha384, sha512, md5
# posgtresql fails with "undefined _BIO_s_socket" when no-sock
# qt fails with "q_ssl_psk_use_session_callback" if no-psk
# mariadb fails with PEM_read_PrivateKey if no-stdio
if(MINGW)
  set(platform_specific_option <SOURCE_DIR>/Configure mingw64)
else()
  set(platform_specific_option <SOURCE_DIR>/config)
endif ()

ExternalProject_Add(
  ext_openssl
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.openssl.org/source/openssl-1.1.1g.tar.gz
  URL_MD5 76766e98997660138cdaf13a187bd234

  CONFIGURE_COMMAND ${platform_specific_option}
    --prefix=${CMAKE_INSTALL_PREFIX}
    --openssldir=${CMAKE_INSTALL_PREFIX}/ssl
    --release
    no-afalgeng
    no-aria
#     no-asn1 # unsupported option
    no-bf
#     no-bio # unsupported option
#     no-bn # unsupported option
    no-blake2
    no-camellia
    no-capieng
    no-cast
    no-chacha
    no-cmac
    no-cms
    no-comp
    no-crypto-mdebug
    no-ct
#     no-des
    no-dgram
#     no-dh
#     no-dsa
#     no-dso
    no-dtls
    no-ec
    no-ec2m
#     no-ecdh
#     no-ecdsa
    no-ec_nistp_64_gcc_128
#     no-edg
    no-filenames
    no-gost
    no-hw-padlock
    no-idea
#     no-md2
    no-md4
    no-mdc2
    no-nextprotoneg
    no-ocb
    no-ocsp
    no-poly1305
#     no-pkcs7 # unsupported option
#     no-psk
#     no-rc2
#     no-rc4
    no-rc5
    no-rfc3779
#     no-rmd160
    no-scrypt
    no-sctp
    no-seed
    no-siphash
    no-sm2
    no-sm3
    no-sm4
    no-srp
    no-srtp
    no-static-engine
#     no-stdio
    no-tests
    no-ts
    no-ui
    no-ui-console
    no-whirlpool

  INSTALL_COMMAND $(MAKE) install_sw
  ${CLEANUP_COMMAND}
)
