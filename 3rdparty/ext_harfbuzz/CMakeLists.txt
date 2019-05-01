# this library isn't shipped with AppImage as per
# https://github.com/AppImage/AppImages/blob/master/excludelist#L126

ExternalProject_Add(
  ext_harfbuzz
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.freedesktop.org/software/harfbuzz/release/harfbuzz-2.0.0.tar.bz2
  URL_MD5 6d1373dba3e8540c448c6a562f2e0a00

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --with-icu=yes
    --disable-gtk-doc-html

  ${CLEANUP_COMMAND}

  DEPENDS ext_icu
)
