if(MINGW)
  set(platform_specific_option <SOURCE_DIR>/Configure mingw64)
else()
  set(platform_specific_option <SOURCE_DIR>/config)
endif ()

ExternalProject_Add(
  ext_openssl
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.openssl.org/source/openssl-1.1.1b.tar.gz
  URL_MD5 4532712e7bcc9414f5bce995e4e13930

  CONFIGURE_COMMAND ${platform_specific_option}
    --prefix=${CMAKE_INSTALL_PREFIX}
    --openssldir=${CMAKE_INSTALL_PREFIX}/ssl
    no-tests
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install_sw
  ${CLEANUP_COMMAND}
)
