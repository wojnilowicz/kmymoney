if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_png
  ${PROJECT_GLOBAL_PROFILE}
  URL https://kent.dl.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz
  URL_MD5 015e8e15db1eecde5f2eb9eb5b6e59e9

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
    $<$<PLATFORM_ID:Darwin>:-DCMAKE_MACOSX_RPATH=ON>
    $<$<PLATFORM_ID:Darwin>:-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}>
    -DPNG_TESTS=OFF
    -DPNG_DEBUG=OFF
    -DPNG_SHARED=ON
    -DPNG_STATIC=OFF
    -DPNG_TESTS=OFF

  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
)

