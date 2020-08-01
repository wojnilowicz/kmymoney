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
  ext_pcre2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://ftp.pcre.org/pub/pcre/pcre2-10.34.tar.gz
  URL_MD5 e3e15cca49557a9c07a21dde2da05ea5

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-pcre2-8
    --enable-pcre2-16
    --disable-pcre2-32
    --disable-debug
    --enable-jit
    --disable-pcre2grep-jit
    --disable-pcre2grep-callout
    --disable-pcre2grep-callout-fork
    --enable-unicode
    --enable-newline-is-lf
    --disable-bsr-anycrlf
    --enable-stack-for-recursion
    --disable-pcre2grep-libz
    --disable-pcre2grep-libbz2
    --disable-pcre2test-libedit
    --disable-pcre2test-libreadline
    --disable-valgrind
    --disable-coverage

  ${CLEANUP_COMMAND}
)
