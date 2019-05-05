# With "no-deprecated" nettle shows undefined reference to `ERR_load_crypto_strings'
if(MINGW)
  set(platform_specific_option <SOURCE_DIR>/Configure mingw64)
else()
  set(platform_specific_option <SOURCE_DIR>/config)
endif ()

ExternalProject_Add(
  ext_openssl
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.openssl.org/source/openssl-1.1.1b.tar.gz
  URL_MD5 4532712e7bcc9414f5bce995e4e13930

  CONFIGURE_COMMAND ${platform_specific_option}
    --prefix=${CMAKE_INSTALL_PREFIX}
    --openssldir=${CMAKE_INSTALL_PREFIX}/ssl
    no-tests no-ui no-static-engine

  INSTALL_COMMAND $(MAKE) install_sw
  ${CLEANUP_COMMAND}
)

