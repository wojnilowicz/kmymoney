ExternalProject_Add(
  ext_assuan2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/libassuan/libassuan-2.5.3.tar.bz2
  URL_MD5 226c504ea78a232224bf3b6846b3adb9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-doc

  ${CLEANUP_COMMAND}

  DEPENDS ext_gpgerror
)
