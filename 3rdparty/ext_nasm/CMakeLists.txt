ExternalProject_Add(
  ext_nasm
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.nasm.us/pub/nasm/releasebuilds/2.14/nasm-2.14.tar.xz
  URL_MD5 7d0f554cacd6c5021b3cda3ba9f2474c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
