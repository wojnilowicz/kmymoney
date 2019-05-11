ExternalProject_Add(
  ext_pcre
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre-8.43.tar.gz
  URL_MD5 e775489c9f024a92ca0f431301ef4c5c

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_SHARED_LIBS=ON
             -DPCRE_BUILD_PCRE8=ON
             -DPCRE_BUILD_PCRE16=OFF
             -DPCRE_BUILD_PCRE32=OFF
             -DPCRE_BUILD_PCRECPP=OFF
             -DPCRE_BUILD_PCREGREP=OFF
             -DPCRE_BUILD_TESTS=OFF
             -DPCRE_EBCDIC=OFF # required by glib
             -DPCRE_NEWLINE=ANY # required by glib
             -DPCRE_NO_RECURSE=OFF # required by KJs
             -DPCRE_SUPPORT_BSR_ANYCRLF=OFF # required by glib
             -DPCRE_SUPPORT_UTF=ON # required by glib
             -DPCRE_SUPPORT_LIBEDIT=OFF
             -DPCRE_SUPPORT_LIBREADLINE=OFF

  ${CLEANUP_COMMAND}
)
