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

# aqbanking checks only for xmlsec1-gnutls >= 1.0.0 so we don't need openssl and nss
# we need gcrypt though because xmlsec-gnutls library requires xmlsec-gcrypt
# aqbanking needs only xmlSecTransformHmacSha1Id
# altough is's available in xmlsec1 only, xmlsec1 doesn't allow to build without any crypto library
# gwenhywfar uses gnutls, so let xmlsec1 use gnutls as well

set (gcrypt_lib "-L${UNIX_INSTALL_PREFIX}/lib -lgcrypt")

ExternalProject_Add(
  ext_xmlsec1
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aleksey.com/xmlsec/download/xmlsec1-1.2.30.tar.gz
  URL_MD5 b66ec21e0a0ac331afb4b1bc5c9ef966

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-mscrypto
    --disable-mscng
    --disable-md5
    --disable-ripemd160
    --disable-sha1
    --disable-sha224
    --disable-sha256
    --disable-sha384
    --disable-sha512
    --disable-hmac
    --disable-dsa
    --disable-rsa
    --disable-x509
    --disable-des
    --disable-aes
    --disable-gost
    --disable-gost2012
    --disable-xmldsig
    --disable-xmlenc
    --disable-soap
    --disable-mans
    --disable-docs
    --disable-apps
    --without-openssl
    --without-nss
    --with-gnutls
    --with-gcrypt
    --disable-crypto-dl
    --disable-skm
    --disable-tmpl-tests
    LDFLAGS=${gcrypt_lib}

  ${CLEANUP_COMMAND}

  DEPENDS ext_xslt ext_gnutls ext_gcrypt
)
