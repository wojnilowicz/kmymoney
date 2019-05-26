#!/bin/bash
#
# Build AppImage of KMyMoney on Ubuntu 16.04.

# Halt on errors and be verbose about what we are doing
set -eux

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

# Save some frequently referenced locations in variables for ease of use / updating
export PLUGINS=$KMYMONEY_INSTALL_PREFIX/lib/plugins/
export APPIMAGEPLUGINS=$KMYMONEY_INSTALL_PREFIX/plugins/

# Now we can get the process started!
#

echo "Copying libs..."

echo "Copying share..."
rsync -prul $DEPS_INSTALL_PREFIX/share/kservicetypes5/kcm* $KMYMONEY_INSTALL_PREFIX/share/kservicetypes5
cp -pv $DEPS_INSTALL_PREFIX/share/icons/breeze/breeze-icons.rcc $KMYMONEY_INSTALL_PREFIX/share

echo "Copying plugins..."
mkdir -p $PLUGINS/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/file* $PLUGINS/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/http* $PLUGINS/kf5/kio
rsync -prul $DEPS_INSTALL_PREFIX/plugins/sqldrivers $PLUGINS

if [ -d $DEPS_INSTALL_PREFIX/share/aqbanking ]; then
  echo "Copying aqbanking and gwenhywfar..."
  rsync -prul $DEPS_INSTALL_PREFIX/share/aqbanking $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/share/gwenhywfar $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/share/ktoblzcheck $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/lib/gwenhywfar/plugins/60/* $PLUGINS/gwenhywfar
  rsync -prul $DEPS_INSTALL_PREFIX/lib/aqbanking/plugins/35/* $PLUGINS/aqbanking
fi

# Step 2: Relocate x64 binaries from the architecture specific directory as required for Appimages
rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/*  $KMYMONEY_INSTALL_PREFIX/lib
rm -rf $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu

# Step 3: Update the rpath in the various plugins we have to make sure they'll be loadable in an Appimage context
pluginFiles=$(find $PLUGINS/kmymoney -type f -and -name "*.so")
for pluginFile in ${pluginFiles}; do
  echo "Patching kmymoney plugins..."
  patchelf --set-rpath '$ORIGIN/../../lib' $pluginFile;
done

if [ -d $PLUGINS/aqbanking ]; then
  echo "Patching aqbanking plugins..."
  pluginFiles=$(find $PLUGINS/aqbanking -type f -and -name "*.so")
  for pluginFile in ${pluginFiles}; do
    patchelf --set-rpath '$ORIGIN/../../../lib' $pluginFile;
  done
fi

if [ -d $PLUGINS/gwenhywfar ]; then
  echo "Patching gwenhywfar plugins..."
  pluginFiles=$(find $PLUGINS/gwenhywfar -type f -and -name "*.so")
  for pluginFile in ${pluginFiles}; do
    patchelf --set-rpath '$ORIGIN/../../../lib' $pluginFile;
  done
fi

if [ -f $PLUGINS/sqldrivers/qsqlcipher.so ]; then
  echo "Patching qsqlcipher plugin..."
  patchelf --set-rpath '$ORIGIN/../../../lib' $PLUGINS/sqldrivers/qsqlcipher.so;
fi

# Step 4: Move plugins to loadable location in AppImage

# Make sure our plugin directory already exists
mkdir -p $APPIMAGEPLUGINS

rsync -prul $PLUGINS/* $APPIMAGEPLUGINS
rm -fr $PLUGINS

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

# Finally transition back to the build directory so we can build the appimage
cd $DOWNLOADS_DIR
#Step 6: Download tool to create AppImage
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
wget -c -nv "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
chmod a+x appimagetool-x86_64.AppImage

cd $KMYMONEY_INSTALL_PREFIX
# Remove redundant files and directories
find . \( -type f -and \( -name *.a -or -name *.la \) \) -exec rm {} \;

# Strip libraries
find . \( -type f -and \( -name *.so -or -name kmymoney \) \) -exec strip {} \;

rm -fr $KMYMONEY_INSTALL_PREFIX/include

cd $CMAKE_BUILD_PREFIX
# Step 7: Build the image!!!
$DOWNLOADS_DIR/linuxdeployqt-continuous-x86_64.AppImage \
  $KMYMONEY_INSTALL_PREFIX/share/applications/org.kde.kmymoney.desktop \
  -qmldir=$DEPS_INSTALL_PREFIX/qml \
  -verbose=2 \
  -bundle-non-qt-libs \
  -exclude-libs=libnss3.so,libnssutil3.so \
  -extra-plugins=iconengines,platformthemes/libqgtk3.so

#   -appimage \

cd $KMYMONEY_INSTALL_PREFIX
cd ..
APPDIR=$(pwd)

mv -f  $KMYMONEY_SOURCES/packaging/linux/appimage/AppRun ${APPDIR}
cd $CMAKE_BUILD_PREFIX
$DOWNLOADS_DIR/appimagetool-x86_64.AppImage --comp xz ${APPDIR} KMyMoneyNEXT-${KMYMONEY_VERSION}-x86_64.AppImage