# without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs
ExternalProject_Add(
  ext_gmp
  ${PROJECT_GLOBAL_PROFILE}
  URL https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
  URL_MD5 8ddbb26dc3bd4e2302984debba1406a5

  CONFIGURE_COMMAND ./configure
    ${AT_GLOBAL_PROFILE}
    --enable-cxx # required by KMyMoney
    --without-readline
  BUILD_IN_SOURCE 1 # MinGW needs it
  ${CLEANUP_COMMAND}
)
