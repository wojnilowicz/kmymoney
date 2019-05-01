ExternalProject_Add(
  ext_pcre2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre2-10.32.zip
  URL_MD5 e0e5b1f7e425b73dcdcea0e08a85da31

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-unicode
    --enable-pcre2-16
    --enable-pcre2-32
    --enable-jit

  ${CLEANUP_COMMAND}
)
