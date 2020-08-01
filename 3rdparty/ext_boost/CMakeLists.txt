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
  ext_boost
  ${PROJECT_GLOBAL_PROFILE}
  URL https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
  URL_MD5 7fbd1890f571051f2a209681d57d486a

  CONFIGURE_COMMAND <SOURCE_DIR>/bootstrap.sh --prefix=${CMAKE_INSTALL_PREFIX} --with-libraries=system
  BUILD_COMMAND ""
  INSTALL_COMMAND <SOURCE_DIR>/b2 -d0 install
  ${CLEANUP_COMMAND}

  BUILD_IN_SOURCE 1
)
