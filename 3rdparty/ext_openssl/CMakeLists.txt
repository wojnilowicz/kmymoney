ExternalProject_Add(
  ext_openssl
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.openssl.org/source/openssl-1.1.1.tar.gz
  URL_MD5 7079eb017429e0ffb9efb42bf80ccb21

  CONFIGURE_COMMAND <SOURCE_DIR>/config --prefix=${CMAKE_INSTALL_PREFIX}/openssl --openssldir=${DEPS_INSTALL_PREFIX}/openssl/ssl
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install_sw
)
