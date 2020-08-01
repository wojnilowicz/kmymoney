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

set (glib_libs "-L${UNIX_INSTALL_PREFIX}/lib -lintl -lglib-2.0")
set (glib_cflags "-I${UNIX_INSTALL_PREFIX}/include/glib-2.0 -I${UNIX_INSTALL_PREFIX}/lib/glib-2.0/include -mms-bitfields")

ExternalProject_Add(
  ext_pkgconfig
  ${PROJECT_GLOBAL_PROFILE}
  URL https://pkg-config.freedesktop.org/releases/pkg-config-0.29.2.tar.gz
  URL_MD5 f6e931e319531b736fadc017f470e68a

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    GLIB_LIBS=${glib_libs}
    GLIB_CFLAGS=${glib_cflags}

  ${CLEANUP_COMMAND}
  DEPENDS ext_glib
)
