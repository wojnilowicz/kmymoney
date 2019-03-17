ExternalProject_Add(
  ext_lzma
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://tukaani.org/xz/xz-5.2.4.tar.xz
  URL_MD5 003e4d0b1b1899fc6e3000b24feddf7c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} --disable-doc
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install
)
