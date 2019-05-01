ExternalProject_Add(
  ext_nasm
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.nasm.us/pub/nasm/releasebuilds/2.14/nasm-2.14.tar.xz
  URL_MD5 7d0f554cacd6c5021b3cda3ba9f2474c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-gdb
    --disable-ccache

  ${CLEANUP_COMMAND}
)


