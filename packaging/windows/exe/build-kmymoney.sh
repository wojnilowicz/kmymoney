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
# Build KMyMoney on Ubuntu 16.04.

# Halt on errors and be verbose about what we are doing
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

if [ ! -z ${TRAVIS+x} ]; then
  BUILD_TESTING=FALSE
elif [ ! -z ${APPVEYOR+x} ]; then
  BUILD_TESTING=TRUE
else
  BUILD_TESTING=FALSE
fi

export CXXFLAGS="-O1 -DNDEBUG"
export CFLAGS="-O1 -DNDEBUG"

# Configure KMyMoney for building
cmake -G"MSYS Makefiles" \
      $KMYMONEY_SOURCES \
      -DCMAKE_INSTALL_PREFIX=$KMYMONEY_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$DEPS_INSTALL_PREFIX \
      -DCMAKE_BUILD_TYPE=None \
      -DBUILD_TESTING=FALSE \
      -DIS_APPIMAGE=FALSE

# Reenable after solving undefined reference to `__imp__ZN7Storage3Gpg13KeyDownloader16staticMetaObjectE'
# -DBUILD_TESTING=${BUILD_TESTING} \
      
# Build and Install KMyMoney (ready for the next phase)
cmake --build . --target install -- -j${CPU_COUNT}
