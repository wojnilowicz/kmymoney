ExternalProject_Add(
  ext_gperf
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.gnu.org/pub/gnu/gperf/gperf-3.1.tar.gz
  URL_MD5 9e251c0a618ad0824b51117d5d9db87e

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
