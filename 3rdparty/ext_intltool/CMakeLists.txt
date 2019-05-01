ExternalProject_Add(
  ext_intltool
  ${PROJECT_GLOBAL_PROFILE}
  URL https://launchpad.net/intltool/trunk/0.51.0/+download/intltool-0.51.0.tar.gz
  URL_MD5 12e517cac2b57a0121cda351570f1e63

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)
