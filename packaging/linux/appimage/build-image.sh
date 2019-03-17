#!/bin/bash
#
# Build AppImage of KMyMoney on Ubuntu 16.04.

# Halt on errors and be verbose about what we are doing
set -euxo pipefail

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Save some frequently referenced locations in variables for ease of use / updating
export PLUGINS=$KMYMONEY_INSTALL_PREFIX/lib/plugins/
export APPIMAGEPLUGINS=$KMYMONEY_INSTALL_PREFIX/plugins/

ls -lh $KMYMONEY_INSTALL_PREFIX/*

# Now we can get the process started!
#

# Step 0: place the translations where ki18n and Qt look for them
if [ -d $KMYMONEY_INSTALL_PREFIX/share/locale ] ; then
    mv $KMYMONEY_INSTALL_PREFIX/share/locale $KMYMONEY_INSTALL_PREFIX/share/kmymoney
fi

# Step 1: Copy over all the resources provided by dependencies that we need
cp -r $DEPS_INSTALL_PREFIX/share/locale $KMYMONEY_INSTALL_PREFIX/share/kmymoney
cp -r $DEPS_INSTALL_PREFIX/share/kf5 $KMYMONEY_INSTALL_PREFIX/share
cp -r $DEPS_INSTALL_PREFIX/share/mime $KMYMONEY_INSTALL_PREFIX/share
if [ -d $DEPS_INSTALL_PREFIX/translations ] ; then
  cp -r $DEPS_INSTALL_PREFIX/translations $KMYMONEY_INSTALL_PREFIX/usr/
else
  echo "Warning: $DEPS_INSTALL_PREFIX/translations does not exist."
fi

if [ -d $DEPS_INSTALL_PREFIX/openssl/lib ] ; then
  cp -r $DEPS_INSTALL_PREFIX/openssl/lib/*  $KMYMONEY_INSTALL_PREFIX/lib
else
  echo "Warning: $DEPS_INSTALL_PREFIX/openssl/lib does not exist."
fi


# Step 2: Relocate x64 binaries from the architecture specific directory as required for Appimages
mv $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/*  $KMYMONEY_INSTALL_PREFIX/lib
rm -rf $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/

# Step 3: Update the rpath in the various plugins we have to make sure they'll be loadable in an Appimage context
for lib in $PLUGINS/kmymoney/*.so*; do
  patchelf --set-rpath '$ORIGIN/../../lib' $lib;
done

# Step 4: Move plugins to loadable location in AppImage

# Make sure our plugin directory already exists
mkdir -p $APPIMAGEPLUGINS

mv $PLUGINS/* $APPIMAGEPLUGINS

# Step 5: Determine the version of KMyMoney we have just built
# This is needed for linuxdeployqt/appimagetool to do the right thing
cd $KMYMONEY_SOURCES
KMYMONEY_VERSION=$(grep "KMyMoney VERSION" CMakeLists.txt | cut -d '"' -f 2)

# Also find out the revision of Git we built
# Then use that to generate a combined name we'll distribute
if [[ -d .git ]]; then
  GIT_REVISION=$(git rev-parse --short HEAD)
  export VERSION=$KMYMONEY_VERSION-$GIT_REVISION
else
  export VERSION=$KMYMONEY_VERSION
fi

# Finally transition back to the build directory so we can build the appimage
cd $DOWNLOADS_DIR
#Step 6: Download tool to create AppImage
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage

cd $CMAKE_BUILD_PREFIX

# Step 7: Build the image!!!
$DOWNLOADS_DIR/linuxdeployqt-continuous-x86_64.AppImage $KMYMONEY_INSTALL_PREFIX/share/applications/org.kde.kmymoney.desktop \
  -executable=$KMYMONEY_INSTALL_PREFIX/bin/kmymoney \
  -qmldir=/opt/qt512/qml \
  -verbose=2 \
  -bundle-non-qt-libs \
  -appimage \
  -exclude-libs=libnss3.so,libnssutil3.so