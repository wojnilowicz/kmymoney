ExternalProject_Add(
  ext_pcre2
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.pcre.org/pub/pcre/pcre2-10.32.zip
  URL_MD5 e0e5b1f7e425b73dcdcea0e08a85da31

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --host=${AT_SYSTEM_TYPE} ${AT_SHARED_ONLY} --enable-unicode --enable-pcre2-16 --enable-pcre2-32 --enable-jit
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
