#  *
#  * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
#  *
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License as
#  * published by the Free Software Foundation; either version 2 of
#  * the License, or (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  *

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
  URL https://dbus.freedesktop.org/releases/dbus/dbus-1.13.16.tar.xz
  URL_MD5 239ea77c456c81c5e64231666ecb05d1

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.12.12-launchd.diff

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
