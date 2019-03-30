ExternalProject_Add(
  ext_icu
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.icu-project.org/files/icu4c/64.1/icu4c-64_1-src.tgz
  URL_MD5 f150be2231c13bb45206d79e0242372b

  CONFIGURE_COMMAND <SOURCE_DIR>/source/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} --disable-draft --enable-rpath --disable-extras --disable-icuio --enable-icu-config --disable-tests --disable-samples
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
