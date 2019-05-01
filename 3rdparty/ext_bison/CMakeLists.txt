ExternalProject_Add(
  ext_bison
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.gnu.org/gnu/bison/bison-3.3.tar.xz
  URL_MD5 7a8aae8e8db86180fb2f3d853d147e31

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-nls

  ${CLEANUP_COMMAND}
)
