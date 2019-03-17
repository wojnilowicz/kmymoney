ExternalProject_Add(
  ext_xslt
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxslt-1.1.32.tar.gz
  URL_MD5 1fc72f98e98bf4443f1651165f3aa146

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --without-python --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST}
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install

  DEPENDS ext_xml
)
