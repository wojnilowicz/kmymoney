# --without-catalog is requirement of QWebEngine
# --with-c14n --with-xptr --with-sax1 --with-output are required by xmlsec1
ExternalProject_Add(
  ext_xml
  ${PROJECT_GLOBAL_PROFILE}
  URL http://xmlsoft.org/sources/libxml2-2.9.9.tar.gz
  URL_MD5 c04a5a0a042eaa157e8e8c9eabe76bd6

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/mingw_xml.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
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

  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_icu
)
