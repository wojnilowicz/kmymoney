# CPPFLAGS and LDFLAGS are required to find unistring and gmp (GMP_CFLAGS and GMP_LIBS do not work in the end)
# enable-cxx undefined psk dh anon
# gwenhywfar requires gnutls_certificate_set_dh_params, so enable-cxx and enable-dhe is required
ExternalProject_Add(
  ext_gnutls
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/gnutls/v3.6/gnutls-3.6.7.1.tar.xz
  URL_MD5 92a8049e618afa60e2c852da1884c457

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-bash-tests
    --disable-doc
    --disable-manpages
    --disable-tools
    --enable-cxx
#    --disable-tls13-interop
#    --disable-ssl3-support
#    --disable-ssl2-support
#    --disable-dtls-srtp-support
#    --disable-alpn-support
#    --disable-heartbeat-support
#    --disable-srp-authentication
#    --enable-psk-authentication
#    --enable-anon-authentication
#    --enable-dhe
#    --enable-ecdhe
#    --disable-gost
#    --disable-cryptodev
#    --disable-ocsp
    --enable-openssl-compatibility
    --disable-tests
    --disable-fuzzer-target
    --disable-gtk-doc
    --disable-gtk-doc-html
    --disable-gtk-doc-pdf
    --disable-nls
    --disable-seccomp-tests
    --disable-valgrind-tests
    --disable-full-test-suite
#    --disable-fips140-mode
#    --disable-non-suiteb-curves
    --disable-libdane
    --disable-guile
    --without-nettle-mini
    --without-included-libtasn1
    --without-included-unistring
#    --without-idn
    --without-p11-kit
#    --without-tpm
  --disable-rpath
  --enable-local-libopts
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2 ext_gpgerror ext_nettle ext_tasn1 ext_unistring
)
