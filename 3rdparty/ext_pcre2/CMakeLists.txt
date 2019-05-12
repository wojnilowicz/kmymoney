ExternalProject_Add(
  ext_pcre2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre2-10.33.tar.gz
  URL_MD5 8988af53875ecdbce57fdc1bb3c4868b

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_SHARED_LIBS=ON
             -DPCRE2_BUILD_PCRE2GREP=OFF
             -DPCRE2_BUILD_PCRE2_8=OFF
             -DPCRE2_BUILD_PCRE2_16=ON
             -DPCRE2_BUILD_PCRE2_32=OFF
             -DPCRE2_BUILD_TESTS=OFF
             -DPCRE2_DEBUG=OFF
             -DPCRE2_NEWLINE=LF
             -DPCRE2_SUPPORT_JIT=ON
             -DPCRE2_SUPPORT_UNICODE=ON
  
  ${CLEANUP_COMMAND}
)
