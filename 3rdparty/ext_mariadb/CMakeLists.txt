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
  set(INSTALL_LIBDIR -DINSTALL_LIBDIR=lib)
  set(INSTALL_PLUGINDIR -DINSTALL_PLUGINDIR=bin/mariadb)
else()
  set(INSTALL_LIBDIR -DINSTALL_LIBDIR=lib)
  set(INSTALL_PLUGINDIR -DINSTALL_PLUGINDIR=plugins/mariadb)
endif()

# PVIO plugins can be set only STATIC or DYNAMIC but not OFF

ExternalProject_Add(
  ext_mariadb
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.mariadb.com/Connectors/c/connector-c-3.1.8/mariadb-connector-c-3.1.8-src.tar.gz
  URL_MD5 0beaee9a77f78a0bf37a1d91999643c6

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/mariadb.diff

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX} # to ensure that OpenSSL in install prefix, and not in Windows prefix is found
             -DWITH_EXTERNAL_ZLIB=ON
             -DWITH_MSI=OFF
             -DWITH_RTC=OFF
             -DWITH_CURL=OFF
             -DAUTH_GSSAPI_PLUGIN_TYPE=OFF
             -DPLUGIN_AUTH_GSSAPI_CLIENT=OFF
             -DPLUGIN_SHA256_PASSWORD=OFF
             -DPLUGIN_CACHING_SHA2_PASSWORD=OFF # MSYS2 lacks bcrypt.h
             -DPLUGIN_CLIENT_ED25519=OFF # MSYS2 lacks bcrypt.h
             -DWITH_SSL=OPENSSL
             -DWITH_MYSQLCOMPAT=ON
             -DWITH_UNIT_TESTS=OFF
             ${INSTALL_LIBDIR}
             ${INSTALL_PLUGINDIR}

  ${CLEANUP_COMMAND}
  DEPENDS ext_openssl
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_mariadb fixLibrariesOnMsys
    COMMAND mv -f ${CMAKE_INSTALL_PREFIX}/lib/libmariadb.dll ${CMAKE_INSTALL_PREFIX}/bin/libmariadb.dll
    COMMAND mv -f ${CMAKE_INSTALL_PREFIX}/lib/liblibmariadb.dll.a ${CMAKE_INSTALL_PREFIX}/lib/libmariadb.dll.a
    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
  )
endif()
