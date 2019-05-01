ExternalProject_Add(
  ext_jpeg
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.sourceforge.net/libjpeg-turbo/libjpeg-turbo-2.0.2.tar.gz
  URL_MD5 79f76fbfb0c6109631332762d10e16d2

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DENABLE_STATIC=OFF
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_nasm
)
