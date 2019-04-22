ExternalProject_Add( ext_png
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://kent.dl.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz
  URL_MD5 015e8e15db1eecde5f2eb9eb5b6e59e9

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DPNG_TESTS=OFF
  BUILD_IN_SOURCE 1
  ${CLEANUP_COMMAND}
)
