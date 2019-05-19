if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_jpeg
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.sourceforge.net/libjpeg-turbo/libjpeg-turbo-2.0.2.tar.gz
  URL_MD5 79f76fbfb0c6109631332762d10e16d2

CMAKE_ARGS
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
  $<$<PLATFORM_ID:Darwin>:-DCMAKE_MACOSX_RPATH=ON>
  $<$<PLATFORM_ID:Darwin>:-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}>
  -DENABLE_STATIC=OFF
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_nasm
)
