# autotools outputs pkg-config file and cmake not
ExternalProject_Add(
  ext_pcre
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre-8.43.tar.gz
  URL_MD5 e775489c9f024a92ca0f431301ef4c5c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-pcre8
    --disable-pcre16
    --disable-pcre32
    --disable-cpp
    --enable-jit
    --disable-pcregrep-jit
    --enable-utf # required by glib
    --enable-newline-is-any # required by glib
    --disable-bsr-anycrlf # required by glib
    --enable-stack-for-recursion # required by KJs
    --disable-pcregrep-libz
    --disable-pcregrep-libbz2
    --disable-pcretest-libedit
    --disable-pcretest-libreadline
    --disable-valgrind
    --disable-coverage

  ${CLEANUP_COMMAND}
)
