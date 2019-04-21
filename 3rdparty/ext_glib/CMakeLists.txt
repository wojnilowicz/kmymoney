if(APPLE)
  set (project_dependencies ext_zlib)
  set (qt_platform_specific_option -Diconv=native)
elseif(MINGW)
  set (project_dependencies ext_zlib ext_iconv)
  set (qt_platform_specific_option "")
else()
  set (project_dependencies ext_iconv )
  set (qt_platform_specific_option "")
endif ()

ExternalProject_Add(
  ext_glib
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://ftp.acc.umu.se/pub/gnome/sources/glib/2.60/glib-2.60.0.tar.xz
  URL_MD5 7d36520dda58de65027abf5b4fb1241a

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/glib.diff
        COMMAND bash -c "sed -i 's+@myprefix+${CMAKE_INSTALL_PREFIX}+g' meson.build"

  CONFIGURE_COMMAND meson _build --buildtype=release -Dprefix=${CMAKE_INSTALL_PREFIX} -Dselinux=disabled -Dman=false -Dgtk_doc=false -Dinstalled_tests=false -Dnls=disabled ${qt_platform_specific_option}
  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ninja -C _build
  INSTALL_COMMAND ninja -C _build install
  DEPENDS ${project_dependencies}
)

