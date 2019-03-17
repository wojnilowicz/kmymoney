if (APPLE)
  set(project_dependencies ext_lzma ext_readline)
elseif(UNIX)
  set(project_dependencies ext_lzma ext_readline ext_icu)
endif ()

# --without-catalog is requirement of QWebEngine
ExternalProject_Add(
  ext_xml
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxml2-2.9.8.tar.gz
  URL_MD5 b786e353e2aa1b872d70d5d1ca0c740d

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} --without-python --with-icu --without-catalog --with-lzma=${CMAKE_INSTALL_PREFIX}/lib CPPFLAGS=${AT_CPP_FLAGS} LDFLAGS=${AT_LD_FLAGS}
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install
  DEPENDS ${project_dependencies}
)
