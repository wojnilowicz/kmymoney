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

# Setup variables needed to help everything find what we build
export LD_LIBRARY_PATH=\
$DEPS_INSTALL_PREFIX/lib:\
$DEPS_INSTALL_PREFIX/openssl/lib:\
/usr/local/lib:\
${LD_LIBRARY_PATH:-}

export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH

export PATH=\
$DEPS_INSTALL_PREFIX/bin:\
$DEPS_INSTALL_PREFIX/openssl/bin:\
/usr/local/opt/qt5/bin:\
/usr/local/opt/bison/bin:\
/usr/local/bin:\
/usr/bin:\
${PATH:-}

export PKG_CONFIG_PATH=\
$DEPS_INSTALL_PREFIX/share/pkgconfig:\
$DEPS_INSTALL_PREFIX/lib/pkgconfig:\
$DEPS_INSTALL_PREFIX/openssl/lib/pkgconfig:\
/usr/local/opt/qt/lib/pkgconfig:\
/usr/local/lib/pkgconfig:\
/usr/lib/pkgconfig:\
${PKG_CONFIG_PATH:-}

export CMAKE_PREFIX_PATH=\
$DEPS_INSTALL_PREFIX:\
$DEPS_INSTALL_PREFIX/openssl:\
/usr/local/opt/qt5:\
/usr/local/opt/bison:\
/usr/local:\
/usr:\
${CMAKE_PREFIX_PATH:-}

# Get available processors count
export CPU_COUNT=`sysctl -n hw.physicalcpu`

if [ $BUILD_TYPE == "deps" ];then
  sh $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-deps.sh
elif [ $BUILD_TYPE == "kmymoney" ];then
  sh $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-kmymoney.sh
elif [ $BUILD_TYPE == "image" ];then
  sh $KMYMONEY_SOURCES/packaging/osx/dmgimage/build-image.sh
fi