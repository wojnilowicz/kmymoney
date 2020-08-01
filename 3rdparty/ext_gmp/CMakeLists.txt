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

# without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs
ExternalProject_Add(
  ext_gmp
  ${PROJECT_GLOBAL_PROFILE}
  URL https://gmplib.org/download/gmp/gmp-6.2.0.tar.xz
  URL_MD5 a325e3f09e6d91e62101e59f9bda3ec1

  CONFIGURE_COMMAND ./configure
    ${AT_GLOBAL_PROFILE}
    --enable-cxx # required by KMyMoney
    --without-readline
  BUILD_IN_SOURCE 1 # MinGW needs it
  ${CLEANUP_COMMAND}
)
