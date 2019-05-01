# this library isn't shipped with AppImage as per
# https://github.com/AppImage/AppImages/blob/master/excludelist#L126
if(MINGW)
  ExternalProject_Add(
    ext_zlib
    DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
    URL https://zlib.net/zlib-1.2.11.tar.gz
    URL_MD5 1c9f62f0778697a09d36121ead88e08e
    CONFIGURE_COMMAND ""
    BUILD_COMMAND $(MAKE) -fwin32/Makefile.gcc
    INSTALL_COMMAND $(MAKE)
      prefix=${UNIX_INSTALL_PREFIX}
      BINARY_PATH=${UNIX_INSTALL_PREFIX}/bin
      INCLUDE_PATH=${UNIX_INSTALL_PREFIX}/include
      LIBRARY_PATH=${UNIX_INSTALL_PREFIX}/lib
      install -fwin32/Makefile.gcc SHARED_MODE=1

    ${CLEANUP_COMMAND}
    BUILD_IN_SOURCE 1
  )
else()
  ExternalProject_Add(
    ext_zlib
    DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
    URL https://zlib.net/zlib-1.2.11.tar.gz
    URL_MD5 1c9f62f0778697a09d36121ead88e08e

    CONFIGURE_COMMAND sh <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX}
    ${CLEANUP_COMMAND}
)
endif ()


