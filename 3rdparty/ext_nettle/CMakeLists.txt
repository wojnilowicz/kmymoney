ExternalProject_Add(
  ext_nettle
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://www.lysator.liu.se/~nisse/archive/nettle-3.4.tar.gz
  URL_MD5 dc0f13028264992f58e67b4e8915f53d

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${CMAKE_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    --enable-public-key
    --disable-documentation
    --with-lib-path=${UNIX_INSTALL_PREFIX}/lib
    --with-include-path=${UNIX_INSTALL_PREFIX}/include

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gmp
)
