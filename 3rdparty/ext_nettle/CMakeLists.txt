ExternalProject_Add(
  ext_nettle
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://www.lysator.liu.se/~nisse/archive/nettle-3.4.1.tar.gz
  URL_MD5 9bdebb0e2f638d3b9d91f7fc264b70c1

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --enable-public-key
    --disable-gcov
    --disable-documentation
    --disable-mini-gmp
    --with-lib-path=${UNIX_INSTALL_PREFIX}/lib
    --with-include-path=${UNIX_INSTALL_PREFIX}/include

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gmp
)
