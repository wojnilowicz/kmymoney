# aqbanking checks only for xmlsec1-gnutls >= 1.0.0 so we don't need openssl and nss
# we need gcrypt though because xmlsec-gnutls library requires xmlsec-gcrypt
ExternalProject_Add(
  ext_xmlsec1
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.aleksey.com/xmlsec/download/xmlsec1-1.2.27.tar.gz
  URL_MD5 508bee7e4f1b99f2d50aaa7d38ede56e

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY} --without-openssl --without-nss --disable-crypto-dl
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xslt ext_gcrypt ext_gnutls
)
