ExternalProject_Add(
  ext_openssl
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.openssl.org/source/openssl-1.1.1b.tar.gz
  URL_MD5 4532712e7bcc9414f5bce995e4e13930

  CONFIGURE_COMMAND <SOURCE_DIR>/config --prefix=${CMAKE_INSTALL_PREFIX}/openssl --openssldir=${CMAKE_INSTALL_PREFIX}/openssl/ssl
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install_sw
  ${CLEANUP_COMMAND}
)
