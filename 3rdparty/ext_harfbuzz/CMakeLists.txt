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

# this library isn't shipped with AppImage as per
# https://github.com/AppImage/AppImages/blob/master/excludelist#L126

ExternalProject_Add(
  ext_harfbuzz
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.freedesktop.org/software/harfbuzz/release/harfbuzz-2.0.0.tar.bz2
  URL_MD5 6d1373dba3e8540c448c6a562f2e0a00

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --with-icu=yes
    --disable-gtk-doc-html

  ${CLEANUP_COMMAND}

  DEPENDS ext_icu
)
