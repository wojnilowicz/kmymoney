ExternalProject_Add(
  ext_gnutls
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/gnutls/v3.6/gnutls-3.6.7.tar.xz
  URL_MD5 c4ac669c500df939d4fbfea722367929

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY} --disable-tests --disable-doc --enable-openssl-compatibility -disable-gtk-doc --enable-cxx --without-p11-kit CPPFLAGS=${AT_CPP_FLAGS} LDFLAGS=${AT_LD_FLAGS}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2 ext_gpgerror ext_nettle ext_tasn1 ext_unistring
)
