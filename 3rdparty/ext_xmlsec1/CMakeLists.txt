# aqbanking checks only for xmlsec1-gnutls >= 1.0.0 so we don't need openssl and nss
# we need gcrypt though because xmlsec-gnutls library requires xmlsec-gcrypt
ExternalProject_Add(
  ext_xmlsec1
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.aleksey.com/xmlsec/download/xmlsec1-1.2.28.tar.gz
  URL_MD5 69b8d95c009a404462e19f335e650241

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${CMAKE_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --disable-mans
    --disable-docs
    --without-openssl
    --without-nss
    --disable-crypto-dl
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xslt ext_gcrypt ext_gnutls
)
