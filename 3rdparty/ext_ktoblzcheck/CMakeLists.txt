ExternalProject_Add(
  ext_ktoblzcheck
  ${PROJECT_GLOBAL_PROFILE}
  URL http://downloads.sourceforge.net/project/ktoblzcheck/ktoblzcheck-1.49.tar.gz
  URL_MD5 2ea1b01a931eb1e52b0ca0e07cb72435

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)
#--with-bankdata-path=${UNIX_INSTALL_PREFIX}/bin/data/ktoblzcheck