#!/bin/bash
#
# Build all KMyMoney's dependencies on Ubuntu 16.04.
#
# Prerequisites: cmake git build-essential libxcb-keysyms1-dev plus all deps for Qt5
#
set -euxo pipefail

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Configure the dependencies for building
cmake -GNinja \
      $KMYMONEY_SOURCES/3rdparty \
      -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DEXT_DOWNLOAD_DIR=$DOWNLOADS_DIR

# Now start building everything we need, in the appropriate order
# cmake --build . --target ext_iconv -- -j${CPU_COUNT}
# cmake --build . --target ext_lzma -- -j${CPU_COUNT}
# cmake --build . --target ext_xml -- -j${CPU_COUNT}
# cmake --build . --target ext_gettext -- -j${CPU_COUNT}
# cmake --build . --target ext_xslt -- -j${CPU_COUNT}
# cmake --build . --target ext_png -- -j${CPU_COUNT}
# cmake --build . --target ext_jpeg -- -j${CPU_COUNT} #this causes build failures in Qt 5.10
# cmake --build . --target ext_qt -- -j${CPU_COUNT}
# cmake --build . --target ext_boost -- -j${CPU_COUNT}
# cmake --build . --target ext_kcmutils -- -j${CPU_COUNT}
# cmake --build . --target ext_kactivities -- -j${CPU_COUNT}
# cmake --build . --target ext_kitemmodels -- -j${CPU_COUNT}
# cmake --build . --target ext_kitemviews -- -j${CPU_COUNT}
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
cmake --build . --target ext_patchelf -- -j${CPU_COUNT}