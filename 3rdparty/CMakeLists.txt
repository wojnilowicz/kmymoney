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

project(kmymoney-and-all-its-deps)

#
# Build all dependencies for KMyMoney and finally KMyMoney itself.
# Parameters: EXT_DOWNLOAD_DIR place to download all packages
#             CMAKE_INSTALL_PREFIX place to install everything to
#
# Example usage: cmake $KMYMONEY_SOURCES/3rdparty -DEXT_DOWNLOAD_DIR=/dev2/d -DCMAKE_INSTALL_PREFIX=/dev2/i


cmake_minimum_required(VERSION 3.1.3)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Compiling in the source directory is not supported. Use for example 'mkdir build; cd build; cmake ..'.")
endif()

# Get available processors count for parallel jobs
include(ProcessorCount)
ProcessorCount(CMAKE_CPU_COUNT)
message(STATUS "Available CPUs: ${CMAKE_CPU_COUNT}")

# Some terminal output silencers
set(AT_CONFIGURE_ARGS --silent)
set(CMAKE_VERBOSE_MAKEFILE FALSE)

# For performance reason, cache install and don't cache build and source directory on Travis CI, which defines CI=true
if($ENV{CI})
  message(STATUS "Running in CI mode. Build and source directory will be deleted after first use")
  # <SOURCE_DIR> could be equal tp <BINARY_DIR> so make sure that there is always something to delete
  # ExternalProject_Add doesn't support wildcards
  # Unix Makefiles generator, contrary to Ninja generator, sometimes wants to enter either <SOURCE_DIR> or <BINARY_DIR>...
  # ...so make sure that there is always something to enter to
  # use TEST_COMMAND instead of COMMAND to ensure that it will be executed after install step
  set(CLEANUP_COMMAND TEST_COMMAND rm -fr <SOURCE_DIR> && mkdir <SOURCE_DIR> && rm -fr <BINARY_DIR> && mkdir <BINARY_DIR>)
else()
  set(CLEANUP_COMMAND TEST_COMMAND "")
endif()

if($ENV{TRAVIS_OS_NAME} AND $ENV{TRAVIS_OS_NAME} STREQUAL "windows")
  message(STATUS "Setting BUILD_IN_SOURCE to 1 due to Travis treating build directory as source directory for CMake projects.")
  set(BUILD_IN_SOURCE BUILD_IN_SOURCE 0)
else()
  set(BUILD_IN_SOURCE BUILD_IN_SOURCE 0)
endif()

# For setting host and build system, which can limit cpu features used
if (APPLE)
  set(AT_SYSTEM_TYPE x86_64-apple-darwin${DARWIN_KERNEL_VERSION})
elseif(MINGW)
  set(AT_SYSTEM_TYPE x86_64-w64-mingw32)
elseif(UNIX)
  set(AT_SYSTEM_TYPE x86_64-linux-gnu)
endif()

# CMake on MS Windows uses paths like "C:/path", so convert them to "/c/path", to avoid problems
set(UNIX_INSTALL_PREFIX "$<SHELL_PATH:${CMAKE_INSTALL_PREFIX}>")

# Prepare CPPFLAGS and LDFLAGS for finding hard to find dependencies
set(AT_CPPFLAGS "-I${UNIX_INSTALL_PREFIX}/include")
set(AT_LDFLAGS "-L${UNIX_INSTALL_PREFIX}/lib")

# Convenience variables for commonly used settings
set(CMAKE_GLOBAL_PROFILE
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    $<$<PLATFORM_ID:Linux>:-DCMAKE_INSTALL_LIBDIR=lib>
    $<$<PLATFORM_ID:Darwin>:-DCMAKE_MACOSX_RPATH=ON>
    $<$<PLATFORM_ID:Darwin>:-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}>
)

set(AT_GLOBAL_PROFILE
    --prefix=${UNIX_INSTALL_PREFIX}
    --build=${AT_SYSTEM_TYPE}
    --host=${AT_SYSTEM_TYPE}
    --disable-static
    --enable-shared
    --enable-silent-rules
    $<$<BOOL:$ENV{CI}>:--disable-dependency-tracking>
)

set(PROJECT_GLOBAL_PROFILE
    DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
    DOWNLOAD_NO_PROGRESS 1
)

set(PATCH_EXECUTABLE patch)

# Add ExternalProject_Add functionality to CMake
include(ExternalProject)

# Dependency ordered list of external projects
add_subdirectory(ext_zlib)
add_subdirectory(ext_iconv)
if (APPLE)
  add_subdirectory(ext_bison) # on MinGW bison doesn't build, so use MSYS2 one
  add_subdirectory(ext_flex)
endif()
add_subdirectory(ext_pcre)
add_subdirectory(ext_pcre2)
if(UNIX AND NOT (APPLE OR MINGW))
  add_subdirectory(ext_glib)
endif()
# add_subdirectory(ext_pkgconfig)
add_subdirectory(ext_expat)
add_subdirectory(ext_dbus)
add_subdirectory(ext_xz)
add_subdirectory(ext_icu)
add_subdirectory(ext_xml)
add_subdirectory(ext_gettext)
add_subdirectory(ext_png)
add_subdirectory(ext_gif)
add_subdirectory(ext_nasm)
add_subdirectory(ext_jpeg)
# add_subdirectory(ext_boost)
add_subdirectory(ext_gpgerror)
add_subdirectory(ext_gcrypt)
add_subdirectory(ext_xslt)
# add_subdirectory(ext_fontconfig)
# add_subdirectory(ext_intltool)
# add_subdirectory(ext_sharedmimeinfo)
add_subdirectory(ext_gmp)
add_subdirectory(ext_assuan2)
add_subdirectory(ext_nettle)
add_subdirectory(ext_gnutls)
# add_subdirectory(ext_freetype)
# add_subdirectory(ext_harfbuzz)
add_subdirectory(ext_sqlite)
add_subdirectory(ext_openssl)
add_subdirectory(ext_mariadb)
add_subdirectory(ext_tcl)
add_subdirectory(ext_postgresql)
if(UNIX AND NOT (APPLE OR MINGW))
  add_subdirectory(ext_xcb)
  add_subdirectory(ext_cups)
endif()
add_subdirectory(ext_qt)
add_subdirectory(ext_kdewin)
add_subdirectory(ext_gperf)
add_subdirectory(ext_gpgme)
if(MINGW)
  add_subdirectory(ext_snoretoast)
endif()
add_subdirectory(ext_frameworks)
add_subdirectory(ext_grantlee)
add_subdirectory(ext_applications)
# add_subdirectory(ext_alkimia)
add_subdirectory(ext_kdiagram)
add_subdirectory(ext_gwenhywfar)
add_subdirectory(ext_xmlsec1)
add_subdirectory(ext_aqbanking)
add_subdirectory(ext_sqlcipher)
add_subdirectory(ext_opensp)
add_subdirectory(ext_ofx)
add_subdirectory(ext_ical)
if(UNIX AND NOT (APPLE OR MINGW))
  add_subdirectory(ext_patchelf)
endif()
if(MINGW)
  add_subdirectory(ext_png2ico)
endif()
