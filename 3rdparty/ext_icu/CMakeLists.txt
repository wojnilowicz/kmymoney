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

# --enable-icuio is required by mysql
# codecs selection is taken from here https://doc.qt.io/qt-5/qtextcodec.html
# ISO 2022 and ISCII are "Partially algorithmic converters" so they are not listed in filters.json
# For details, see https://github.com/unicode-org/icu/blob/master/icu4c/source/data/mappings/convrtrs.txt
# UCONFIG_NO_FORMATTING is required by QIcuTimeZonePrivate::displayName of QtCore
# Warning Apple patch has sed -i '' instead of sed -i

set(ICU_CPPFLAGS "-DUCONFIG_NO_TRANSLITERATION=1 -DUCONFIG_NO_REGULAR_EXPRESSIONS=1 -DUCONFIG_NO_BREAK_ITERATION=1")

if(MINGW)
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/append-correct-path-separator.diff
                                    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/make-smaller-data.diff)
elseif(APPLE)
  set(platform_specific_patch PATCH_COMMAND bash -c "find . -maxdepth 3 -type f -name BUILDRULES.py -exec sed -i '' \
    -e 's+-s {IN_DIR} -d {OUT_DIR} -c {INPUT_FILE_PLACEHOLDER}+--small -s {IN_DIR} -d {OUT_DIR} -c {INPUT_FILE_PLACEHOLDER}+g' \
    {} \\+")
else()
  set(platform_specific_patch PATCH_COMMAND bash -c "find . -maxdepth 3 -type f -name BUILDRULES.py -exec sed -i \
    -e 's+-s {IN_DIR} -d {OUT_DIR} -c {INPUT_FILE_PLACEHOLDER}+--small -s {IN_DIR} -d {OUT_DIR} -c {INPUT_FILE_PLACEHOLDER}+g' \
    {} \\+")
endif()

ExternalProject_Add(
  ext_icu
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/unicode-org/icu/releases/download/release-67-1/icu4c-67_1-src.tgz
  URL_MD5 c4d62b497cbd89ab2a9ca6b543e57b30

  ${platform_specific_patch}
  
  CONFIGURE_COMMAND export ICU_DATA_FILTER_FILE=${CMAKE_CURRENT_SOURCE_DIR}/filters.json &&
    <SOURCE_DIR>/source/configure  
    ${AT_GLOBAL_PROFILE}
    --disable-debug
    --disable-static
    --enable-release
    --disable-draft
    --enable-rpath # required by QtBase on macOS 
    --disable-extras
    --disable-icuio
    --disable-layoutex
    --disable-icu-config
    --enable-tools # required to build custom data
    --disable-tests
    --disable-samples
    --with-data-packaging=library
    CPPFLAGS=${ICU_CPPFLAGS}

  ${CLEANUP_COMMAND}
)

ExternalProject_Add_Step(
  ext_icu icudata
  COMMAND wget -nc https://github.com/unicode-org/icu/releases/download/release-67-1/icu4c-67_1-data.zip
  COMMAND rm -fr data
  COMMAND ${CMAKE_COMMAND} -E tar xf icu4c-67_1-data.zip
    
  DEPENDEES download
  DEPENDERS patch
  WORKING_DIRECTORY <SOURCE_DIR>/source
)