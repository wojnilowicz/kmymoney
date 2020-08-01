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

# xmlsec1 needs: sha1 hmac
# gwenhywfar needs: md5 rmd160 sha1 sha256 des blowfish aes
# kwallet needs: pkdf2 sha512
ExternalProject_Add(
  ext_gcrypt
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.8.5.tar.bz2
  URL_MD5 348cc4601ca34307fc6cd6c945467743

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-ciphers=blowfish,des,aes
    --enable-pubkey-ciphers=ecc # the smallest cipher is rsa but gcrypt doesn't build without ecc on Linux
    --enable-digests=sha1,sha256,sha512,md5,rmd160,blake2 #blake2 isn't required but  it doesn't compile without it on Linux
    --enable-kfds=pkdf5
    --disable-large-data-tests
    --disable-hmac-binary-check
    --disable-arm-crypto-support
    --disable-neon-support
    --disable-padlock-support
    --disable-doc

    CXXFLAGS=${CXXFLAGS//-w/} # if "-w" is given then "camellia-aesni-avx2-amd64.S:617:8: error: invalid alignment value" during build
    CFLAGS=${CFLAGS//-w/}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gpgerror
)