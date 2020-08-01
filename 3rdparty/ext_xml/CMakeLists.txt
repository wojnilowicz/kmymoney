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

if (MINGW)
  set(platform_specific_options "CPPFLAGS=${AT_CPPFLAGS}") # somehow --prefix is not added as include search path and results in "fatal error: unicode/ucnv.h: No such file or directory"
else()
  set(platform_specific_options "")
endif()

ExternalProject_Add(
  ext_xml
  ${PROJECT_GLOBAL_PROFILE}
  URL http://xmlsoft.org/sources/libxml2-2.9.10.tar.gz
  URL_MD5 10942a1dc23137a8aa07f0639cbfece5

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/xml.diff # prevents "fatal error: unicode/ucnv.h: No such file or directory" in libraries that link to libxml2
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/add_guards_to_valid.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --with-c14n # required by xmlsec1
    --without-catalog # required to be disabled by QWebEngine
    --without-debug
    --without-docbook
    --without-ftp
    --with-html # required by xslt
    --without-iconv
    --without-http
    --with-icu
    --without-legacy
    --with-output # required by xmlsec1
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-pattern> # schemas needs it
    --without-push
    --without-python
    --with-reader # untested
    --without-regexps
    --with-sax1 # required by xmlsec1
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-schemas>
    --without-schematron
    --with-tree # required by gettext
    --without-valid
    --with-writer # required by gettext
    $<$<NOT:$<PLATFORM_ID:Darwin>>:--without-xinclude>
    --with-xpath # required by gettext
    --with-xptr # required by xmlsec1
    --without-modules
    ${platform_specific_options}

  ${CLEANUP_COMMAND}

  DEPENDS ext_xz ext_icu
)
