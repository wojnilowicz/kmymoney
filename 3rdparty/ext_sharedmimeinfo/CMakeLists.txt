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
  ext_sharedmimeinfo
  ${PROJECT_GLOBAL_PROFILE}
  URL https://people.freedesktop.org/~hadess/shared-mime-info-1.10.tar.xz
  URL_MD5 418c2ced9dc4dd5ca8b06a755e6d64e9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-update-mimedb

  ${CLEANUP_COMMAND}

  DEPENDS ext_intltool
)
