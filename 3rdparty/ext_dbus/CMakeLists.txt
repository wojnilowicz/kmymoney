# cmake on Ubuntu 16.04 cannot use SOURCE_SUBDIR, so autotools is the only way
if(MINGW)
  set(platform_specific_dependencies ext_expat)
  set(platform_specific_options --with-dbus-session-bus-listen-address=autolaunch:scope=*install-path
                                                  --with-dbus-session-bus-connect-address=autolaunch:scope=*install-path
                                                  --with-system-socket=autolaunch:scope=*install-path
                                                  --datarootdir=${UNIX_INSTALL_PREFIX}/bin/data)
elseif(APPLE)
  set(platform_specific_dependencies ext_expat)
  set(platform_specific_options --without-x
                                                  --enable-launchd
                                                  --with-launchd-agent-dir=${UNIX_INSTALL_PREFIX}/Library/LaunchAgents)
else()
  set(platform_specific_dependencies "")
  set(platform_specific_options --enable-x11-autolaunch)
endif()

ExternalProject_Add(
  ext_dbus
  ${PROJECT_GLOBAL_PROFILE}
  URL https://dbus.freedesktop.org/releases/dbus/dbus-1.12.14.tar.gz
  URL_MD5 9e748b8c50dae17ae639f1c55ae6417d

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.11.4-20160903.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.12.12-launchd.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-fix_data_dir.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-developer
    --enable-debug=no
    --disable-verbose-mode
    --disable-asserts
    --disable-xml-docs
    --disable-doxygen-docs
    --disable-ducktype-docs
    --disable-selinux
    --disable-apparmor
    --disable-libaudit
    --disable-inotify
    --disable-kqueue
    --disable-systemd
    --disable-embedded-tests
    --disable-modular-tests
    --disable-tests
    --disable-installed-tests
    --disable-code-coverage
    --disable-epoll
    --disable-compile-warnings
    --enable-relocation=yes
    --disable-stats
    --disable-user-session
    --without-gcov
    --without-valgrind
    ${platform_specific_options}

  BUILD_IN_SOURCE 1 # otherwise no Win32.Manifest could be found on MS Windows
  ${CLEANUP_COMMAND}
  DEPENDS ${platform_specific_dependencies}
)

# Without this Qt cannot find dbus easilly
if(MINGW)
  ExternalProject_Add_Step(
  ext_dbus copyincludes
  COMMAND cp -fr dbus-1.0/dbus .
  COMMAND cp -fr ${CMAKE_INSTALL_PREFIX}/lib/dbus-1.0/include/dbus .

  DEPENDEES install
  WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include
  )
endif()
