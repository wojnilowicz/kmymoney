ExternalProject_Add(
  ext_sqlcipher
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://github.com/sqlcipher/sqlcipher/archive/v3.4.2.zip
  URL_MD5 f12c5975d784856ea89f83cfc9af1a6c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY} CPPFLAGS=${AT_CPP_FLAGS} LDFLAGS=${AT_LD_FLAGS} --enable-tempstore=yes CFLAGS=-DSQLITE_HAS_CODEC
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_tcl
)
