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
  ext_snoretoast
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/snoretoast/0.7.0/src/snoretoast-0.7.0.tar.xz
  URL_MD5 f62fddc434c791958edb1cb4ba78b2e5

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_EXAMPLES=OFF
             -DBUILD_STATIC_RUNTIME=OFF
  
  ${CLEANUP_COMMAND}
)
