#!/bin/bash
#
# Build KMyMoney on MacOS High Sierra.

# Halt on errors and be verbose about what we are doing
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Configure KMyMoney for building
cmake -GNinja \
      $KMYMONEY_SOURCES \
      -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DBUILD_TESTING=FALSE \
      -DENABLE_WEBENGINE=TRUE \
      -DIS_APPIMAGE=FALSE

# Build and Install KMyMoney (ready for the next phase)
cmake --build . --target install -- -j${CPU_COUNT}