ExternalProject_Add(
  ext_patchelf
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2
  URL_MD5 d02687629c7e1698a486a93a0d607947

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${EXT_INSTALL_DIR} ${AT_HOST}
  BUILD_COMMAND ${CMAKE_MAKE_PROGRAM}
  INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
)
