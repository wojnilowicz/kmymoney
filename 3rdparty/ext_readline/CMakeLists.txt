ExternalProject_Add(
  ext_readline
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/gnu/readline/readline-8.0.tar.gz
  URL_MD5 7e6c1f16aee3244a69aba6e438295ca3

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --with-curses
    --disable-install-examples
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_ncurses
)
