ExternalProject_Add(
  ext_flex
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
  URL_MD5 2882e3179748cc9f9c23ec593d6adc8d
  CONFIGURE_COMMAND HELP2MAN=/tools/bin/true <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY}
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install
  DEPENDS ext_bison
)
