if (APPLE)
  set(project_dependencies ext_xml ext_ncurses)
elseif(UNIX)
  set(project_dependencies ext_iconv ext_xml ext_ncurses)
endif ()

ExternalProject_Add(
  ext_gettext
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.19.8.1.tar.gz
  URL_MD5 97e034cf8ce5ba73a28ff6c3c0638092

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --disable-java --disable-native-java --enable-nls --enable-c++
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ${project_dependencies}
)
