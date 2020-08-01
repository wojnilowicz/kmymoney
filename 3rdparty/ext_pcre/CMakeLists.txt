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

# autotools outputs pkg-config file and cmake not
ExternalProject_Add(
  ext_pcre
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre-8.44.tar.gz
  URL_MD5 3bcd2441024d00009a5fee43f058987c

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-pcre8
    --disable-pcre16
    --disable-pcre32
    --disable-cpp
    --enable-jit
    --disable-pcregrep-jit
    --enable-utf # required by glib
    --enable-newline-is-any # required by glib
    --disable-bsr-anycrlf # required by glib
    --enable-stack-for-recursion # required by KJs
    --disable-pcregrep-libz
    --disable-pcregrep-libbz2
    --disable-pcretest-libedit
    --disable-pcretest-libreadline
    --disable-valgrind
    --disable-coverage

  ${CLEANUP_COMMAND}
)
