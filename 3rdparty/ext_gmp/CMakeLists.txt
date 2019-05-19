# without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs

if(MINGW)
  ExternalProject_Add(
    ext_gmp
    ${PROJECT_GLOBAL_PROFILE}
    URL https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
    URL_MD5 8ddbb26dc3bd4e2302984debba1406a5

    CONFIGURE_COMMAND ./configure
      ${AT_GLOBAL_PROFILE}
      --enable-cxx # required by KMyMoney
    BUILD_IN_SOURCE 1
    ${CLEANUP_COMMAND}
  )
else()
  ExternalProject_Add(
    ext_gmp
    ${PROJECT_GLOBAL_PROFILE}
    URL https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
    URL_MD5 8ddbb26dc3bd4e2302984debba1406a5

    CONFIGURE_COMMAND <SOURCE_DIR>/configure
      ${AT_GLOBAL_PROFILE}
      --enable-cxx # required by KMyMoney
    ${CLEANUP_COMMAND}
  )
endif()
