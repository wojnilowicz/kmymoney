if (APPLE)
  set(iconv_dependency "")
elseif(UNIX)
  set(iconv_dependency --with-iconv=${CMAKE_INSTALL_PREFIX})
endif ()

# --without-catalog is requirement of QWebEngine
ExternalProject_Add(
  ext_xml
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxml2-2.9.9.tar.gz
  URL_MD5 c04a5a0a042eaa157e8e8c9eabe76bd6

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --without-python --with-icu --with-catalog --with-lzma=${CMAKE_INSTALL_PREFIX} ${iconv_dependency}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_readline ext_icu
)
