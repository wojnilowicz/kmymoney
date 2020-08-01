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
  ext_ical
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/libical/libical/archive/v3.0.8.zip
  URL_MD5 ef009c386db4f5f4634f4b4c6e9f4cee

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DUSE_BUILTIN_TZDATA=ON
             -DICAL_UNIX_NEWLINE=OFF
             -DICAL_GLIB=OFF
             -DSHARED_ONLY=ON
  ${CLEANUP_COMMAND}
)
