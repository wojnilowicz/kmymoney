ExternalProject_Add(
  ext_expat
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/libexpat/libexpat/releases/download/R_2_2_6/expat-2.2.6.tar.bz2
  URL_MD5 ca047ae951b40020ac831c28859161b2

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --without-xmlwf
    --without-docbook

  ${CLEANUP_COMMAND}
)
