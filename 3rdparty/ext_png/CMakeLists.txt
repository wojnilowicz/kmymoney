ExternalProject_Add(
  ext_png
  ${PROJECT_GLOBAL_PROFILE}
  URL https://kent.dl.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz
  URL_MD5 015e8e15db1eecde5f2eb9eb5b6e59e9

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DPNG_TESTS=OFF
             -DPNG_DEBUG=OFF
             -DPNG_SHARED=ON
             -DPNG_STATIC=OFF
             -DPNG_TESTS=OFF

  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
)
