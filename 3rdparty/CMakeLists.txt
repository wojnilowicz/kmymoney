project (kmymoney-and-all-its-deps)

#
# Build all dependencies for KMyMoney and finally KMyMoney itself.
# Parameters: EXT_DOWNLOAD_DIR place to download all packages
#             CMAKE_INSTALL_PREFIX place to install everything to
#
# Example usage: cmake ..\kmymoneydep -DEXT_DOWNLOAD_DIR=/dev2/d -DCMAKE_INSTALL_PREFIX=/dev2/i


cmake_minimum_required(VERSION 3.0.2)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Compiling in the source directory is not supported. Use for example 'mkdir build; cd build; cmake ..'.")
endif()

# Get available processors count
include(ProcessorCount)
ProcessorCount(CMAKE_CPU_COUNT)
message( STATUS "Available CPUs: ${CMAKE_CPU_COUNT}")

# Check if make executable for autotools projects is available
find_program(MAKE_EXECUTABLE NAMES gmake nmake make )
if(NOT MAKE_EXECUTABLE)
  message(FATAL_ERROR "No gmake, nmake, mingw32-make or make available.")
endif()

set (MAKE_ARGS --silent -j${CMAKE_CPU_COUNT})
# set (CLEANUP_COMMAND TEST_COMMAND rm -fr <SOURCE_DIR> && rm -fr <BINARY_DIR>)
set (CLEANUP_COMMAND "")

# Tools must be obtained to work with:
include(ExternalProject)

message( STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX_NATIVE)
file(TO_CMAKE_PATH ${CMAKE_INSTALL_PREFIX} CMAKE_INSTALL_PREFIX_CMAKE)

set (UNIX_INSTALL_PREFIX "$<SHELL_PATH:${CMAKE_INSTALL_PREFIX}>")
message( STATUS "UNIX_INSTALL_PREFIX ${UNIX_INSTALL_PREFIX}")

set(CMAKE_GLOBAL_PROFILE
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  -DBUILD_TESTING=FALSE
)

if (APPLE)
  list(APPEND CMAKE_GLOBAL_PROFILE
    -DAPPLE_SUPPRESS_X11_WARNING=ON
    -DCMAKE_MACOSX_RPATH=ON
  )
  # without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs
  set(AT_SYSTEM_TYPE x86_64-apple-darwin${DARWIN_KERNEL_VERSION})
elseif(MINGW)
  list(APPEND CMAKE_GLOBAL_PROFILE
    -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
  )
  # without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs
  set(AT_SYSTEM_TYPE x86_64-w64-mingw32)
elseif(UNIX)
  # without --host=x86_64 gmp builds with optimizations for skylake and that prevents running KMyMoney on pre-skylake CPUs
  set(AT_SYSTEM_TYPE x86_64-linux-gnu)
endif ()

set(AT_SHARED_ONLY --disable-static --enable-shared)
set(AT_CPP_FLAGS "-I${UNIX_INSTALL_PREFIX}/include -I${UNIX_INSTALL_PREFIX}/openssl/include")
set(AT_LD_FLAGS "-L${UNIX_INSTALL_PREFIX}/lib -L${UNIX_INSTALL_PREFIX}/openssl/lib")
set(AT_LESS_PROCESSING --enable-silent-rules --disable-dependency-tracking)

set(PATCH_COMMAND patch)

# this list must be dependency-ordered
add_subdirectory( ext_zlib )
add_subdirectory( ext_iconv )
add_subdirectory( ext_bison )
add_subdirectory( ext_flex )
add_subdirectory( ext_glib )
add_subdirectory( ext_pkgconfig )
add_subdirectory( ext_xz )
add_subdirectory( ext_icu )
add_subdirectory( ext_ncurses )
add_subdirectory( ext_readline )
add_subdirectory( ext_xml )
add_subdirectory( ext_gettext )
add_subdirectory( ext_png )
add_subdirectory( ext_nasm )
add_subdirectory( ext_jpeg )
add_subdirectory( ext_boost )
add_subdirectory( ext_gpgerror )
add_subdirectory( ext_gcrypt )
add_subdirectory( ext_xslt )
add_subdirectory( ext_fontconfig )
add_subdirectory( ext_intltool )
add_subdirectory( ext_sharedmimeinfo )
add_subdirectory( ext_gmp )
add_subdirectory( ext_assuan2 )
add_subdirectory( ext_nettle )
add_subdirectory( ext_tasn1 )
add_subdirectory( ext_unistring )
add_subdirectory( ext_gnutls )
add_subdirectory( ext_pcre2 )
add_subdirectory( ext_freetype )
add_subdirectory( ext_harfbuzz )
add_subdirectory( ext_sqlite )
add_subdirectory( ext_openssl )
add_subdirectory( ext_mysql )
add_subdirectory( ext_tcl )
add_subdirectory( ext_postgresql )
add_subdirectory( ext_qt )
add_subdirectory( ext_kdewin )
add_subdirectory( ext_gperf )
add_subdirectory( ext_gpgme )
add_subdirectory( ext_frameworks )
add_subdirectory( ext_grantlee )
add_subdirectory( ext_applications )
add_subdirectory( ext_alkimia )
add_subdirectory( ext_kdiagram )
add_subdirectory( ext_ktoblzcheck )
add_subdirectory( ext_gwenhywfar )
add_subdirectory( ext_xmlsec1 )
add_subdirectory( ext_aqbanking )
add_subdirectory( ext_sqlcipher )
add_subdirectory( ext_opensp )
add_subdirectory( ext_ofx )
add_subdirectory( ext_ical )
add_subdirectory( ext_patchelf )
add_subdirectory( ext_png2ico )
