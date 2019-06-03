#!/bin/bash
#
# Build DmgImage of KMyMoney on MacOS High Sierra.

# Halt on errors and be verbose about what we are doing
set -eu

# Switch directory in order to put all build files in the right place
cd $CMAKE_BUILD_PREFIX

QT_DIR=$DEPS_INSTALL_PREFIX

APPDIR=$KMYMONEY_INSTALL_PREFIX/Applications/KDE
CONTENTSDIR=$KMYMONEY_INSTALL_PREFIX/Applications/KDE/kmymoney.app/Contents

mkdir -p $CONTENTSDIR/PlugIns
mkdir -p $CONTENTSDIR/Frameworks

sourceLibPaths=(
$DEPS_INSTALL_PREFIX/lib
$DEPS_INSTALL_PREFIX/colisionlibs/lib
)

substitute_with_rpaths () {
  local libFiles=(${@})
  for libFile in ${libFiles[@]}; do
    local isSubstituted=false
    local needed_libs_for_lib=($(otool -XL ${libFile} | awk '{print $1}'))

    # get rid of library's id prefix
    local libID=$(otool -XD ${libFile})
    for souceLibPath in ${sourceLibPaths[@]}; do
      if [ "${libID:0:${#souceLibPath}}" == "${souceLibPath}" ]; then
        install_name_tool -id ${libID##*/} "${libFile}"
        isSubstituted=true
      fi
    done

    # replace library's depenencies prefix with @rpath
    for lib in ${needed_libs_for_lib[@]:0}; do
      for souceLibPath in ${sourceLibPaths[@]}; do
        if [ ${lib:0:1} != "@" ] && [ "${lib:0:${#souceLibPath}}" == "${souceLibPath}" ]; then
          local isSubstituted=true
          install_name_tool -change ${lib} "@rpath${lib:${#souceLibPath}}" "${libFile}"
          break
        fi
      done
    done
    if [ $isSubstituted == true ]; then
      local needed_libs_for_lib=($(otool -L ${libFile} | awk '{print $1}'))
      echo " rpaths substituted for ${libFile##*/} and now look like that:" >&2
      printf '   %s\n' "${needed_libs_for_lib[@]:0}" >&2
      printf '   \n' >&2
    fi
  done
}

find_needed_libs () {
  echo "Analizing libraries with oTool..." >&2
  local needed_libs=() # input lib_lists founded
  local needed_libs_for_lib=()
  local libFiles=(${@})
  for libFile in ${libFiles[@]}; do
    echo ${libFile} >&2

    needed_libs_for_lib=($(otool -XL ${libFile} | awk '{print $1}'))

    # frameworks and dylibs have itself at first row, so omit that
    if [[ ${libFile} == *".dylib" ]] || [[ ${libFile} == *".framework"* ]]; then
      needed_libs+=(${needed_libs_for_lib[@]:1})
    else
      needed_libs+=(${needed_libs_for_lib[@]:0})
    fi

  done

  if [ ${#needed_libs[@]} -gt 0 ]; then
    echo ${needed_libs[@]}
  fi
}

find_missing_libs (){
  echo "Searching for missing libs on deployment folders…" >&2
  local missing_libs=()
  # filter out libraies that are already in the appdir or are system ones
  local needed_libs=($(printf '%s\n' "${@}" |
      sort -u |
      sed '/\/usr\/lib\//d' |
      sed '/..\/Frameworks/d' |
      sed '/..\/PlugIns/d' |
      sed '/..\/MacOS/d'
      ))

  for lib in ${needed_libs[@]:0}; do
    for souceLibPath in ${sourceLibPaths[@]}; do

      # expand placeholder paths, because that may uncover missing depenent library
      pathsToExpand=(@rpath @loader_path)
      for pathToExpand in ${pathsToExpand[@]}; do
        if [[ ${lib} == ${pathToExpand}* ]]; then
          if [ -f "${souceLibPath}/${lib##${pathToExpand}/}" ]; then
            lib="${souceLibPath}/${lib##${pathToExpand}/}"
            break
          fi
        fi
      done

      if [ "${lib:0:${#souceLibPath}}" == "${souceLibPath}" ]; then
        # see if the library is realy missing or only has out of appdir reference
        if [ ! -f  "${CONTENTSDIR}/Frameworks${lib:${#souceLibPath}}" ]; then
          echo "Adding ${lib##*/} to missing libraries." >&2
          missing_libs+=("${lib}")
        fi
        break
      # don't report libraries which failed at placeholder path expansion
      elif [[ ${lib} !=  @* ]]; then
        echo "Library from unexpected source ${lib}." >&2
      fi

    done
  done

  if [ ${#missing_libs[@]} -gt 0 ]; then
    echo ${missing_libs[@]}
  fi
}

copy_missing_libs () {

  for lib in ${@}; do
    if [[ $lib  == *".framework"* ]]; then
      echo "copying missing framework ${lib##*/}" >&2
      frameworkBaseName=$(basename ${lib})
      frameworkDirName="${lib%%.framework/*}.framework"
      rsync -priul ${frameworkDirName}/${frameworkBaseName} ${CONTENTSDIR}/Frameworks/${frameworkBaseName}.framework/
      rsync -priul ${frameworkDirName}/Resources ${CONTENTSDIR}/Frameworks/${frameworkBaseName}.framework/
      rsync -priul ${frameworkDirName}/Versions ${CONTENTSDIR}/Frameworks/${frameworkBaseName}.framework/
      rm -fr ${CONTENTSDIR}/Frameworks/${frameworkBaseName}.framework/Versions/*/Headers
    else
      echo "copying ${lib##*/} to Frameworks dir" >&2
      cp -pv ${lib} ${CONTENTSDIR}/Frameworks
    fi
  done
}

kmymoney_findmissinglibs() {
  echo "Starting search for missing libraries"
  local needed_libs=()
  local missing_libs=()
  local missing_libs_partial=()
  # start searching for missing libraries based on libraries already in the appdir
  local libFiles=$(find ${CONTENTSDIR} -type f \( -name "*.so" -or -name "*.dylib" \))
  local macOSFiles=$(find $CONTENTSDIR/MacOS -type f -and \( -perm +111 -and  ! -name "*.*"  \))
  libFiles+=" ${macOSFiles[*]}"
  while [ true ]; do

    needed_libs=($(find_needed_libs ${libFiles[@]}))
    missing_libs_partial=($(find_missing_libs ${needed_libs[@]}))
    # break only if there is no missing libraries left
    if [ ${#missing_libs_partial[@]} -eq 0 ]; then
      break
    else
      missing_libs+=(${missing_libs_partial[@]})
      libFiles=(${missing_libs_partial[@]}) # see if missing libraries also have some missing libraries
    fi
  done

  if [ ${#missing_libs[@]} -gt 0 ]; then
      echo "Found missing libs!"
      missing_libs=($(printf '%s\n' "${missing_libs[@]}" | sort -u))
      copy_missing_libs ${missing_libs[@]}
  else
      echo "No missing libraries found."
  fi
  libFiles=$(find ${CONTENTSDIR} -type f \( -name "*.so" -or -name "*.dylib" \))
  macOSFiles=$(find $CONTENTSDIR/MacOS -type f -and \( -perm +111 -and  ! -name "*.*"  \))
  libFiles+=" ${macOSFiles[*]}"
  substitute_with_rpaths ${libFiles}
  echo "Done!"
}

createDMG () {
  echo "Starting creation of dmg..."
  cd $CMAKE_BUILD_PREFIX
  DMG_size=500
  DMG_title=KMyMoneyNEXT
  DMG_background=background.png

  ## Build dmg from folder

  # create dmg on local system
  # usage of -fsargs minimze gaps at front of filesystem (reduce size)
  hdiutil create -srcfolder "${APPDIR}" -volname "${DMG_title}" -fs HFS+ \
      -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${DMG_size}m kmymoney.temp.dmg

  device=$(hdiutil attach -readwrite -noverify -noautoopen "kmymoney.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')

  # Set style for dmg
  if [[ ! -d "/Volumes/${DMG_title}/.background" ]]; then
      mkdir "/Volumes/${DMG_title}/.background"
  fi

#   cp -fv "${KMYMONEY_SOURCES}/packaging/osx/dmgimage/KMyMoneyNEXTIcon.icns" "/Volumes/${DMG_title}/.VolumeIcon.icns"
#   SetFile -c icnC "/Volumes/${DMG_title}/.VolumeIcon.icns"
#   SetFile -a C "/Volumes/${DMG_title}"
#   cp -rv "${KMYMONEY_SOURCES}/packaging/osx/dmgimage/DBus HOWTO.txt" "/Volumes/${DMG_title}/DBus HOWTO.txt"

  cp -rv "${KMYMONEY_SOURCES}/packaging/osx/dmgimage/${DMG_background}" "/Volumes/${DMG_title}/.background/${DMG_background}"
  ln -s "/Applications" "/Volumes/${DMG_title}/Applications"
  ## Apple script to set style
  echo "Applying style"
  echo '
      tell application "Finder"
          tell disk "KMyMoneyNEXT"
              open
              delay 10
              close
          end tell
      end tell
      ' | osascript

#         echo '
#       tell application "Finder"
#           tell disk "'${DMG_title}'"
#               open
#               set current view of container window to icon view
#               set toolbar visible of container window to false
#               set statusbar visible of container window to false
#               set the bounds of container window to {200, 200, (200 + 350), (200 + 200)}
#               set theViewOptions to the icon view options of container window
#               set arrangement of theViewOptions to not arranged
#               set icon size of theViewOptions to 80
#               set background picture of theViewOptions to file ".background:'${DMG_background}'"
#               set position of item "kmymoney.app" of container window to {0, 0}
#               set position of item "Applications" of container window to {100, 0}
#               update without registering applications
#               delay 1
#               close
#           end tell
#       end tell
#       ' | osascript

  echo "Changing image permissions"
  chmod -Rf go-w "/Volumes/${DMG_title}"

  # ensure all writting operations to dmg are over
  sync

  echo "Detaching image"
  hdiutil detach $device
  echo "Compressing image"
  hdiutil convert kmymoney.temp.dmg -format ULFO -o kmymoney-out.dmg

  # Add git version number

  USE_GIT=true
  if [ $USE_GIT == true ]; then
    cd $KMYMONEY_SOURCES
    KMYMONEY_VERSION=$(grep "KMyMoney VERSION" CMakeLists.txt | cut -d '"' -f 2)

    # Also find out the revision of Git we built
    # Then use that to generate a combined name we'll distribute
    if [ -d .git ]; then
      GIT_REVISION=$(git rev-parse --short HEAD)
      export VERSION=$KMYMONEY_VERSION-${GIT_REVISION:0:7}
    else
      export VERSION=$KMYMONEY_VERSION
    fi
    DMG_NAME=KMyMoneyNEXT-$VERSION-x86_64.dmg
  else
    DMG_NAME="$(date '+%Y-%m-%d')-KMyMoneyNEXT-x86_64.dmg"
  fi

  cd $CMAKE_BUILD_PREFIX
  mv kmymoney-out.dmg ${DMG_NAME}
  echo "moved kmymoney-out.dmg to ${DMG_NAME}"
  rm kmymoney.temp.dmg

  echo "dmg done!"
}

echo "Copying libs..."
rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/*.dylib $CONTENTSDIR/Frameworks

echo "Copying share..."
rsync -prul $KMYMONEY_INSTALL_PREFIX/share/* $CONTENTSDIR/Resources
if [ -d $CONTENTSDIR/Resources/kmymoney ]; then
  # It's because QStandardPaths::DataLocation return <APPDIR>/../Resources and not <APPDIR>/../Resources/<APPNAME> like in case of other OSes
  rsync -prul $CONTENTSDIR/Resources/kmymoney/* $CONTENTSDIR/Resources
  rm -fr $CONTENTSDIR/Resources/kmymoney
fi

rsync -prul $DEPS_INSTALL_PREFIX/share/kservicetypes5/kcm* $CONTENTSDIR/Resources/kservicetypes5
cp -pv $DEPS_INSTALL_PREFIX/share/icons/breeze/breeze-icons.rcc $CONTENTSDIR/Resources/icontheme.rcc

echo "Copying plugins..."
mkdir -p $CONTENTSDIR/PlugIns/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/file* $CONTENTSDIR/PlugIns/kf5/kio
cp -fpv $DEPS_INSTALL_PREFIX/plugins/kf5/kio/http* $CONTENTSDIR/PlugIns/kf5/kio
rsync -prul $DEPS_INSTALL_PREFIX/plugins/sqldrivers $CONTENTSDIR/PlugIns
rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/plugins/kmymoney $CONTENTSDIR/PlugIns

if [ -d $KMYMONEY_INSTALL_PREFIX/lib/plugins/sqldrivers ]; then
  echo "Copying SQLCipher..."
  rsync -prul $KMYMONEY_INSTALL_PREFIX/lib/plugins/sqldrivers $CONTENTSDIR/PlugIns

  echo "Copying qsqlcipher for tests..."
  cp -fv $KMYMONEY_INSTALL_PREFIX/lib/plugins/sqldrivers/qsqlcipher* $DEPS_INSTALL_PREFIX/plugins/sqldrivers
fi

if [ -d $DEPS_INSTALL_PREFIX/plugins/mariadb ]; then
  echo "Copying MariaDB..."
  rsync -prul $DEPS_INSTALL_PREFIX/plugins/mariadb $CONTENTSDIR/PlugIns
fi

if [ -d $DEPS_INSTALL_PREFIX/share/dbus-1 ]; then
  echo "Copying DBus..."
  rsync -prul $DEPS_INSTALL_PREFIX/bin/dbus* $CONTENTSDIR/MacOS
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libdbus-1* $CONTENTSDIR/Frameworks
  rsync -prul $DEPS_INSTALL_PREFIX/share/dbus-1 $CONTENTSDIR/Resources
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libexec/kf5/kioslave $CONTENTSDIR/MacOS
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libexec/kf5/klauncher $CONTENTSDIR/MacOS
  rsync -prul $DEPS_INSTALL_PREFIX/bin/kdeinit5 $CONTENTSDIR/MacOS
  mkdir -p $CONTENTSDIR/Library/LaunchAgents
  rsync -prul $DEPS_INSTALL_PREFIX/Library/LaunchAgents $CONTENTSDIR/Library
  echo "Patching org.freedesktop.dbus-session.plist.org ..."
  installDMGPath=/Applications/kmymoney.app/Contents
  buildDMGPath=$(grep bin/dbus-daemon $CONTENTSDIR/Library/LaunchAgents/org.freedesktop.dbus-session.plist)
  buildDMGPath=${buildDMGPath#*>}
  buildDMGPath=${buildDMGPath%/bin/dbus-daemon*}
  sed -i '' \
        -e s+$buildDMGPath/bin+$installDMGPath/MacOS+g \
        -e s+--session+--config-file=$installDMGPath/Resources/dbus-1/session.conf+g \
        $CONTENTSDIR/Library/LaunchAgents/org.freedesktop.dbus-session.plist
fi

if [ -f $DEPS_INSTALL_PREFIX/lib/libKF5KHtml.dylib ]; then
  echo "Copying KF5KHtml..."
  rsync -prul $DEPS_INSTALL_PREFIX/lib/libKF5KHtml* $CONTENTSDIR/Frameworks
  install_name_tool -change libgif.7.dylib "@rpath/libgif.7.dylib" "$CONTENTSDIR/Frameworks/libKF5KHtml.dylib"
  rsync -prul $DEPS_INSTALL_PREFIX/share/kf5/khtml $CONTENTSDIR/Resources/kf5
  rsync -prul $DEPS_INSTALL_PREFIX/share/kservicetypes5/qimageio* $CONTENTSDIR/Resources/kservicetypes5
fi

if [ -d $DEPS_INSTALL_PREFIX/share/aqbanking ]; then
  echo "Copying aqbanking and gwenhywfar..."
  rsync -prul $DEPS_INSTALL_PREFIX/share/aqbanking $CONTENTSDIR/Resources
  rsync -prul $DEPS_INSTALL_PREFIX/share/gwenhywfar $CONTENTSDIR/Resources
  rsync -prul $DEPS_INSTALL_PREFIX/share/ktoblzcheck $CONTENTSDIR/Resources
  rsync -prul $DEPS_INSTALL_PREFIX/lib/gwenhywfar/plugins/60/* $CONTENTSDIR/PlugIns/gwenhywfar
  rsync -prul $DEPS_INSTALL_PREFIX/lib/aqbanking/plugins/35/* $CONTENTSDIR/PlugIns/aqbanking
fi

cd $CMAKE_BUILD_PREFIX

# withouth rpath no dependent library will be loaded
macOSFiles=$(find $CONTENTSDIR/MacOS -type f -and \( -perm +111 -and  ! -name "*.*"  \))
for macOSFile in $macOSFiles; do
  install_name_tool -add_rpath @loader_path/../Frameworks $macOSFile
done

cd $DEPS_INSTALL_PREFIX/bin
macdeployqt $APPDIR/kmymoney.app \
           -verbose=1 \
           -executable=${CONTENTSDIR}/MacOS/kmymoney \
           -qmldir=$DEPS_INSTALL_PREFIX/qml \
           -libpath=$DEPS_INSTALL_PREFIX/lib

kmymoney_findmissinglibs

# Remove redundant files and directories
cd $CONTENTSDIR
rm -fr $CONTENTSDIR/Resources/icons/oxygen
rm -fr $CONTENTSDIR/Resources/icons/Tango
rm -fr $CONTENTSDIR/Frameworks/*Test*
find . \( -type f \( -name *.a -or -name *.la \) \) -exec rm {} \;

# Strip libraries
find . \( -type f \( -name *.dylib -or -name *.so -or -name Qt* -or -name kmymoney \) \) -exec strip -Sx {} \;

createDMG