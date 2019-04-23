ExternalProject_Add(
  ext_xz
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://tukaani.org/xz/xz-5.2.4.tar.xz
  URL_MD5 003e4d0b1b1899fc6e3000b24feddf7c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --disable-lzmadec
    --disable-lzmainfo
    --disable-scripts
    --disable-doc
    --disable-nls
    --disable-xz
    --disable-xzdec
    --disable-debug
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
