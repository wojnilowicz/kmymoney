ExternalProject_Add(
  ext_icu
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.icu-project.org/files/icu4c/63.1/icu4c-63_1-src.tgz
  URL_MD5 9e40f6055294284df958200e308bce50

  CONFIGURE_COMMAND <SOURCE_DIR>/source/configure --prefix=${CMAKE_INSTALL_PREFIX} --disable-tests
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install
)
