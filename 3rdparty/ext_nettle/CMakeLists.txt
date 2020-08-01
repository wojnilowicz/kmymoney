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
  ext_nettle
  ${PROJECT_GLOBAL_PROFILE}
  URL http://www.lysator.liu.se/~nisse/archive/nettle-3.6.tar.gz
  URL_MD5 c45ee24ed7361dcda152a035d396fe8a

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-public-key
    --disable-gcov
    --disable-documentation
    --disable-mini-gmp
    --disable-openssl
    --with-lib-path=${UNIX_INSTALL_PREFIX}/lib
    --with-include-path=${UNIX_INSTALL_PREFIX}/include

  ${CLEANUP_COMMAND}

  DEPENDS ext_gmp
)
