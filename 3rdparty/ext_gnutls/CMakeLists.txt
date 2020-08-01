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

# CPPFLAGS and LDFLAGS are required to find unistring and gmp (GMP_CFLAGS and GMP_LIBS do not work in the end)
# enable-cxx undefined psk dh anon
# gwenhywfar requires gnutls_certificate_set_dh_params, so enable-cxx and enable-dhe is required

if(MINGW)
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0006-fix-ncrypt-bcrypt-linking.patch)
else()
  set(platform_specific_patch "")
endif()

ExternalProject_Add(
  ext_gnutls
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/gnutls/v3.6/gnutls-3.6.14.tar.xz
  URL_MD5 bf70632d420e421baff482247f01dbfe

  ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-bash-tests
    --disable-doc
    --disable-manpages
    --disable-tools
    --disable-cxx
    --disable-tls13-interop
    --disable-ssl3-support
    --disable-ssl2-support
    --disable-dtls-srtp-support
    --disable-alpn-support
    --disable-heartbeat-support
    --disable-srp-authentication
    --disable-psk-authentication
    --disable-anon-authentication
    --enable-dhe
    --disable-ecdhe
    --disable-gost
    --disable-cryptodev
    --disable-ocsp
    --disable-openssl-compatibility
    --disable-tests
    --disable-fuzzer-target
    --disable-gtk-doc
    --disable-gtk-doc-html
    --disable-gtk-doc-pdf
    --disable-nls
    --disable-rpath
    --disable-seccomp-tests
    --disable-valgrind-tests
    --disable-full-test-suite
    --disable-fips140-mode
    --disable-non-suiteb-curves
    --disable-libdane
    --disable-guile
    --without-nettle-mini
    --with-included-libtasn1
    --with-included-unistring # It's 1.4 MB for libgnutls.30.dylib with this option compared to 1,5 MB for libunistring2.dylib and 1,2 MB for libgnutls.30.dylib without it
    --without-idn
    --without-p11-kit
    --without-tpm
    --enable-local-libopts
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2 ext_gpgerror ext_nettle
)