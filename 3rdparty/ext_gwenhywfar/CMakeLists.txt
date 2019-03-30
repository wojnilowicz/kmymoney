set(gwenhywfar_guis "qt5 cpp")

ExternalProject_Add(
  ext_gwenhywfar
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.aquamaniac.de/sites/download/download.php?package=01&release=208&file=02&dummy=gwenhywfar-4.20.0.tar.gz
  URL_MD5 0744255cae68adca6dff81d3a9f3932d

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_SHARED_ONLY} --with-guis=${gwenhywfar_guis} CPPFLAGS=${AT_CPP_FLAGS} LDFLAGS=${AT_LD_FLAGS}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gnutls ext_gcrypt
)
