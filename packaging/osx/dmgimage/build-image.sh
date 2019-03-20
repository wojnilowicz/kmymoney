#!/bin/bash
#
# Build DmgImage of KMyMoney on MacOS High Sierra.

# Halt on errors and be verbose about what we are doing
set -eu

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

ls -lh $DEPS_INSTALL_PREFIX/*

QT_DIR=/usr/local/Cellar/qt/5.12.2
export MACOSX_DEPLOYMENT_TARGET=10.11
export QMAKE_MACOSX_DEPLOYMENT_TARGET=10.11

# Save some frequently referenced locations in variables for ease of use / updating
export PLUGINS=$KMYMONEY_INSTALL_PREFIX/lib/plugins/
export APPIMAGEPLUGINS=$KMYMONEY_INSTALL_PREFIX/plugins/

mkdir -p $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/PlugIns
mkdir -p $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/Frameworks

KMYMONEY_DMG=$KMYMONEY_INSTALL_PREFIX/Applications/KDE
DMG_title=kmymoney

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


# Find all @rpath and Absolute to buildroot path libs
# Add to libs_used
# converts absolute buildroot path to @rpath
find_needed_libs () {
    echo "Analizing libraries with oTool..." >&2
    local libs_used="" # input lib_lists founded
    for libFile in $(find ${KMYMONEY_DMG}/kmymoney.app/Contents -name "*.so" -or -name "*.dylib"); do
        oToolResult=$(otool -L ${libFile} | awk '{print $1}')
        resultArray=(${oToolResult}) # convert to array
        echo "${libFile##*Contents/}" >&2
        for lib in ${resultArray[@]:1}; do
            if test "${lib:0:1}" = "@"; then
                local libs_used=$(add_lib_to_list ${lib} "${libs_used}")
            fi
            if test "${lib:0:${#WORKSPACE_PATH}}" = "${WORKSPACE_PATH}"; then
                install_name_tool -id ${lib##*/} "${libFile}"
                install_name_tool -change ${lib} "@rpath/${lib##*/}" "${libFile}"
                local libs_used=$(add_lib_to_list ${lib} "${libs_used}")
            fi
        done
    done
    echo ${libs_used} # return updated list
}

find_missing_libs (){
    echo "Searching for missing libs on deployment folders…" >&2
    local libs_missing=""
    for lib in ${@}; do
        if test -z "$(find ${KMYMONEY_DMG}/kmymoney.app/Contents/ -name ${lib})"; then
            echo "Adding ${lib} to missing libraries." >&2
            libs_missing="${libs_missing} ${lib}"
        fi
    done
    echo ${libs_missing}
}

copy_missing_libs () {
    for lib in ${@}; do
        result=$(find "$DEPS_INSTALL_PREFIX" -name "${lib}")

        if test $(countArgs ${result}) -eq 1; then
            echo ${result}
            if [ "$(stringContains "${result}" "plugin")" ]; then
                echo "copying ${lib} to plugins dir"
                cp -pv ${result} ${KMYMONEY_DMG}/kmymoney.app/Contents/PlugIns/
            else
                echo "copying ${lib} to Frameworks dir"
                cp -pv ${result} ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/
            fi
        else
            echo "${lib} might be a missing framework"
            if [ "$(stringContains "${result}" "framework")" ]; then
                echo "copying framework $DEPS_INSTALL_PREFIX/lib/${lib}.framework to dmg"
                # TODO rsync only included ${lib} Resources Versions
                rsync -priul $DEPS_INSTALL_PREFIX/lib/${lib}.framework/${lib} ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
                rsync -priul $DEPS_INSTALL_PREFIX/lib/${lib}.framework/Resources ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
                rsync -priul $DEPS_INSTALL_PREFIX/lib/${lib}.framework/Versions ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
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

createDMG () {
    echo "Starting creation of dmg..."
    cd $CMAKE_BUILD_PREFIX
    DMG_size=500

    ## Build dmg from folder

    # create dmg on local system
    # usage of -fsargs minimze gaps at front of filesystem (reduce size)
    hdiutil create -srcfolder "${KMYMONEY_DMG}" -volname "${DMG_title}" -fs HFS+ \
        -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${DMG_size}m kmymoney.temp.dmg
    # Next line is only useful if we have a dmg as a template!
    # previous hdiutil must be uncommented
    # cp kmymoney-template.dmg kmymoney.dmg

    device=$(hdiutil attach -readwrite -noverify -noautoopen "kmymoney.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

    # Set style for dmg
#     if [[ ! -d "/Volumes/${DMG_title}/.background" ]]; then
#         mkdir "/Volumes/${DMG_title}/.background"
#     fi
#     cp ${BUILDROOT}/${DMG_background} "/Volumes/${DMG_title}/.background/"

    ## Apple script to set style
    echo '
        tell application "Finder"
            tell disk "'${DMG_title}'"
                open
                set current view of container window to icon view
                set toolbar visible of container window to false
                set statusbar visible of container window to false
                set the bounds of container window to {186, 156, 956, 592}
                set theViewOptions to the icon view options of container window
                set arrangement of theViewOptions to not arranged
                set icon size of theViewOptions to 80
                set background picture of theViewOptions to file ".background:'${DMG_background}'"
                set position of item "'kmymoney.app'" of container window to {279, 272}
                set position of item "Applications" of container window to {597, 272}
                set position of item "Terms of Use" of container window to {597, 110}
                update without registering applications
                delay 1
                close
            end tell
        end tell
        ' | osascript


    chmod -Rf go-w "/Volumes/${DMG_title}"

    # ensure all writting operations to dmg are over
    sync

    hdiutil detach $device
    hdiutil convert "kmymoney.temp.dmg" -format UDZO -imagekey -zlib-level=9 -o kmymoney-out.dmg

    # Add git version number
#     GIT_SHA=$(grep "#define KMYMONEY_GIT_SHA1_STRING" ${KIS_BUILD_DIR}/libs/version/kmymoneygitversion.h | awk '{gsub(/"/, "", $3); printf $3}')

    mv kmymoney-out.dmg kmymoney-nightly_${GIT_SHA}.dmg
    echo "moved kmymoney-out.dmg to kmymoney-$VERSION-x86_64.dmg"
    rm kmymoney.temp.dmg

    echo "dmg done!"
}


echo "Copying share..."
cd  $DEPS_INSTALL_PREFIX/share
rsync -prul --delete ./ \
        --exclude kmymoney_SRCS.icns \
        --exclude aclocal \
        --exclude doc \
        --exclude ECM \
        --exclude eigen3 \
        --exclude emacs \
        --exclude gettext \
        --exclude gettext-0.19.8 \
        --exclude info \
        --exclude kf5 \
        --exclude kservices5 \
        --exclude man \
        --exclude ocio \
        --exclude pkgconfig \
        --exclude mime \
        --exclude translations \
        --exclude qml \
        $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/Resources

echo "Copying plugins..."
# exclude kmymoneyquicklook.qlgenerator/
# cd $DEPS_INSTALL_PREFIX/plugins/
# rsync -prul --delete \
#   $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/PlugIns

rsync -prul $QT_DIR/plugins/kmymoney/ $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/PlugIns

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
cd $QT_DIR/bin
macdeployqt $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app \
            -verbose=1 \
            -qmldir=$QT_DIR/qml \
            -libpath=$DEPS_INSTALL_PREFIX/lib

# repair kmymoney for plugins
kmymoney_findmissinglibs

createDMG

ls -lh $KMYMONEY_INSTALL_PREFIX/*
ls -lh $CMAKE_BUILD_PREFIX/*

# cd $CMAKE_BUILD_PREFIX
# git clone https://github.com/arl/macdeployqtfix.git
# cd $CMAKE_BUILD_PREFIX/macdeployqtfix
# python macdeployqtfix.py -v $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents/MacOS/kmymoney $QT_DIR
#
# cd $QT_DIR/bin
# macdeployqt $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app -dmg -verbose=2
#
# if [ -f $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.dmg ]; then
#   mv $KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.dmg $CMAKE_BUILD_PREFIX/kmymoney-$VERSION-x86_64.dmg
# fi
