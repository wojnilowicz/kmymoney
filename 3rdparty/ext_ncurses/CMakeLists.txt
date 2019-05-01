# Without --enable-term-driver there are linking errors on MinGW.
ExternalProject_Add(
  ext_ncurses
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.1.tar.gz
  URL_MD5 98c889aaf8d23910d2b92d65be2e737a

  # otherwise shlib gets invoked with PATH variable converted to Windows style paths
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/ncurses_mingw.diff
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-overwrite
    --enable-pc-files
    --without-tests
    --without-manpages
    --with-shared
    --with-cxx-shared
    --without-ada
    --disable-home-terminfo
    --enable-term-driver
    --without-termlib
    --enable-sp-funcs
    --enable-interop
    --without-debug
    --without-normal
    --disable-widec
    --enable-rpath
    --with-pkg-config-libdir=${UNIX_INSTALL_PREFIX}/lib/pkgconfig

  ${CLEANUP_COMMAND}
)
