ExternalProject_Add(
  ext_ncurses
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.1.tar.gz
  URL_MD5 98c889aaf8d23910d2b92d65be2e737a

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --enable-overwrite --enable-pc-files --without-tests --with-shared --with-cxx-shared --without-ada --with-pkg-config-libdir=${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
