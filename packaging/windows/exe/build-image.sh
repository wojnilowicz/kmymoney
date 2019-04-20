#!/bin/bash
#
# Build DmgImage of KMyMoney on MS Windows 7.

# Halt on errors and be verbose about what we are doing
set -eu

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

QT_DIR=$DEPS_INSTALL_PREFIX

KMYMONEY_DMG=$KMYMONEY_INSTALL_PREFIX/Applications/KDE

# Helper functions
countArgs () {
    echo "${#}"
}

stringContains () {
    echo "$(grep "${2}" <<< "${1}")"

}

add_lib_to_list() {
    local llist=${2}
    if test -z "$(grep ${1##*/} <<< ${llist})" ; then
        local llist="${llist} ${1##*/} "
    fi
    echo ${llist}
}

sourceLibPaths=(
$DEPS_INSTALL_PREFIX/bin
/e/msys64/mingw64/bin
)

# Find all @rpath and Absolute to buildroot path libs
# Add to libs_used
# converts absolute buildroot path to @rpath
find_needed_libs () {
    echo "Analizing libraries with ldd..." >&2
    local libs_used="" # input lib_lists founded
    for libFile in $(find $KMYMONEY_INSTALL_PREFIX -type f -name "*.exe" -or -type f -name "*.dll"); do
        lddResult=$(ldd ${libFile} | awk '{print $3}')

        resultArray=(${lddResult}) # convert to array
        echo "${libFile##$KMYMONEY_INSTALL_PREFIX}" >&2

        for lib in ${resultArray[@]:0}; do
            for souceLibPath in ${sourceLibPaths[@]}; do
                if test "${lib:0:${#souceLibPath}}" = "${souceLibPath}"; then
                    local libs_used=$(add_lib_to_list ${lib} "${libs_used}")
                    break
                fi
            done
        done
    done
    echo ${libs_used} # return updated list
}

find_missing_libs (){
    echo "Searching for missing libs on deployment folders…" >&2
    local libs_missing=""
    for lib in ${@}; do
        if test -z "$(find $KMYMONEY_INSTALL_PREFIX -name ${lib})"; then
            echo "Adding ${lib} to missing libraries." >&2
            libs_missing="${libs_missing} ${lib}"
        fi
    done
    echo ${libs_missing}
}

copy_missing_libs () {
    for lib in ${@}; do
        for souceLibPath in ${sourceLibPaths[@]} ; do
            result=$(find "$souceLibPath" -name "${lib}")
            if test $(countArgs ${result[@]}) -gt 0; then
                break
            fi
        done

        if test $(countArgs ${result}) -eq 1; then
            echo ${result}
            if [ "$(stringContains "${result}" "plugin")" ]; then
                echo "copying ${lib} to plugins dir"
                cp -pv ${result} $KMYMONEY_INSTALL_PREFIX/bin
            else
                echo "copying ${lib} to bin dir"
                cp -pv ${result} $KMYMONEY_INSTALL_PREFIX/bin
            fi
        fi
    done
}

kmymoney_findmissinglibs() {
    echo "Starting search for missing libraries"
    neededLibs=$(find_needed_libs)
    echo "\nDone!"
    missingLibs=$(find_missing_libs ${neededLibs})

    if test $(countArgs ${missingLibs}) -gt 0; then
        echo "Found missing libs!"
        echo "${missingLibs}\n"
        copy_missing_libs ${missingLibs}
    else
        echo "No missing libraries found."
    fi

    echo "Done!"
}

createNSIS () {
  cd ${CMAKE_BUILD_PREFIX}
  cp -pv $KMYMONEY_SOURCES/packaging/windows/exe/NullsoftInstaller.nsi $CMAKE_BUILD_PREFIX
  cp -pv $KMYMONEY_SOURCES/COPYING $CMAKE_BUILD_PREFIX
  if [ ! -d $CMAKE_BUILD_PREFIX/bin ]; then
    cp -pr $KMYMONEY_INSTALL_PREFIX/bin $CMAKE_BUILD_PREFIX
  fi
  if [ ! -d $CMAKE_BUILD_PREFIX/qif ]; then
    mkdir -p $CMAKE_BUILD_PREFIX/qif/plugin
    mv $CMAKE_BUILD_PREFIX/bin/kmymoney/*qif* $CMAKE_BUILD_PREFIX/qif/plugin
    mkdir -p $CMAKE_BUILD_PREFIX/qif/service
    mv $CMAKE_BUILD_PREFIX/bin/data/kservices5/*qif* $CMAKE_BUILD_PREFIX/qif/service
  fi

  appName="KMyMoneyNEXT"
  installerBaseName="${appName}-${VERSION}-x86_64"
  IMAGE_BUILD_DIR_WINPATH=$(echo "$CMAKE_BUILD_PREFIX" | sed -e 's/^\///' -e 's/\//\\\\/g' -e 's/^./\0:/')
  echo "------------------------------------"
  echo $IMAGE_BUILD_DIR_WINPATH
  iconName="kmymoney.ico"
  iconFilePath="${IMAGE_BUILD_DIR_WINPATH}\\\\${iconName}"
  licenseFilePath="${IMAGE_BUILD_DIR_WINPATH}\\\\COPYING"
#  png2ico ${iconName} "${KMYMONEY_INSTALL_PREFIX}/bin/data/icons/hicolor/64x64/apps/kmymoney.png"

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
    sed -i "s|@{${key}}|${defines[$key]}|g" $CMAKE_BUILD_PREFIX/NullsoftInstaller.nsi
  done

#  makensis.exe NullsoftInstaller.nsi

  echo "Done!" >&2
}

#echo "Copying libs..."
#cp -fv $DEPS_INSTALL_PREFIX/lib/libicudt64.dll $KMYMONEY_INSTALL_PREFIX/bin/icudt64.dll
#cp -fv $DEPS_INSTALL_PREFIX/lib/libgpgme* $KMYMONEY_INSTALL_PREFIX/bin
#cp -fv $DEPS_INSTALL_PREFIX/lib/libkdewin* $KMYMONEY_INSTALL_PREFIX/bin
#cp -fv $DEPS_INSTALL_PREFIX/lib/libssl* $KMYMONEY_INSTALL_PREFIX/bin

#echo "Copying share..."
#cp -fv $DEPS_INSTALL_PREFIX/bin/data/icons/breeze/breeze-icons.rcc $KMYMONEY_INSTALL_PREFIX/bin/data/icontheme.rcc
#cp -fv $DEPS_INSTALL_PREFIX/bin/data/kservicetypes5/kcmodule* $KMYMONEY_INSTALL_PREFIX/bin/data/kservicetypes5

#echo "Copying plugins..."
#mkdir -p $KMYMONEY_INSTALL_PREFIX/bin/kmymoney
#cp -fv $KMYMONEY_INSTALL_PREFIX/lib/plugins/kmymoney/* $KMYMONEY_INSTALL_PREFIX/bin/kmymoney/
#cp -frv $DEPS_INSTALL_PREFIX/plugins/sqldrivers $KMYMONEY_INSTALL_PREFIX/bin

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

cd $CMAKE_BUILD_PREFIX

#windeployqt $KMYMONEY_INSTALL_PREFIX/bin/kmymoney.exe \
#            --verbose=2 \
#            --release \
#            --qmldir=${DEPS_INSTALL_PREFIX}/qml \
#            --no-translations \
#            --compiler-runtime

#kmymoney_findmissinglibs

createNSIS
