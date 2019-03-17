ExternalProject_Add(
  ext_postgresql
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.postgresql.org/pub/source/v11.0/postgresql-11.0.tar.bz2
  URL_MD5 338ede84c0443db02ff6e8c4c8cbc5e9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --with-libxml --with-libxslt --with-icu --with-tcl --enable-thread-safety --without-readline --with-openssl --with-libraries=${CMAKE_INSTALL_PREFIX}/lib --with-includes=${CMAKE_INSTALL_PREFIX}/include CPPFLAGS=${AT_CPP_FLAGS} LDFLAGS=${AT_LD_FLAGS}
  BUILD_COMMAND rsync -a --ignore-existing src/backend/utils/ src/include/utils/
  COMMAND       rsync -a --ignore-existing src/backend/catalog/ src/include/catalog/
  COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} -C src/bin install
  COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} -C src/include install
  COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} -C src/interfaces install
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_IN_SOURCE 1
  DEPENDS ext_readline ext_tcl ext_xslt ext_openssl
)
