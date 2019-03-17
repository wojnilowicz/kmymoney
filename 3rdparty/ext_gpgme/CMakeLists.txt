ExternalProject_Add(
  ext_gpgme
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.12.0.tar.bz2
  URL_MD5 902fca3a94907efe4e929b2ade545a7c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --enable-languages=cpp,qt --disable-gpg-test
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install

  DEPENDS ext_assuan2
)
