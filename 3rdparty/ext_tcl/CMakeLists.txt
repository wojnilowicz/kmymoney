ExternalProject_Add(
  ext_tcl
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://github.com/tcltk/tcl/archive/core-8-6-9.zip
  URL_MD5 f402cc573e6b486b65bda0dcbd74000a

  CONFIGURE_COMMAND <SOURCE_DIR>/unix/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --enable-threads --enable-64bit
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
