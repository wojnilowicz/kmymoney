#!/bin/bash
#
# Build all KMyMoney's dependencies on Microsoft Windows.
#
#
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

 cd $DOWNLOADS_DIR
# wget https://www.python.org/ftp/python/3.7.3/python-3.7.3-amd64.exe
# ./python-3.7.3-amd64.exe /quiet TargetDir=$DEPS_INSTALL_PREFIX/python
# export PATH=$DEPS_INSTALL_PREFIX/python:$PATH

# NINJA_EXECUTABLE=$DEPS_INSTALL_PREFIX/bin/ninja
# if [ ! -f $NINJA_EXECUTABLE ] ||
#    [ $(tr . 0 <<< $($NINJA_EXECUTABLE --version)) -lt $(tr . 0 <<< "1.9.0") ] ; then
#   rm -fr ninja
#   git clone --single-branch -b release --depth 1 git://github.com/ninja-build/ninja.git
#   cd ninja
#   ./configure.py --bootstrap --platform=mingw --host=mingw
#   mkdir -p $DEPS_INSTALL_PREFIX/bin
#   install -vm755 ninja $DEPS_INSTALL_PREFIX/bin
#   cd ..
#   rm -fr ninja
# fi

cd $CMAKE_BUILD_PREFIX
# Configure the dependencies for building
cmake -G "MSYS Makefiles" \
      $KMYMONEY_SOURCES/3rdparty \
      -DCMAKE_INSTALL_PREFIX=$DEPS_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DEXT_DOWNLOAD_DIR=$DOWNLOADS_DIR

# Now start building everything we need, in the appropriate order
cmake --build . --target ext_pkgconfig -- -j${CPU_COUNT}

cmake --build . --target ext_gettext -- -j${CPU_COUNT}
cmake --build . --target ext_xslt -- -j${CPU_COUNT}
cmake --build . --target ext_png -- -j${CPU_COUNT}

cmake --build . --target ext_kdewin -- -j${CPU_COUNT}
# cmake --build . --target ext_jpeg -- -j${CPU_COUNT} #this causes build failures in Qt 5.10

if [ ! -f $DEPS_INSTALL_PREFIX/bin/Qt5Core.dll ]; then
  bash -c "for i in {1..5};do sleep 9m; echo \"Still building\"; done;" &
  cmake --build . --target ext_qtbase -- -j${CPU_COUNT}
fi

cmake --build . --target ext_qttools -- -j${CPU_COUNT}
cmake --build . --target ext_qtdeclarative -- -j${CPU_COUNT}
cmake --build . --target ext_qtwinextras -- -j${CPU_COUNT}
# cmake --build . --target ext_qtwebchannel -- -j${CPU_COUNT}
# cmake --build . --target ext_qtwebengine -- -j${CPU_COUNT}

 cmake --build . --target ext_breezeicons -- -j${CPU_COUNT}
 cmake --build . --target ext_kcmutils -- -j${CPU_COUNT}
 cmake --build . --target ext_kactivities -- -j${CPU_COUNT}
 cmake --build . --target ext_kitemmodels -- -j${CPU_COUNT}

 cmake --build . --target ext_gmp -- -j${CPU_COUNT}
# cmake --build . --target ext_kholidays -- -j${CPU_COUNT}
# cmake --build . --target ext_kidentitymanagement -- -j${CPU_COUNT}
# cmake --build . --target ext_kcontacts -- -j${CPU_COUNT}
# cmake --build . --target ext_akonadi -- -j${CPU_COUNT}
# cmake --build . --target ext_alkimia -- -j${CPU_COUNT}
# cmake --build . --target ext_kdiagram -- -j${CPU_COUNT}
# cmake --build . --target ext_aqbanking -- -j${CPU_COUNT}
# cmake --build . --target ext_gpgme -- -j${CPU_COUNT}
# cmake --build . --target ext_sqlcipher -- -j${CPU_COUNT}
# cmake --build . --target ext_ofx -- -j${CPU_COUNT}
# cmake --build . --target ext_ical -- -j${CPU_COUNT}
cmake --build . --target ext_png2ico -- -j${CPU_COUNT}
