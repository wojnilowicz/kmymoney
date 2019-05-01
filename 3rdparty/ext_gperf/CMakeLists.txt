ExternalProject_Add(
  ext_gperf
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/pub/gnu/gperf/gperf-3.1.tar.gz
  URL_MD5 9e251c0a618ad0824b51117d5d9db87e

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)
