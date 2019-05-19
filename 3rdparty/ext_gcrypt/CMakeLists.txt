ExternalProject_Add(
  ext_gcrypt
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/libgcrypt/libgcrypt-1.8.4.tar.bz2
  URL_MD5 fbfdaebbbc6d7e5fbbf6ffdb3e139573

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-doc
    --disable-arm-crypto-support
    --disable-neon-support
    --disable-padlock-support
    --enable-large-data-tests

  ${CLEANUP_COMMAND}

  DEPENDS ext_gpgerror
)
