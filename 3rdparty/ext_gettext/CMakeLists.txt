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

# --disable-nls won't build libintl
# ki18n needs libintl library and msgmerge executable

if (APPLE OR MINGW)
set(platform_specific_options "CPPFLAGS=${AT_CPPFLAGS} -I${UNIX_INSTALL_PREFIX}/include/libxml2"
                                "LDFLAGS=${AT_LDFLAGS}")
else()
# on Linux error is:
# ext_gettext-build/gettext-tools/src/./textstyle.h': File exists
# Makefile:2462: recipe for target 'install-nobase_includeHEADERS' failed
# make[9]: *** [install-nobase_includeHEADERS] Error 1
# Makefile:3955: recipe for target 'textstyle/stdbool.h' failed
set(platform_specific_options "")
endif()

ExternalProject_Add(
  ext_gettext
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.20.2.tar.xz
  URL_MD5 0cf5f68338d5d941bbf9ac93b847310f

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-runtime-build.diff
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-java
    --disable-native-java
    --disable-curses
    --disable-csharp
    --disable-libasprintf
    --disable-openmp
    --disable-acl
    --without-git
    --without-cvs
    --without-emacs
    --enable-c++ # is required by ki18n
    --enable-relocatable
    --without-included-libxml
    --with-libxml2-prefix=${UNIX_INSTALL_PREFIX}
    ${platform_specific_options}

  ${CLEANUP_COMMAND}
  COMMAND rm -fr ${CMAKE_INSTALL_PREFIX}/share/doc/gettext
  DEPENDS ext_xml
)