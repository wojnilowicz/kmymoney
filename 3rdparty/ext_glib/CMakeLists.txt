ExternalProject_Add(
  ext_glib
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.acc.umu.se/pub/gnome/sources/glib/2.61/glib-2.61.0.tar.xz
  URL_MD5 2a2148436b63088ea77243f1222e5e10

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-tests.diff
#   PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/glib.diff
#         COMMAND bash -c "sed -i 's+@myprefix+${CMAKE_INSTALL_PREFIX}+g' meson.build"

  CONFIGURE_COMMAND meson _build
                          --buildtype=release
                          -Dprefix=${CMAKE_INSTALL_PREFIX}
                          -Dselinux=disabled
                          -Dxattr=false
                          -Dlibmount=false
                          -Dinternal_pcre=false
                          -Dman=false
                          -Dgtk_doc=false
                          -Dinstalled_tests=false
                          -Dnls=disabled
                          $<$<PLATFORM_ID:Linux>:--libdir=lib>

  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ninja -C _build
  INSTALL_COMMAND ninja -C _build install
  ${CLEANUP_COMMAND}
  DEPENDS ext_pcre
)
