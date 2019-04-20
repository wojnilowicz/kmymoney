ExternalProject_Add(
  ext_bison
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://ftp.gnu.org/gnu/bison/bison-3.3.tar.xz
  URL_MD5 7a8aae8e8db86180fb2f3d853d147e31

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_LESS_PROCESSING}
    --enable-relocatable
    --disable-nls
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
