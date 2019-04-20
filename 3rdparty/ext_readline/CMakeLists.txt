ExternalProject_Add(
  ext_readline
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/gnu/readline/readline-8.0.tar.gz
  URL_MD5 7e6c1f16aee3244a69aba6e438295ca3
#  PATCH_COMMAND bash -c "find <SOURCE_DIR> -type f -name Makefile.in -exec sed -i -e 's+\\d036{BUILD_DIR}/config.h+\\d036(topdir)/config.h+g' -e 's+\\d036(BUILD_DIR)/config.h+\\d036(topdir)/config.h+g' {} \\+"
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    --with-curses
    --disable-install-examples
    LDFLAGS=${AT_LD_FLAGS}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_ncurses
)
#--disable-largefile
