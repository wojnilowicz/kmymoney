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

if(MINGW)
  set(platform_specific_build_prefix export CC="x86_64-w64-mingw32-gcc" &&)
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/giflib-mingw.diff)
elseif(APPLE)
  set(platform_specific_build_prefix "")
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/giflib-macos.diff)
else()
  set(platform_specific_build_prefix "")
  set(platform_specific_patch "")
endif()

if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_gif
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.sourceforge.net/project/giflib/giflib-5.2.1.tar.gz
  URL_MD5 6f03aee4ebe54ac2cc1ab3e4b0a049e5
  ${platform_specific_patch}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ${platform_specific_build_prefix} $(MAKE)
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND $(MAKE) PREFIX=${UNIX_INSTALL_PREFIX}${install_suffix} install
  ${CLEANUP_COMMAND}
)
