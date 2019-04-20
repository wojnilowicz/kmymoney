ExternalProject_Add(
  ext_assuan2
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/libassuan/libassuan-2.5.3.tar.bz2
  URL_MD5 226c504ea78a232224bf3b6846b3adb9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --disable-doc

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gpgerror
)
