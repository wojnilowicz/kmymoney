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

if(MINGW)
  set(platform_specific_dir win)
else()
  set(platform_specific_dir unix)
endif()

ExternalProject_Add(
  ext_tcl
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/tcltk/tcl/archive/core-8-6-10.tar.gz
  URL_MD5 b9578606270ac55d2cbabf35515724bc

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/do-not-use-precompiled-zlib.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/${platform_specific_dir}/configure
    ${AT_GLOBAL_PROFILE}
    --enable-threads
    --enable-64bit

  ${CLEANUP_COMMAND}
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_tcl correcttclsh
    COMMAND cp -f tclsh86.exe ${UNIX_INSTALL_PREFIX}/bin/tclsh.exe
    COMMAND mkdir -p ${UNIX_INSTALL_PREFIX}/lib/tcl8.6
    COMMAND cp -f <SOURCE_DIR>/library/init.tcl ${UNIX_INSTALL_PREFIX}/lib/tcl8.6
    COMMAND sleep 5 # tzdata fails to build on MingGW, so try to make a little pause

    DEPENDEES build
    DEPENDERS install
    WORKING_DIRECTORY <BINARY_DIR>
  )
endif()

