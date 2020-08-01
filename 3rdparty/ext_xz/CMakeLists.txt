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
  ext_xz
  ${PROJECT_GLOBAL_PROFILE}
  URL https://tukaani.org/xz/xz-5.2.5.tar.xz
  URL_MD5 aa1621ec7013a19abab52a8aff04fe5b

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-lzmadec
    --disable-lzmainfo
    --disable-scripts
    --disable-doc
    --disable-nls
    --disable-xz
    --disable-xzdec
    --disable-debug

  ${CLEANUP_COMMAND}
)
