ExternalProject_Add(
  ext_xslt
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxslt-1.1.33.tar.gz
  URL_MD5 b3bd254a03e46d58f8ad1e4559cd2c2f

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --without-python --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xml ext_gcrypt
)
