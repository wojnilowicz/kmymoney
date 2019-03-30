# this library isn't shipped with AppImage as per
# https://github.com/AppImage/AppImages/blob/master/excludelist#L126

ExternalProject_Add(
  ext_freetype
	DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
	URL https://quantum-mirror.hu/mirrors/pub/gnusavannah/freetype/freetype-2.9.1.tar.bz2
  URL_MD5 60ef7d8160cd4bf8cb118ee9d65367ca

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --enable-freetype-config
  BUILD_ALWAYS TRUE
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} clean
  COMMAND ${CMAKE_MAKE_PROGRAM}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
