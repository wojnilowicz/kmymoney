# Ubuntu 16.04 uses libgpg-error 1.21 and newer version won't be packaged into AppImage because this library is blacklisted, so to avoid missing symbols stay with old version

if(APPLE OR MINGW)
  ExternalProject_Add(
    ext_gpgerror
    ${PROJECT_GLOBAL_PROFILE}
    URL https://www.gnupg.org/ftp/gcrypt/libgpg-error/libgpg-error-1.36.tar.bz2
    URL_MD5 eff437f397e858a9127b76c0d87fa5ed

    CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
      ${AT_GLOBAL_PROFILE}
      --disable-doc
      --disable-tests
      --disable-nls
      --disable-languages
      --without-readline

    ${CLEANUP_COMMAND}
  )
else()
  ExternalProject_Add(
    ext_gpgerror
    ${PROJECT_GLOBAL_PROFILE}
    URL https://www.gnupg.org/ftp/gcrypt/libgpg-error/libgpg-error-1.21.tar.bz2
    URL_MD5 ab0b5aba6d0a185b41d07bda804fd8b2

    CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
      ${AT_GLOBAL_PROFILE}
      --disable-doc
      --disable-tests
      --disable-nls
      --disable-languages
      --without-readline

    ${CLEANUP_COMMAND}
  )
endif()