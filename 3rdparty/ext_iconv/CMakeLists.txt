ExternalProject_Add(
  ext_iconv
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz
  URL_MD5 ace8b5f2db42f7b3b3057585e80d9808

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --enable-relocatable
    --disable-documentation
    --disable-nls
    --disable-extra-encodings

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
