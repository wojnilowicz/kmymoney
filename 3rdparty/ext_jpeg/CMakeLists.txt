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
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_jpeg
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.sourceforge.net/libjpeg-turbo/libjpeg-turbo-2.0.4.tar.gz
  URL_MD5 d01d9e0c28c27bc0de9f4e2e8ff49855

CMAKE_ARGS
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
  $<$<PLATFORM_ID:Darwin>:-DCMAKE_MACOSX_RPATH=ON>
  $<$<PLATFORM_ID:Darwin>:-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}>
  -DENABLE_STATIC=OFF
  -DWITH_TURBOJPEG=OFF
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_nasm
)
