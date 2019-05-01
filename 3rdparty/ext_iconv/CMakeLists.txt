ExternalProject_Add(
  ext_iconv
  ${PROJECT_GLOBAL_PROFILE}

  URL https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz
  URL_MD5 ace8b5f2db42f7b3b3057585e80d9808

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-documentation
    --disable-nls
    --disable-extra-encodings

  ${CLEANUP_COMMAND}
)

