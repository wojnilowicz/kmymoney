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

if(APPLE)
  set(platform_specific_options "")
elseif(MINGW)
  set(platform_specific_options "")
else()
  set(platform_specific_options $<$<PLATFORM_ID:Linux>:-Dxattr=false>
                                                  $<$<PLATFORM_ID:Linux>:-Dlibmount=false>
                                                  $<$<PLATFORM_ID:Linux>:--libdir=lib>)
endif()

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
                          -Dinternal_pcre=false
                          -Dman=false
                          -Dgtk_doc=false
                          -Dinstalled_tests=false
                          -Dnls=disabled
                          ${platform_specific_options}

  BUILD_IN_SOURCE TRUE
  BUILD_COMMAND ninja -C _build
  INSTALL_COMMAND ninja -C _build install
  ${CLEANUP_COMMAND}
  DEPENDS ext_pcre
)

