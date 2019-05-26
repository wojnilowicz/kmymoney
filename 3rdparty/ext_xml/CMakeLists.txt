if(MINGW)
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/mingw_xml.diff)
else()
  set(platform_specific_patch "")
endif()

ExternalProject_Add(
  ext_xml
  ${PROJECT_GLOBAL_PROFILE}
  URL http://xmlsoft.org/sources/libxml2-2.9.9.tar.gz
  URL_MD5 c04a5a0a042eaa157e8e8c9eabe76bd6

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff
        ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --with-c14n # required by xmlsec1
    --without-catalog # required to be disabled by QWebEngine
    --without-debug
    --without-docbook
    --without-ftp
    --without-iconv
    --with-icu
    --without-legacy
    --with-output # required by xmlsec1
    --without-python
    --without-regexps
    --with-sax1 # required by xmlsec1
    --without-schematron
    --with-xptr # required by xmlsec1
    --without-modules
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-schemas>
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-xinclude>
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-modules>

  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_icu
)

