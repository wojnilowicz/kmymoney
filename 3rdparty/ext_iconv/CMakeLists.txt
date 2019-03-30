ExternalProject_Add(
  ext_iconv
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz
  URL_MD5 ace8b5f2db42f7b3b3057585e80d9808

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --enable-relocatable --disable-documentation
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  TEST_COMMAND rm -fr <SOURCE_DIR> && rm -fr <BINARY_DIR>
  ${CLEANUP_COMMAND}
)
