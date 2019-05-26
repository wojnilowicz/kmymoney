#!/bin/bash
#
# Build KMyMoney on Ubuntu 16.04.

# Halt on errors and be verbose about what we are doing
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Configure KMyMoney for building
cmake -G"Unix Makefiles" \
      $KMYMONEY_SOURCES \
      -DCMAKE_INSTALL_PREFIX=$KMYMONEY_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$DEPS_INSTALL_PREFIX \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DBUILD_TESTING=TRUE \
      -DIS_APPIMAGE=FALSE

# Build and Install KMyMoney (ready for the next phase)
cmake --build . --target install -- -j${CPU_COUNT}