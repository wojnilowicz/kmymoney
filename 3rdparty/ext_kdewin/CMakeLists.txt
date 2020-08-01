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

ExternalProject_Add(
  ext_kdewin
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/KDE/kdewin/archive/v0.6.4.zip
  URL_MD5 10b93a002192fd73d6dfc774dd2311ca

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/kdewin.diff
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_qtbase
)
