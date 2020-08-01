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

# xmlsec1 doesn't use exsltCryptoRegister from xslt so no gcrypt needed
# --with-plugins requires --with-modules in libxml2
ExternalProject_Add(
  ext_xslt
  ${PROJECT_GLOBAL_PROFILE}
  URL http://xmlsoft.org/sources/libxslt-1.1.34.tar.gz
  URL_MD5 db8765c8d076f1b6caafd9f2542a304a

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --without-python
    --without-crypto
    --without-debug
    --without-debugger
    --without-profiler

  ${CLEANUP_COMMAND}

  DEPENDS ext_xml
)
