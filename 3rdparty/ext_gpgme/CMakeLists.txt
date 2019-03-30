ExternalProject_Add(
  ext_gpgme
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.13.0.tar.bz2
  URL_MD5 e511a0d95c507ab87e713140c82fc7d0

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --enable-silent-rules --enable-languages=cpp,qt --disable-gpgconf-test --disable-gpg-test --disable-gpgsm-test --disable-g13-test
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2
)
