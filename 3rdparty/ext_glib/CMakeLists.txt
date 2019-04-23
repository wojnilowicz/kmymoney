if(APPLE)
  set (platform_specific_option -Diconv=native)
else()
  set (platform_specific_option "")
endif ()

ExternalProject_Add(
  ext_glib
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.acc.umu.se/pub/gnome/sources/glib/2.61/glib-2.61.0.tar.xz
  URL_MD5 2a2148436b63088ea77243f1222e5e10

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-tests.diff
#   PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/glib.diff
#         COMMAND bash -c "sed -i 's+@myprefix+${CMAKE_INSTALL_PREFIX}+g' meson.build"

  CONFIGURE_COMMAND meson _build --buildtype=release -Dprefix=${CMAKE_INSTALL_PREFIX} -Dselinux=disabled -Dman=false -Dgtk_doc=false -Dinstalled_tests=false -Dnls=disabled ${platform_specific_option}
  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ninja -C _build
  INSTALL_COMMAND ninja -C _build install
)

