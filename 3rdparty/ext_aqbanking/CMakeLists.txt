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

# Without CPPFLAGS and LDFLAGS "error: GMP is required"
# "make -j1" prevents "cannot find the library libaqhbci.la or unhandled argument libaqhbci.la"
# "make -j1 install" is needed on Linux, otherwise it fails to find libaqhbci.so
# xslt is required by aqebics plugin

if(MINGW)
  set(platform_specific_options
    --with-plugin-searchdir=bin/aqbanking
    --with-data-searchdir=bin/data # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=bin/data
    --with-locale-searchdir=bin/data/locale
  )
elseif(APPLE)
  set(platform_specific_options
    --with-plugin-searchdir=PlugIns/aqbanking
    --with-data-searchdir=Resources # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=Resources
    --with-locale-searchdir=Resources/locale
  )
else()
  set(platform_specific_options
    --with-plugin-searchdir=plugins/aqbanking
    --with-data-searchdir=share # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=share
    --with-locale-searchdir=share/locale
  )
endif()

ExternalProject_Add(
  ext_aqbanking
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/283/aqbanking-6.1.4.tar.gz
  URL_MD5 0d18c13d7f6a4e31689422549847d750
  
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-gwenhywfar-test
    --enable-local-install
    --enable-release
    --disable-debug
    --disable-warnings
    --disable-cli
    --disable-tutorials
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  BUILD_COMMAND $(MAKE) -j1
  INSTALL_COMMAND $(MAKE) -j1 install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gwenhywfar ext_xmlsec1 ext_gmp
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_aqbanking correctcmake
    COMMAND bash -c "find . -type f -name aqbanking-config.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    -e 's+lib/libaqbanking.so+lib/libaqbanking.dll.a+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
elseif(APPLE)
  ExternalProject_Add_Step(
    ext_aqbanking correctcmake
    COMMAND bash -c "find . -type f -name aqbanking-config.cmake -exec sed -i '' \
    -e 's+lib/libaqbanking.so.\\([0-9]\\{2\\}\\)+lib/libaqbanking.\\1.dylib+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
endif()