ExternalProject_Add(
  ext_glib
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.acc.umu.se/pub/gnome/sources/glib/2.60/glib-2.60.0.tar.xz
  URL_MD5 7d36520dda58de65027abf5b4fb1241a

  CONFIGURE_COMMAND meson _build -Diconv=native -Dprefix=${EXT_INSTALL_DIR}
  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ninja -C _build
  INSTALL_COMMAND ninja -C _build install
  DEPENDS ext_gettext ext_xml
)
