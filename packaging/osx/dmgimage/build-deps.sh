#!/bin/bash
#
# Build all KMyMoney's dependencies on MacOS High Sierra.

set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Those flags will be propageted to Autotools and CMake
# KChart produces many -Wzero-as-null-pointer-constant
# Solid and KIO produces many -Wnonportable-include-path
export CXXFLAGS="-O2 -DNDEBUG -w"

export CFLAGS="-O2 -DNDEBUG -w"

# Build ninja from source in order to avoid lenghty "brew install ninja"
cd $CMAKE_BUILD_PREFIX
NINJA_EXECUTABLE=$DEPS_INSTALL_PREFIX/bin/ninja
if [ ! -f $NINJA_EXECUTABLE ] ||
   [ $(tr . 0 <<< $($NINJA_EXECUTABLE --version)) -lt $(tr . 0 <<< "1.9.0") ] ; then
  rm -fr ninja
  git clone --single-branch -b release --depth 1 git://github.com/ninja-build/ninja.git
  cd ninja
  python3 configure.py --bootstrap
  mkdir -p $DEPS_INSTALL_PREFIX/bin
  install -vm755 ninja $DEPS_INSTALL_PREFIX/bin
  cd ..
  rm -fr ninja
fi

cmake --version
# Configure the dependencies for building
cmake -G "Unix Makefiles" \
      $KMYMONEY_SOURCES/3rdparty \
      -DCMAKE_INSTALL_PREFIX=$DEPS_INSTALL_PREFIX \
      -DCMAKE_BUILD_TYPE=None \
      -DEXT_DOWNLOAD_DIR=$DOWNLOADS_DIR \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 \
      -DDARWIN_KERNEL_VERSION=16.0.0

# Now start building everything we need, in the appropriate order

# if [ ! -f $DEPS_INSTALL_PREFIX/lib/libglib* ]; then
#   if [ ! -z ${TRAVIS+x} ]; then pip3 install meson; fi;
#   cmake --build . --target ext_glib -- -j${CPU_COUNT}
# fi

if [ ! -f $DEPS_INSTALL_PREFIX/lib/Qt5Core.dydl ]; then
  if [ ! -z ${TRAVIS+x} ]; then bash -c "for i in {1..4};do sleep 540; echo \"Still building\"; done;" & fi; #MacOS accepts only seconds
  cmake --build . --target ext_qtbase -- -j${CPU_COUNT}
fi

if [ ! -f $DEPS_INSTALL_PREFIX/lib/Qt5Qml.dydl ]; then
  if [ ! -z ${TRAVIS+x} ]; then bash -c "for i in {1..4};do sleep 540; echo \"Still building\"; done;" & fi; #MacOS accepts only seconds
  cmake --build . --target ext_qtdeclarative -- -j${CPU_COUNT}
fi

cmake --build . --target ext_qttools -- -j${CPU_COUNT}
cmake --build . --target ext_qtspeech -- -j${CPU_COUNT}
cmake --build . --target ext_qtmacextras -- -j${CPU_COUNT}

cmake --build . --target ext_bison -- -j${CPU_COUNT} # required by solid
cmake --build . --target ext_xslt -- -j${CPU_COUNT} # otherwise xslt from system is used for KI18n which requires _xmlModuleClose

cmake --build . --target ext_kcmutils -- -j${CPU_COUNT}
cmake --build . --target ext_kactivities -- -j${CPU_COUNT}
cmake --build . --target ext_kitemmodels -- -j${CPU_COUNT}
cmake --build . --target ext_kinit -- -j${CPU_COUNT}
cmake --build . --target ext_khtml -- -j${CPU_COUNT}
cmake --build . --target ext_kholidays -- -j${CPU_COUNT}
cmake --build . --target ext_kidentitymanagement -- -j${CPU_COUNT}
cmake --build . --target ext_kcontacts -- -j${CPU_COUNT}
cmake --build . --target ext_kdiagram -- -j${CPU_COUNT}
cmake --build . --target ext_aqbanking -- -j${CPU_COUNT}
cmake --build . --target ext_sqlcipher -- -j${CPU_COUNT}
cmake --build . --target ext_ofx -- -j${CPU_COUNT}
cmake --build . --target ext_ical -- -j${CPU_COUNT}
cmake --build . --target ext_breezeicons -- -j${CPU_COUNT}
