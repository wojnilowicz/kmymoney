# --without-catalog is requirement of QWebEngine
if(APPLE)
  set (project_dependencies "")
  set (qt_platform_specific_option "")
elseif (UNIX OR MINGW)
  set (project_dependencies ext_iconv)
  set (qt_platform_specific_option --with-iconv=${UNIX_INSTALL_PREFIX})
endif ()

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
    --without-python
    --with-icu
    --with-catalog
    ${qt_platform_specific_option}

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_icu ${project_dependencies}
)
