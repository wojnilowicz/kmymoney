ExternalProject_Add( ext_png
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://kent.dl.sourceforge.net/project/libpng/libpng16/1.6.36/libpng-1.6.36.tar.xz
  URL_MD5 df2be2d29c40937fe1f5349b16bc2826

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DPNG_TESTS=OFF
)
