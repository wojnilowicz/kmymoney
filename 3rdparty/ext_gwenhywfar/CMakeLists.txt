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

# Without CPPFLAGS "error: libintl.h: No such file or directory" on MinGW
# Without LDFLAGS "Undefined symbols for architecture x86_64: _libintl_bind_textdomain_codeset" on MacOS

if(MINGW)
  set(platform_specific_patch ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/help-setting-up-qt5-on-mingw.diff)
  set(platform_specific_options
    --with-plugin-searchdir=bin/gwenhywfar
    --with-data-searchdir=bin/data/gwenhywfar
    --with-sysdata-searchdir=bin/data
    --with-locale-searchdir=bin/data/locale
  )
elseif(APPLE)
  set(platform_specific_patch ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/help-setting-up-qt5-on-macos.diff)
  set(platform_specific_options
    --with-plugin-searchdir=PlugIns/gwenhywfar
    --with-data-searchdir=Resources/gwenhywfar
    --with-sysdata-searchdir=Resources
    --with-locale-searchdir=Resources/locale
  )
else()
  set(platform_specific_patch "")
  set(platform_specific_options
    --with-plugin-searchdir=plugins/gwenhywfar
    --with-data-searchdir=share/gwenhywfar
    --with-sysdata-searchdir=share
    --with-locale-searchdir=share/locale
  )
endif()

ExternalProject_Add(
  ext_gwenhywfar
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/319/gwenhywfar-5.3.0.tar.gz
  URL_MD5 96075362cff861fbf6f4326715e91f69

  PATCH_COMMAND ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --build=${AT_SYSTEM_TYPE}
    $<$<NOT:$<PLATFORM_ID:Linux>>:--host=${AT_SYSTEM_TYPE}>
    --disable-static
    --enable-shared
    --enable-silent-rules
    $<$<PLATFORM_ID:Windows>:--disable-binreloc> # otherwise "undefined reference to `S_ISLNK'" on MS WIndows
    --enable-local-install
    --disable-debug
    --disable-warnings
    --with-guis=qt5
    --disable-ssl
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gnutls ext_gcrypt
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_gwenhywfar correctcmake
    COMMAND bash -c "find ./gwen* -type f -name gwen*config.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    -e 's+lib/libgwengui-cpp.so+lib/libgwengui-cpp.dll.a+g' \
    -e 's+lib/libgwengui-qt5.so+lib/libgwengui-qt5.dll.a+g' \
    -e 's+lib/libgwenhywfar.so.\\([0-9]*\\)+lib/libgwenhywfar.dll.a+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
elseif(APPLE)
  ExternalProject_Add_Step(
    ext_gwenhywfar correctcmake
    COMMAND bash -c "find ./gwen* -type f -name gwen*config.cmake -exec sed -i '' \
    -e 's+libgwengui-cpp.so+libgwengui-cpp.dylib+g' \
    -e 's+libgwengui-qt5.so+libgwengui-qt5.dylib+g' \
    -e 's+libgwenhywfar.so.\\([0-9]*\\)+libgwenhywfar.\\1.dylib+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )

 ExternalProject_Add_Step(
   ext_gwenhywfar correctxmlmerge
   COMMAND install_name_tool -change "@rpath/libintl.8.dylib" "${CMAKE_INSTALL_PREFIX}/lib/libintl.8.dylib" xmlmerge
   
   DEPENDEES install
   WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
 )
endif()
