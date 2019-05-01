ExternalProject_Add(
  ext_fontconfig
  ${PROJECT_GLOBAL_PROFILE}
  URL http://files.kde.org/krita/build/dependencies/fontconfig-2.12.0.tar.gz
  URL_MD5 d8b056231abcb6257db6dc6d745360b2

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)
