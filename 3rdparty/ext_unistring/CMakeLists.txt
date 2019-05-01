ExternalProject_Add(
  ext_unistring
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://ftp.gnu.org/gnu/libunistring/libunistring-0.9.10.tar.xz
  URL_MD5 db08bb384e81968957f997ec9808926e

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)
