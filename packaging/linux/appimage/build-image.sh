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

if [ -f $PLUGINS/sqldrivers/qsqlcipher.so ]; then
  echo "Copying qsqlcipher for tests..."
  cp -fv $PLUGINS/sqldrivers/qsqlcipher.so $DEPS_INSTALL_PREFIX/plugins/sqldrivers
fi

echo "Copying libs..."

echo "Copying share..."
rsync -prul $DEPS_INSTALL_PREFIX/share/kservicetypes5/kcm* $KMYMONEY_INSTALL_PREFIX/share/kservicetypes5
cp -pv $DEPS_INSTALL_PREFIX/share/icons/breeze/breeze-icons.rcc $KMYMONEY_INSTALL_PREFIX/share/kmymoney/icontheme.rcc

echo "Copying plugins..."
mkdir -p $PLUGINS/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/file* $PLUGINS/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/http* $PLUGINS/kf5/kio
echo "Patching kio..."
pluginFiles=$(find $PLUGINS/kf5/kio -type f)
for pluginFile in ${pluginFiles}; do
  patchelf --set-rpath '$ORIGIN/../../../lib' $pluginFile;
done

rsync -prul $DEPS_INSTALL_PREFIX/plugins/sqldrivers $PLUGINS

if [ -d $DEPS_INSTALL_PREFIX/share/aqbanking ]; then
  echo "Copying aqbanking and gwenhywfar..."
  rsync -prul $DEPS_INSTALL_PREFIX/share/aqbanking $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/share/gwenhywfar $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/share/ktoblzcheck $KMYMONEY_INSTALL_PREFIX/share
  rsync -prul $DEPS_INSTALL_PREFIX/lib/gwenhywfar/plugins/60/* $PLUGINS/gwenhywfar
  rsync -prul $DEPS_INSTALL_PREFIX/lib/aqbanking/plugins/35/* $PLUGINS/aqbanking
fi

if [ -f $DEPS_INSTALL_PREFIX/lib/libKF5KHtml.so.5 ]; then
  echo "Copying KF5KHtml..."
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libKF5KHtml* $KMYMONEY_INSTALL_PREFIX/lib
  mkdir -p $KMYMONEY_INSTALL_PREFIX/share/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/share/kf5/khtml $KMYMONEY_INSTALL_PREFIX/share/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/share/kservicetypes5/qimageio* $KMYMONEY_INSTALL_PREFIX/share/kservicetypes5
fi

if [ -d $DEPS_INSTALL_PREFIX/share/dbus-1 ]; then
  echo "Copying DBus..."
  rsync -prul $DEPS_INSTALL_PREFIX/bin/dbus* $KMYMONEY_INSTALL_PREFIX/bin
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libdbus-1.so* $KMYMONEY_INSTALL_PREFIX/lib
  rsync -prul $DEPS_INSTALL_PREFIX/share/dbus-1 $KMYMONEY_INSTALL_PREFIX/share
  mkdir -p $KMYMONEY_INSTALL_PREFIX/libexec/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libexec/kf5/kioslave $KMYMONEY_INSTALL_PREFIX/libexec/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libexec/kf5/klauncher $KMYMONEY_INSTALL_PREFIX/libexec/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libkdeinit5_klauncher* $KMYMONEY_INSTALL_PREFIX/lib
  rsync -prul $DEPS_INSTALL_PREFIX/bin/kdeinit5 $KMYMONEY_INSTALL_PREFIX/bin
  echo "Patching dbus..."
  dbusFiles=$(find $KMYMONEY_INSTALL_PREFIX/bin -type f -and -name "dbus*" -o -name "kdeinit5")
  for dbusFile in ${dbusFiles}; do
    patchelf --set-rpath '$ORIGIN/../lib' $dbusFile;
  done

  echo "Patching libexec..."
  libexecFiles=$(find $KMYMONEY_INSTALL_PREFIX/libexec/kf5 -type f)
  for libexecFile in ${libexecFiles}; do
    patchelf --set-rpath '$ORIGIN/../../lib' $libexecFiles;
  done

fi

if [ -d $DEPS_INSTALL_PREFIX/plugins/mariadb ]; then
  echo "Copying MariaDB plugins..."
  rsync -prul $DEPS_INSTALL_PREFIX/plugins/mariadb $PLUGINS
  if [ -f $PLUGINS/mariadb/remote_io.so ]; then
    rm $PLUGINS/mariadb/remote_io.so
  fi
fi

# Step 2: Relocate x64 binaries from the architecture specific directory as required for Appimages
if [ -d $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu ]; then
  rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu/*  $KMYMONEY_INSTALL_PREFIX/lib
  rm -rf $KMYMONEY_INSTALL_PREFIX/lib/x86_64-linux-gnu
fi

# Step 3: Update the rpath in the various plugins we have to make sure they'll be loadable in an Appimage context
pluginFiles=$(find $PLUGINS/kmymoney -type f -and -name "*.so")
echo "Patching kmymoney plugins..."
for pluginFile in ${pluginFiles}; do
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

if [ -d $PLUGINS/mariadb ]; then
  echo "Patching mariadb plugins..."
  pluginFiles=$(find $PLUGINS/mariadb -type f -and -name "*.so")
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
wget -c -nv "https://github.com/AppImage/AppImageKit/releases/download/continuous/runtime-x86_64"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
chmod a+x appimagetool-x86_64.AppImage

cd $CMAKE_BUILD_PREFIX
# Step 7: Build the image!!!
$DOWNLOADS_DIR/linuxdeployqt-continuous-x86_64.AppImage \
  $KMYMONEY_INSTALL_PREFIX/share/applications/org.kde.kmymoney.desktop \
  -qmldir=$DEPS_INSTALL_PREFIX/qml \
  -verbose=2 \
  -bundle-non-qt-libs \
  -executable=$KMYMONEY_INSTALL_PREFIX/libexec/kf5/kioslave \
  -executable=$KMYMONEY_INSTALL_PREFIX/libexec/kf5/klauncher \
  -executable=$KMYMONEY_INSTALL_PREFIX/bin/dbus-daemon \
  -executable=$KMYMONEY_INSTALL_PREFIX/bin/dbus-launch \
  -exclude-libs=libnss3.so,libnssutil3.so \

cd $KMYMONEY_INSTALL_PREFIX
cd ..
APPDIR=$(pwd)

cd $KMYMONEY_INSTALL_PREFIX
# Remove redundant files and directories
rm -fr $KMYMONEY_INSTALL_PREFIX/include
rm -fr $KMYMONEY_INSTALL_PREFIX/share/doc
rm -fr $KMYMONEY_INSTALL_PREFIX/share/kmymoney/icons/oxygen
rm -fr $KMYMONEY_INSTALL_PREFIX/share/kmymoney/icons/Tango
rm -f $KMYMONEY_INSTALL_PREFIX/lib/libQt5*Test*
find . \( -type f -and \( -name *.a -or -name *.la \) \) -exec rm {} \;

rsync -prul $DEPS_INSTALL_PREFIX/lib/libgpg-error.so* $KMYMONEY_INSTALL_PREFIX/lib
patchelf --set-rpath '$ORIGIN/./lib' $KMYMONEY_INSTALL_PREFIX/lib/libgpg-error.so.0

rsync -prul $DEPS_INSTALL_PREFIX/lib/libmariadb.so* $KMYMONEY_INSTALL_PREFIX/lib
patchelf --set-rpath '$ORIGIN/./lib' $KMYMONEY_INSTALL_PREFIX/lib/libmariadb.so.3

if [ -f $KMYMONEY_INSTALL_PREFIX/lib/libmysqlclient.so.20 ]; then
rm $KMYMONEY_INSTALL_PREFIX/lib/libmysqlclient.so.20
fi

if [ -f $KMYMONEY_INSTALL_PREFIX/lib/libgssapi.so.3 ]; then
  echo "Removing curl libaries..."
  rm $KMYMONEY_INSTALL_PREFIX/lib/libgssapi*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libkrb5*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libasn1*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libroken*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libhx509*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libwind*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libheim*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libhcrypto*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libcurl-gnutls*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libidn*
  rm $KMYMONEY_INSTALL_PREFIX/lib/librtmp*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libldap*
  rm $KMYMONEY_INSTALL_PREFIX/lib/liblber*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libk5crypto*
  rm $KMYMONEY_INSTALL_PREFIX/lib/libsasl*

fi

# Strip libraries
find . \( -type f -and \( -name *.so -or -name kmymoney \) \) -exec strip {} \;

mv -f  $KMYMONEY_SOURCES/packaging/linux/appimage/AppRun ${APPDIR}
cd $CMAKE_BUILD_PREFIX
APPIMAGE_NAME="KMyMoneyNEXT-${VERSION}-x86_64"
mksquashfs ${APPDIR} ${APPIMAGE_NAME}.squashfs -root-owned -noappend -b 1M -comp xz
cat $DOWNLOADS_DIR/runtime-x86_64 >> ${APPIMAGE_NAME}.AppImage
cat ${APPIMAGE_NAME}.squashfs >> ${APPIMAGE_NAME}.AppImage
chmod a+x ${APPIMAGE_NAME}.AppImage

# $DOWNLOADS_DIR/appimagetool-x86_64.AppImage --comp xz ${APPDIR} KMyMoneyNEXT-${VERSION}-x86_64.AppImage