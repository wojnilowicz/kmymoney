#!/bin/bash
#
# Build DmgImage of KMyMoney on MacOS High Sierra.

# Halt on errors and be verbose about what we are doing
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Save some frequently referenced locations in variables for ease of use / updating
export PLUGINS=$KMYMONEY_INSTALL_PREFIX/lib/plugins/
export APPIMAGEPLUGINS=$KMYMONEY_INSTALL_PREFIX/plugins/

# Now we can get the process started!
#

# # Step 0: place the translations where ki18n and Qt look for them
# if [ -d $KMYMONEY_INSTALL_PREFIX/share/locale ] ; then
#     mv $KMYMONEY_INSTALL_PREFIX/share/locale $KMYMONEY_INSTALL_PREFIX/share/kmymoney
# fi
#
# # Step 1: Copy over all the resources provided by dependencies that we need
# cp -r $DEPS_INSTALL_PREFIX/share/locale $KMYMONEY_INSTALL_PREFIX/share/kmymoney
# cp -r $DEPS_INSTALL_PREFIX/share/kf5 $KMYMONEY_INSTALL_PREFIX/share
# cp -r $DEPS_INSTALL_PREFIX/share/mime $KMYMONEY_INSTALL_PREFIX/share
# if [ -d $DEPS_INSTALL_PREFIX/translations ] ; then
#   cp -r $DEPS_INSTALL_PREFIX/translations $KMYMONEY_INSTALL_PREFIX/usr/
# else
#   echo "Warning: $DEPS_INSTALL_PREFIX/translations does not exist."
# fi
#
# if [ -d $DEPS_INSTALL_PREFIX/openssl/lib ] ; then
#   cp -r $DEPS_INSTALL_PREFIX/openssl/lib/*  $KMYMONEY_INSTALL_PREFIX/lib
# else
#   echo "Warning: $DEPS_INSTALL_PREFIX/openssl/lib does not exist."
# fi
#
#
# # Step 2: Relocate x64 binaries from the architecture specific directory as required for Appimages
# mv $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/*  $KMYMONEY_INSTALL_PREFIX/lib
# rm -rf $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/
#
# # Step 3: Update the rpath in the various plugins we have to make sure they'll be loadable in an Appimage context
# for lib in $PLUGINS/kmymoney/*.so*; do
#   patchelf --set-rpath '$ORIGIN/../../lib' $lib;
# done
#
# # Step 4: Move plugins to loadable location in AppImage
#
# # Make sure our plugin directory already exists
# mkdir -p $APPIMAGEPLUGINS
#
# mv $PLUGINS/* $APPIMAGEPLUGINS

# Step 5: Determine the version of KMyMoney we have just built
# This is needed for linuxdeployqt/appimagetool to do the right thing
cd $KMYMONEY_SOURCES
KMYMONEY_VERSION=$(grep "KMyMoney VERSION" CMakeLists.txt | cut -d '"' -f 2)

# Also find out the revision of Git we built
# Then use that to generate a combined name we'll distribute
if [ -d .git ]; then
  GIT_REVISION=$(git rev-parse --short HEAD)
  export VERSION=$KMYMONEY_VERSION-$GIT_REVISION
else
  export VERSION=$KMYMONEY_VERSION
fi

ls -lh $KMYMONEY_INSTALL_PREFIX/*

cd $CMAKE_BUILD_PREFIX

# Step 7: Build the image!!!
QT_DIR=/usr/local/Cellar/qt/5.12.2
cd $QT_DIR/bin
macdeployqt $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app -verbose=2

cd $CMAKE_BUILD_PREFIX
git clone https://github.com/arl/macdeployqtfix.git
cd $CMAKE_BUILD_PREFIX/macdeployqtfix
python macdeployqtfix.py -v $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/MacOS/kmymoney $QT_DIR

cd $QT_DIR/bin
macdeployqt $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app -dmg -always-overwrite -verbose=2

if [ -f $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.dmg ]; then
  mv $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.dmg $CMAKE_BUILD_PREFIX/kmymoney-$VERSION-x86_64.dmg
fi
