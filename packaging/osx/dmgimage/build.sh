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

#!/bin/bash
#
# Wrapper for all build scripts, which sets many shared variables.

# Halt on errors and be verbose about what we are doing
set -eux

# Read in our parameters
BUILD_TYPE=$1
export WORKSPACE_PATH=$2
export KMYMONEY_SOURCES=$3

# We want to use $prefix/deps/usr/ for all our dependencies
if [ $BUILD_TYPE == "deps" ] || [ $BUILD_TYPE == "kmymoney" ] || [ $BUILD_TYPE == "image" ];then
  export CMAKE_INSTALL_PREFIX=$WORKSPACE_PATH/$BUILD_TYPE-install
  export CMAKE_BUILD_PREFIX=$WORKSPACE_PATH/$BUILD_TYPE-build
else
  echo "ERROR: no valid build type."
  exit 1
fi

export DEPS_INSTALL_PREFIX=$WORKSPACE_PATH/deps-install
export KMYMONEY_INSTALL_PREFIX=$WORKSPACE_PATH/kmymoney-install
export DOWNLOADS_DIR=$WORKSPACE_PATH/downloads

# A kmymoney build layout looks like this:
# kmymoney/ -- the source directory
# kmymoney-build/ -- build directory for kmymoney itself
# kmymoney-install/ -- install directory for kmymoney and the dependencies
# deps-build/ -- build directory for the dependencies
# deps-install/ -- the location for the built dependencies

# Make sure our build directory exists
mkdir -p $CMAKE_INSTALL_PREFIX
mkdir -p $CMAKE_BUILD_PREFIX
mkdir -p $DEPS_INSTALL_PREFIX
mkdir -p $DOWNLOADS_DIR

# qjsonparser, used to add metadata to the plugins needs to work in a en_US.UTF-8 environment.
# That's not always the case, so make sure it is
export LC_ALL=en_US.UTF-8
export LANG=en_us.UTF-8

export PATH=\
$DEPS_INSTALL_PREFIX/bin:\
${PATH:-}

export PKG_CONFIG_PATH=\
$DEPS_INSTALL_PREFIX/lib/pkgconfig:\
$DEPS_INSTALL_PREFIX/colisionlibs/lib/pkgconfig:\
${PKG_CONFIG_PATH:-}

# Get available processors count
export CPU_COUNT=`sysctl -n hw.physicalcpu`

if [ $BUILD_TYPE == "deps" ];then
  $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-deps.sh
elif [ $BUILD_TYPE == "kmymoney" ];then
  $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-kmymoney.sh
elif [ $BUILD_TYPE == "image" ];then
  $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-image.sh
fi
