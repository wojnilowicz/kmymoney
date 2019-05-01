ExternalProject_Add(
  ext_flex
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
  URL_MD5 2882e3179748cc9f9c23ec593d6adc8d

  CONFIGURE_COMMAND HELP2MAN=/tools/bin/true <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}

  DEPENDS ext_bison
)
