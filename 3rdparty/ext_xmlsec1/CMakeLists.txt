# aqbanking checks only for xmlsec1-gnutls >= 1.0.0 so we don't need openssl and nss
# we need gcrypt though because xmlsec-gnutls library requires xmlsec-gcrypt
set (gcrypt_lib "-L${UNIX_INSTALL_PREFIX}/lib -lgcrypt")
ExternalProject_Add(
  ext_xmlsec1
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aleksey.com/xmlsec/download/xmlsec1-1.2.28.tar.gz
  URL_MD5 69b8d95c009a404462e19f335e650241

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-mans
    --disable-docs
    --without-openssl
    --without-nss
    --disable-crypto-dl
    LDFLAGS=${gcrypt_lib}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gcrypt ext_gnutls ext_xml
)
