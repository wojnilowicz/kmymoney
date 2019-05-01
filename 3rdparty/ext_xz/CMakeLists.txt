ExternalProject_Add(
  ext_xz
  ${PROJECT_GLOBAL_PROFILE}
  URL http://tukaani.org/xz/xz-5.2.4.tar.xz
  URL_MD5 003e4d0b1b1899fc6e3000b24feddf7c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-lzmadec
    --disable-lzmainfo
    --disable-scripts
    --disable-doc
    --disable-nls
    --disable-xz
    --disable-xzdec
    --disable-debug

  ${CLEANUP_COMMAND}
)
