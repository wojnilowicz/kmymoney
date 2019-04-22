ExternalProject_Add( ext_dbus
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  SOURCE_SUBDIR cmake
  URL https://dbus.freedesktop.org/releases/dbus/dbus-1.12.12.tar.gz
  URL_MD5 ea11069521beeee4e47f0086596a43c8

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.11.4-20160903.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.12.12-launchd.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-fix_data_dir.diff

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DDBUS_BUILD_TESTS=OFF
             -DDBUS_ENABLE_XML_DOCS=OFF
             -DDBUS_DISABLE_ASSERT=ON
             -DDBUS_ENABLE_VERBOSE_MODE=OFF
             -DCMAKE_INSTALL_DATADIR:STRING=bin/data
             -DDBUS_SESSION_BUS_LISTEN_ADDRESS:STRING=autolaunch:scope=*install-path
             -DDBUS_SESSION_BUS_CONNECT_ADDRESS:STRING=autolaunch:scope=*install-path
             -DDBUS_SYSTEM_BUS_DEFAULT_ADDRESS:STRING=autolaunch:scope=*install-path
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_expat ext_glib
)
