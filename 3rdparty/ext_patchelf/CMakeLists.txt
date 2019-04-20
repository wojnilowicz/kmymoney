ExternalProject_Add(
  ext_patchelf
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2
  URL_MD5 d02687629c7e1698a486a93a0d607947

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
