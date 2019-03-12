ExternalProject_Add(
  ext_xslt
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxslt-1.1.32.tar.gz
  URL_MD5 1fc72f98e98bf4443f1651165f3aa146

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --without-python --prefix=${EXT_INSTALL_DIR} ${AT_HOST}
  BUILD_COMMAND ${CMAKE_BUILD_TOOL}
  INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install

  DEPENDS ext_xml
)
