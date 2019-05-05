#!/bin/bash
#
# Build DmgImage of KMyMoney on MS Windows 7.

# Halt on errors and be verbose about what we are doing
set -eu

IMAGE_BUILD_PREFIX=${CMAKE_BUILD_PREFIX}

sourceLibPaths=(
$DEPS_INSTALL_PREFIX/bin
/c/msys64/mingw64/bin
)

# Function expects to be started from bin directory
function find_needed_libs () {
  echo "Analizing libraries with ldd..." >&2
  local needed_libs=() # input lib_lists founded

  for libFile in $(find .. -type f \( -name "*.exe" -or -name "*.dll" \)); do
    echo ${libFile} >&2
    local needed_libs+=($(ldd ${libFile} | awk '{print $3}'))
  done

  echo ${needed_libs[@]} # return updated list
}

function find_missing_libs (){
  echo "Searching for missing libs on deployment folders…" >&2
  local missing_libs=()

  local needed_libs=($(printf '%s\n' "${@}" |
      sort -u |
      sed '/image-build/d' |
      sed '/system32/d' |
      sed '/SYSTEM32/d'))
    
  for lib in ${needed_libs[@]:0}; do
    for souceLibPath in ${sourceLibPaths[@]}; do
      if test "${lib:0:${#souceLibPath}}" = "${souceLibPath}"; then
        echo "Adding ${lib##*/} to missing libraries." >&2
        local missing_libs+=("${lib}")
        break
      fi
    done
  done
  
  echo ${missing_libs[@]}
}

function copy_missing_libs () {
  for lib in ${@}; do
    echo "copying ${lib##*/} to bin dir" >&2
    cp -pvu ${lib} ${IMAGE_BUILD_PREFIX}/bin
  done
}

function kmymoney_findmissinglibs() {
  echo "Starting search for missing libraries"
  needed_libs=($(find_needed_libs))
  missing_libs=($(find_missing_libs ${needed_libs[@]}))

  if test ${#missing_libs[@]} -gt 0; then
      echo "Found missing libs!"
      copy_missing_libs ${missing_libs[@]}
  else
      echo "No missing libraries found."
  fi

  echo "Done!"
}

createNSIS () {
  cd ${IMAGE_BUILD_PREFIX}

  # Extract KMymoney's components
  mkdir -p qif/plugin
  mv bin/kmymoney/*qif* qif/plugin
  mkdir -p qif/service
  mv bin/data/kservices5/*qif* qif/service

  cp -v $KMYMONEY_SOURCES/packaging/windows/exe/NullsoftInstaller.nsi .
  cp -v $KMYMONEY_SOURCES/COPYING .

  appName="KMyMoneyNEXT"
  installerBaseName="${appName}-${VERSION}-x86_64"
  IMAGE_BUILD_DIR_WINPATH=$(echo "$IMAGE_BUILD_PREFIX" | sed -e 's/^\///' -e 's/\//\\\\/g' -e 's/^./\0:/')

  iconName="kmymoney.ico"
  iconFilePath="${IMAGE_BUILD_DIR_WINPATH}\\\\${iconName}"
  licenseFilePath="${IMAGE_BUILD_DIR_WINPATH}\\\\COPYING"
  png2ico ${iconName} "${KMYMONEY_INSTALL_PREFIX}/bin/data/icons/hicolor/64x64/apps/kmymoney.png"

  declare -A defines

  defines=(
  ["appName"]=${appName}
  ["outFile"]=${installerBaseName}.exe
  ["company"]="KDE"
  ["iconName"]=${iconName}
  ["iconFile"]=${iconFilePath}
  ["publisher"]="Lukasz Wojnilowicz"
  ["version"]="${VERSION}"
  ["website"]="https://wojnilowicz.github.io/kmymoneynext"
  ["licenseFile"]="${licenseFilePath}"
  )

  for key in "${!defines[@]}"; do
    sed -i "s|@{${key}}|${defines[$key]}|g" NullsoftInstaller.nsi
  done

  makensis.exe NullsoftInstaller.nsi

  echo "Done!" >&2
}

# Copy installed KMyMoney to adjust its file locations
rm -fr $IMAGE_BUILD_PREFIX
cp -r $KMYMONEY_INSTALL_PREFIX $IMAGE_BUILD_PREFIX
cd $IMAGE_BUILD_PREFIX

echo "Copying libs..."
cp -v $DEPS_INSTALL_PREFIX/bin/libpq.dll bin
cp -v $DEPS_INSTALL_PREFIX/bin/libphonon4qt5* bin
cp -v $DEPS_INSTALL_PREFIX/bin/libgpg* bin

echo "Copying shares..."
if [ -f $DEPS_INSTALL_PREFIX/bin/data/icons/breeze/breeze-icons.rcc ]; then
 cp -v $DEPS_INSTALL_PREFIX/bin/data/icons/breeze/breeze-icons.rcc bin/data/icontheme.rcc
fi

cp -v $DEPS_INSTALL_PREFIX/bin/data/kservicetypes5/kcmodule* bin/data/kservicetypes5

mkdir -p share
touch share/emptyfile
for i in gwenhywfar aqbanking ktoblzcheck; do
 if [ -d $DEPS_INSTALL_PREFIX/share/${i} ]; then
   cp -r $DEPS_INSTALL_PREFIX/share/${i} share
 fi
done

echo "Copying plugins..."
cp -r $DEPS_INSTALL_PREFIX/plugins/sqldrivers bin
mkdir -p bin/kmymoney
mv -v lib/plugins/kmymoney/* bin/kmymoney

if [ -d lib/plugins/sqldrivers ]; then
 mv -v lib/plugins/sqldrivers/* bin/sqldrivers
fi

mkdir -p lib
touch lib/emptyfile
for i in gwenhywfar aqbanking; do
 if [ -d $DEPS_INSTALL_PREFIX/lib/${i} ]; then
   cp -r $DEPS_INSTALL_PREFIX/lib/${i} lib
 fi
done

# windeployqt fails without icudt64.dll, so workaround
cp -fv $DEPS_INSTALL_PREFIX/lib/libicudt64.dll \
      $DEPS_INSTALL_PREFIX/bin/icudt64.dll
windeployqt $IMAGE_BUILD_PREFIX/bin/kmymoney.exe \
          --verbose=2 \
          --release \
          --qmldir=${DEPS_INSTALL_PREFIX}/qml \
          --no-translations \
          --compiler-runtime
# Get rid of the workaround
rm $DEPS_INSTALL_PREFIX/bin/icudt64.dll

# replace stub libicudt64.dll with the real one
cd $IMAGE_BUILD_PREFIX
mv bin/icudt64.dll bin/libicudt64.dll

# Must be invoked from bin, otherwise libraries appear as "??? -> ???"
cd $IMAGE_BUILD_PREFIX/bin
kmymoney_findmissinglibs

# Remove redundant files and directories
cd $IMAGE_BUILD_PREFIX
rm -fr include
rm -fr lib/plugins
find . -type f \( -name *.dll.a -or -name *.la \) -exec rm {} \;

# Strip libraries
find . -type f \( -name *.dll -or -name *.exe \) -exec strip {} \;

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

cd $IMAGE_BUILD_PREFIX
createNSIS
