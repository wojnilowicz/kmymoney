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
  ext_gpgme
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.13.1.tar.bz2
  URL_MD5 198f0a908ec3cd8f0ce9a4f3a4489645

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gpgme-1.1.11-20180620.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --enable-languages=cpp
    --disable-gpgconf-test
    --disable-gpg-test
    --disable-gpg-test
    --disable-gpgsm-test
    --disable-g13-test

  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2 ext_gpgerror
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_gpgme correctcmake
    COMMAND bash -c "find . -type f -name GpgmeppConfig.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )

  ExternalProject_Add_Step(
    ext_gpgme installfiles
    COMMAND cp -fv ./src/.libs/gpgme-w32spawn.exe ${UNIX_INSTALL_PREFIX}/bin
  
    DEPENDEES install
    WORKING_DIRECTORY <BINARY_DIR>
  )
endif()
