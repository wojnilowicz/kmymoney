# --without-catalog is requirement of QWebEngine
# --with-c14n --with-xptr --with-sax1 --with-output are required by xmlsec1
ExternalProject_Add(
  ext_xml
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxml2-2.9.9.tar.gz
  URL_MD5 c04a5a0a042eaa157e8e8c9eabe76bd6

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/mingw_xml.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
   ${AT_LESS_PROCESSING}
    --with-c14n
    --without-catalog
    --without-debug
    --without-docbook
    --with-icu
    --without-legacy
    --with-output
    --without-python
    --without-regexps
    --with-sax1
    --without-schemas
    --without-schematron
    --without-xinclude
    --with-xptr
    --without-modules

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_icu
)
