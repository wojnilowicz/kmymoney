if(MINGW)
  ExternalProject_Add(
    ext_gmp
    DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
    URL https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
    URL_MD5 8ddbb26dc3bd4e2302984debba1406a5

    CONFIGURE_COMMAND ./configure
      --prefix=${UNIX_INSTALL_PREFIX}
      --host=${AT_SYSTEM_TYPE}
      ${AT_SHARED_ONLY}
      ${AT_LESS_PROCESSING}
      --enable-cxx
    BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
    ${CLEANUP_COMMAND}
  )
else()
  ExternalProject_Add(
    ext_gmp
    DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
    URL https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
    URL_MD5 8ddbb26dc3bd4e2302984debba1406a5

    CONFIGURE_COMMAND <SOURCE_DIR>/configure
      --prefix=${UNIX_INSTALL_PREFIX}
      --host=${AT_SYSTEM_TYPE}
      ${AT_SHARED_ONLY}
      ${AT_LESS_PROCESSING}
      --enable-cxx
    BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
    INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
    ${CLEANUP_COMMAND}
  )
endif ()
