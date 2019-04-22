#!/bin/bash
#
# Wrapper for all build scripts, which sets many shared variables.

# Halt on errors and be verbose about what we are doing
set -eux

# Read in our parameters
BUILD_TYPE=$1
export WORKSPACE_PATH=$2
export KMYMONEY_SOURCES=$3
REMAINING_TIME=$4
REMAINING_TIME="${REMAINING_TIME:-0}"

# We want to use $prefix/deps/usr/ for all our dependencies
if [ $BUILD_TYPE == "deps" ] || [ $BUILD_TYPE == "kmymoney" ] || [ $BUILD_TYPE == "image" ];then
  export CMAKE_BUILD_PREFIX=$WORKSPACE_PATH/$BUILD_TYPE-build
else
  echo "ERROR: no valid build type."
  exit 1
fi

export DEPS_INSTALL_PREFIX=$WORKSPACE_PATH/deps-install
export KMYMONEY_INSTALL_PREFIX=$WORKSPACE_PATH/kmymoney-install
export IMAGE_INSTALL_PREFIX=$WORKSPACE_PATH/image-install
export DOWNLOADS_DIR=$WORKSPACE_PATH/downloads

# A kmymoney build layout looks like this:
# kmymoney/ -- the source directory
# kmymoney-build/ -- build directory for kmymoney itself
# kmymoney-install/ -- install directory for kmymoney and the dependencies
# deps-build/ -- build directory for the dependencies
# deps-install/ -- the location for the built dependencies

# Make sure our build directory exists
mkdir -p $CMAKE_BUILD_PREFIX
mkdir -p $DEPS_INSTALL_PREFIX
mkdir -p $KMYMONEY_INSTALL_PREFIX
mkdir -p $IMAGE_INSTALL_PREFIX
mkdir -p $DOWNLOADS_DIR

# qjsonparser, used to add metadata to the plugins needs to work in a en_US.UTF-8 environment.
# That's not always the case, so make sure it is
export LC_ALL=en_US.UTF-8
export LANG=en_us.UTF-8

# Setup variables needed to help everything find what we build
#export LD_LIBRARY_PATH=\
#$DEPS_INSTALL_PREFIX/lib:\
#$DEPS_INSTALL_PREFIX/lib/x86_64-linux-gnu:\
#$DEPS_INSTALL_PREFIX/openssl/lib:\
#${LD_LIBRARY_PATH:-}

#/d/msys64/mingw64/bin:\
export PATH=\
$DEPS_INSTALL_PREFIX/bin:\
$DEPS_INSTALL_PREFIX/openssl/bin:\
${PATH:-}

export PKG_CONFIG_PATH=\
$DEPS_INSTALL_PREFIX/share/pkgconfig:\
$DEPS_INSTALL_PREFIX/lib/pkgconfig:\
$DEPS_INSTALL_PREFIX/openssl/lib/pkgconfig:\
${PKG_CONFIG_PATH:-}


export CMAKE_PREFIX_PATH=\
$DEPS_INSTALL_PREFIX\;\
$DEPS_INSTALL_PREFIX/openssl\;\
${CMAKE_PREFIX_PATH:-}

# Get available processors count
export CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`

if [ $BUILD_TYPE == "deps" ];then
  timeout ${REMAINING_TIME}m sh $KMYMONEY_SOURCES/packaging/windows/exe/build-deps.sh || true
elif [ $BUILD_TYPE == "kmymoney" ];then
  timeout ${REMAINING_TIME}m sh $KMYMONEY_SOURCES/packaging/windows/exe/build-kmymoney.sh || true
elif [ $BUILD_TYPE == "image" ];then
  timeout ${REMAINING_TIME}m sh $KMYMONEY_SOURCES/packaging/windows/exe/build-image.sh || true
fi
