set (glib_libs "-L${UNIX_INSTALL_PREFIX}/lib -lintl -lglib-2.0")
set (glib_cflags "-I${UNIX_INSTALL_PREFIX}/include/glib-2.0 -I${UNIX_INSTALL_PREFIX}/lib/glib-2.0/include -mms-bitfields")

ExternalProject_Add(
  ext_pkgconfig
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://pkg-config.freedesktop.org/releases/pkg-config-0.29.2.tar.gz
  URL_MD5 f6e931e319531b736fadc017f470e68a

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    GLIB_LIBS=${glib_libs}
    GLIB_CFLAGS=${glib_cflags}

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
  DEPENDS ext_glib
)
