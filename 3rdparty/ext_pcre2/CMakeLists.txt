# autotools outputs pkg-config file and cmake not
ExternalProject_Add(
  ext_pcre2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre2-10.33.tar.gz
  URL_MD5 8988af53875ecdbce57fdc1bb3c4868b

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-pcre2-8
    --enable-pcre2-16
    --disable-pcre2-32
    --disable-debug
    --enable-jit
    --disable-pcre2grep-jit
    --disable-pcre2grep-callout
    --disable-pcre2grep-callout-fork
    --enable-unicode
    --enable-newline-is-lf
    --disable-bsr-anycrlf
    --enable-stack-for-recursion
    --disable-pcre2grep-libz
    --disable-pcre2grep-libbz2
    --disable-pcre2test-libedit
    --disable-pcre2test-libreadline
    --disable-valgrind
    --disable-coverage
  
  ${CLEANUP_COMMAND}
)N
