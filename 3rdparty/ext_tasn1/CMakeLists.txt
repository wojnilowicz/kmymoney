ExternalProject_Add(
  ext_tasn1
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.gnu.org/gnu/libtasn1/libtasn1-4.13.tar.gz
  URL_MD5 ce2ba4d3088119b48e7531a703669c52

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)

