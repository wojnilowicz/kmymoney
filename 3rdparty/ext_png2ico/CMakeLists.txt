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
  ext_png2ico
  ${PROJECT_GLOBAL_PROFILE}
  URL http://www.winterdrache.de/freeware/png2ico/data/png2ico-win-2002-12-08.zip
  URL_MD5 5be668b436990255b75cce4ce03be282

  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND bash -c "cp -f <SOURCE_DIR>/png2ico.exe ${CMAKE_INSTALL_PREFIX}/bin/png2ico.exe"
  ${CLEANUP_COMMAND}
)
