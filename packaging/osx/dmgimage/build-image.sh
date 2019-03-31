#!/bin/bash
#
# Build DmgImage of KMyMoney on MacOS High Sierra.

# Halt on errors and be verbose about what we are doing
set -eu

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

QT_DIR=$DEPS_INSTALL_PREFIX

KMYMONEY_DMG=$KMYMONEY_INSTALL_PREFIX/Applications/KDE

mkdir -p $KMYMONEY_DMG/kmymoney.app/Contents/PlugIns
mkdir -p $KMYMONEY_DMG/kmymoney.app/Contents/Frameworks

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
$DEPS_INSTALL_PREFIX/lib
)

# Find all @rpath and Absolute to buildroot path libs
# Add to libs_used
# converts absolute buildroot path to @rpath
find_needed_libs () {
    echo "Analizing libraries with oTool..." >&2
    local libs_used="" # input lib_lists founded
    for libFile in $(find ${KMYMONEY_DMG}/kmymoney.app/Contents -type f -name "*.so" -or -type f -name "*.dylib" -or -type f -name "kmymoney"); do
        oToolResult=$(otool -L ${libFile} | awk '{print $1}')
        resultArray=(${oToolResult}) # convert to array
        echo "${libFile##*Contents/}" >&2
        for lib in ${resultArray[@]:1}; do
            if test "${lib:0:1}" = "@"; then
                local libs_used=$(add_lib_to_list ${lib} "${libs_used}")
            fi

            for souceLibPath in ${sourceLibPaths[@]}; do
                if test "${lib:0:${#souceLibPath}}" = "${souceLibPath}"; then
                    install_name_tool -id ${lib##*/} "${libFile}"
                    install_name_tool -change ${lib} "@rpath${lib:${#souceLibPath}}" "${libFile}"
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
        if test -z "$(find ${KMYMONEY_DMG}/kmymoney.app/Contents/ -name ${lib})"; then
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
#        result=$(find "$DEPS_INSTALL_PREFIX" -name "${lib}")

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

                for souceLibPath in ${sourceLibPaths[@]}; do
                    if test "${result:0:${#souceLibPath}}" = "${souceLibPath}"; then
                        break
                    fi
                done

                echo "copying framework $souceLibPath/${lib}.framework to dmg"
                # TODO rsync only included ${lib} Resources Versions
                rsync -priul $souceLibPath/${lib}.framework/${lib} ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
                rsync -priul $souceLibPath/${lib}.framework/Resources ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
                rsync -priul $souceLibPath/${lib}.framework/Versions ${KMYMONEY_DMG}/kmymoney.app/Contents/Frameworks/${lib}.framework/
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
    DMG_title=kmymoney
    DMG_background=background.png

    ## Build dmg from folder

    # create dmg on local system
    # usage of -fsargs minimze gaps at front of filesystem (reduce size)
    hdiutil create -srcfolder "${KMYMONEY_DMG}" -volname "${DMG_title}" -fs HFS+ \
        -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${DMG_size}m kmymoney.temp.dmg

    device=$(hdiutil attach -readwrite -noverify -noautoopen "kmymoney.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

    # Set style for dmg
    if [[ ! -d "/Volumes/${DMG_title}/.background" ]]; then
        mkdir "/Volumes/${DMG_title}/.background"
    fi

    cp ${KMYMONEY_SOURCES}/kmymoney/pics/${DMG_background} "/Volumes/${DMG_title}/.background/"

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
    hdiutil convert kmymoney.temp.dmg -format ULFO -o kmymoney-out.dmg

    # Add git version number
    mv kmymoney-out.dmg kmymoney-$VERSION-x86_64.dmg
    echo "moved kmymoney-out.dmg to kmymoney-$VERSION-x86_64.dmg"
    rm kmymoney.temp.dmg

    echo "dmg done!"
}

echo "Copying libs..."
rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/* \
            --exclude plugins \
            $KMYMONEY_DMG/kmymoney.app/Contents/Frameworks

rsync -prul $DEPS_INSTALL_PREFIX/lib/libphonon4qt5* $KMYMONEY_DMG/kmymoney.app/Contents/Frameworks
rsync -prul $DEPS_INSTALL_PREFIX/lib/libKF5Notifications* $KMYMONEY_DMG/kmymoney.app/Contents/Frameworks

echo "Copying share..."
rsync -prul $KMYMONEY_INSTALL_PREFIX/share/* $KMYMONEY_DMG/kmymoney.app/Contents/Resources
rsync -prul $DEPS_INSTALL_PREFIX/share/* \
            --exclude aclocal \
            --exclude doc \
            --exclude ECM \
            --exclude eigen3 \
            --exclude emacs \
            --exclude gettext \
            --exclude gettext-0.19.8 \
            --exclude info \
            --exclude man \
            --exclude ocio \
            --exclude pkgconfig \
            --exclude mime \
            --exclude translations \
            --exclude qml \
            --exclude locale \
            --exclude terminfo \
            --exclude gtk-doc \
            --exclude bison \
            --exclude icu \
            --exclude kf5 \
            --exclude tabset \
            --exclude phonon4qt5 \
            --exclude knotifications5 \
            --exclude common-lisp \
            --exclude dbus-1 \
            $KMYMONEY_DMG/kmymoney.app/Contents/Resources
rsync -prul $KMYMONEY_INSTALL_PREFIX/share/kmymoney/* $KMYMONEY_DMG/kmymoney.app/Contents/Resources
cp $DEPS_INSTALL_PREFIX/share/icons/breeze/breeze-icons.rcc $KMYMONEY_DMG/kmymoney.app/Contents/Resources/icontheme.rcc
rm -fr $KMYMONEY_DMG/kmymoney.app/Contents/Resources/icons/breeze
rm -fr $KMYMONEY_DMG/kmymoney.app/Contents/Resources/icons/breeze-dark

echo "Copying plugins..."
# rsync -prul $DEPS_INSTALL_PREFIX/lib/plugins/* $KMYMONEY_DMG/kmymoney.app/Contents/PlugIns
rsync -prul $DEPS_INSTALL_PREFIX/plugins/* \
            --exclude geoservices \
            --exclude qmltooling \
            --exclude kcm_kio.so \
            --exclude kcm_trash.so \
            --exclude kcm_webshortcuts.so \
            $KMYMONEY_DMG/kmymoney.app/Contents/PlugIns
rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/plugins/kmymoney $KMYMONEY_DMG/kmymoney.app/Contents/PlugIns


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

# Step 7: Build the image!!!
install_name_tool -add_rpath @loader_path/../Frameworks $KMYMONEY_DMG/kmymoney.app/Contents/MacOS/kmymoney
cd $DEPS_INSTALL_PREFIX/bin
macdeployqt $KMYMONEY_DMG/kmymoney.app \
            -verbose=1 \
            -executable=${KMYMONEY_DMG}/kmymoney.app/Contents/MacOS/kmymoney \
            -qmldir=$DEPS_INSTALL_PREFIX/qml \
            -libpath=$DEPS_INSTALL_PREFIX/lib

macdeployqt $KMYMONEY_DMG/kmymoney.app \
            -verbose=1 \
            -executable=${KMYMONEY_DMG}/kmymoney.app/Contents/MacOS/kmymoney \
            -qmldir=$DEPS_INSTALL_PREFIX/qml \
            -libpath=$DEPS_INSTALL_PREFIX/lib

# repair kmymoney for plugins
kmymoney_findmissinglibs
kmymoney_findmissinglibs

createDMG