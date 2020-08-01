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

# TextDocument is required by kpimtextedit

ExternalProject_Add(
  ext_grantlee
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/steveire/grantlee/archive/v5.2.0.tar.gz
  URL_MD5 da76560384bb47ddd4e0dce07c0bdd8e

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_TEMPLATES=OFF
             -DBUILD_TEXTDOCUMENT=ON
             -DBUILD_MAIN_PLUGINS=OFF
             -DBUILD_I18N_PLUGIN=OFF
             -DBUILD_SCRIPT_PLUGIN=OFF
             -DBUILD_TESTS=OFF
  ${CLEANUP_COMMAND}
)