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

# silence a lot of warnings on MacOS
ExternalProject_Add(
  ext_kdiagram
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/kdiagram/2.7.0/kdiagram-2.7.0.tar.xz
  URL_MD5 787b95dc3e5fad6c767d7758d6a0c5e5
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix-isnan-on-mingw.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix-qt5_wrap_cpp.diff

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_TESTING=FALSE
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtsvg
)
