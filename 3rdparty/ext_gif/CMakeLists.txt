if(MINGW)
  set(platform_specific_build_prefix export CC="x86_64-w64-mingw32-gcc" &&)
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/giflib-mingw.diff)
else()
  set(platform_specific_build_prefix "" )
  set(platform_specific_patch "")
endif()

ExternalProject_Add(
  ext_gif
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.sourceforge.net/project/giflib/giflib-5.1.9.tar.gz
  URL_MD5 179336e739eeacee4cef2a8f789a0fcb
  ${platform_specific_patch}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${platform_specific_build_prefix} $(MAKE)
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND $(MAKE) PREFIX=${UNIX_INSTALL_PREFIX} install
  ${CLEANUP_COMMAND}
)
