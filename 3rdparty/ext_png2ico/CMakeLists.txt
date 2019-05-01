ExternalProject_Add(
  ext_png2ico
  ${PROJECT_GLOBAL_PROFILE}
  URL http://www.winterdrache.de/freeware/png2ico/data/png2ico-win-2002-12-08.zip
  URL_MD5 5be668b436990255b75cce4ce03be282

  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND bash -c "cp -f <SOURCE_DIR>/png2ico.exe ${CMAKE_INSTALL_PREFIX}/bin/png2ico.exe"
  ${CLEANUP_COMMAND}
)
