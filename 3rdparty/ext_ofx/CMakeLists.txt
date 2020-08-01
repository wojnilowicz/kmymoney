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

set(ofx_openSPIncludeDir "${UNIX_INSTALL_PREFIX}/include/OpenSP")
set(ofx_openSPLibDir "${UNIX_INSTALL_PREFIX}/lib")

ExternalProject_Add(
  ext_ofx
  ${PROJECT_GLOBAL_PROFILE}
  URL http://downloads.sourceforge.net/project/libofx/libofx/libofx-0.9.15.tar.gz
  URL_MD5 2863156ca18f73deb75501a4fc0d6937

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-doxygen
    --disable-html-docs
    --with-opensp-includes=${ofx_openSPIncludeDir}
    --with-opensp-libs=${ofx_openSPLibDir}

  ${CLEANUP_COMMAND}

  DEPENDS ext_opensp
)
