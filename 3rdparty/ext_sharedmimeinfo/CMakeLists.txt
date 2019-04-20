ExternalProject_Add(
  ext_sharedmimeinfo
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://people.freedesktop.org/~hadess/shared-mime-info-1.10.tar.xz
  URL_MD5 418c2ced9dc4dd5ca8b06a755e6d64e9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY} ${AUTOMAKE_DEFAULT_HOST} ${AUTOMAKE_SHARED_ONLY} --disable-update-mimedb
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_intltool
)
