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

# qtquickcontrols and qtwebchannel are required by qtwebengine
# "WebRTC" is required by "Pepper Plugins" is required by "Printing and PDF"
# don't use -I and -L switches as they will cause explicit link to
# OpenSSL instead of BoringSSL in QWebEngine
# kxmlgui uses QSslSocket and fails to build without that, so openssl must be present

# Tested disables:
#  -no-feature-animation # KF5Completion requires that
#  -no-feature-movie # KF5WidgetsAddons requires that
#  -no-feature-networkinterface # otherwise 'QNetworkInterface' does not name a type in qnativesocketengine_win.cpp
#  -no-feature-tabletevent # KItemViews reports "incomplete type 'class QTabletEvent'"
#  -no-feature-xmlstream # qvkgen and qdbusxmlparser needs QXmlStreamReader
#  -no-feature-localserver # required by something
#  -no-feature-bearermanagement # required by KIO for QNetworkConfigurationManager
#  -no-feature-texthtmlparser # required by QtTools which uses QTextBrowser::setHtml
#  -no-feature-cssparser # required by KWidgetsAddons for QLabel::setStyleSheet
#  -no-style-windows # required by KWidgetsAddons for QLabel::setStyleSheet (it's a condition to enable style-stylesheet)
#  -no-feature-networkproxy # required by KIO for QNetworkProxy::setApplicationProxy
#  -no-feature-socks5 # required by KIO for QNetworkProxy::Socks5Proxy
#  -no-doubleconversion # "Your C library does not provide sscanf_l or snprintf_l" at the end of configuration
#  -no-feature-easingcurve # required by QQmlXmlListModel
#  -no-graphicsview # required by KDiagram
#  -no-feature-future # required by KIO
#  -no-feature-mdiarea # required by QtQuickWidgets
#  -no-feature-udpsocket # required by kdelibs4support
#  -no-feature-calendarwidget # required by datetimeedit which is required by printdialog which is required by kxmlgui

if (APPLE)

  set(platform_specific_options -macos-additional-datadirs ${CMAKE_INSTALL_PREFIX}/share
                                                  -qt-freetype # otherwise system's freetype gets bundled with macdeployqt and it depends on wrong libpng
                                                  -no-feature-corewlan
                                                  )
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qstandardpaths-extra-dirs.patch
                              COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix_GenericDataLocation_mac.patch
        )
elseif(MINGW)
  set(platform_specific_options -platform win32-g++ -opengl desktop)
  set(platform_specific_patch "")
elseif(UNIX)
  set(platform_specific_options -xcb # otherwise no QtX11Extras
                                -fontconfig # otherwise no font
                                -cups
                                CUPS_PREFIX=${CMAKE_INSTALL_PREFIX}
                                -I ${CMAKE_INSTALL_PREFIX}/include #otherwise cups/ppd.h: No such file or directory
                                )
  set(platform_specific_patch "")
endif()

if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_qtbase
  ${PROJECT_GLOBAL_PROFILE}

  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtbase-everywhere-src-5.15.0.tar.xz
  URL_MD5 6be4d7ae4cd0d75c50b452cc05117009
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/search-for-release-dbus.diff
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qdbus-manager-quit-5.7.patch
        ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    -prefix ${CMAKE_INSTALL_PREFIX}
    -opensource
    -confirm-license
    -verbose
    -release
    -optimize-size
    -strip
    -shared
    -nomake examples
    -nomake tests
    -nomake tools
    -pkg-config
    -dbus
    -dbus-runtime
    -no-glib
    -icu
    -ssl
    -openssl-linked
    -no-sql-odbc
    -sql-psql
    -sql-mysql
    -system-sqlite
    -system-pcre
    -no-dtls
    -no-ocsp
    -no-feature-textodfwriter
    -feature-textmarkdownwriter # required by QtDeclarative for QTextEdit::setMarkdownText
    -feature-textmarkdownreader # required by QtDeclarative for QTextEdit::setMarkdownText
    -qt-libmd4c
    -feature-printdialog # required by kxmlgui
    -no-feature-style-windowsvista
    -no-feature-statemachine
    -no-feature-gestures
    -no-feature-ftp
    -no-feature-lcdnumber
    -no-feature-im
    -no-feature-dnslookup
    -no-feature-networkdiskcache
    -no-feature-topleveldomain
    -no-feature-sspi
    -no-feature-gssapi
    -no-feature-valgrind
    -no-feature-system-proxies
    -no-feature-islamiccivilcalendar
    -no-feature-hijricalendar
    -no-feature-jalalicalendar
    -no-feature-journald
    -no-feature-syslog
    -no-feature-vkgen
    -no-feature-vulkan
    -no-feature-vnc
    -no-feature-vsp2
    -no-feature-simulator_and_device
    -no-feature-avx2
    -no-feature-dial
    -no-feature-graphicseffect
    -no-feature-datawidgetmapper
    -no-feature-commandlinkbutton
    -no-feature-netlistmgr
    -no-feature-big_codecs # remember to include in ICU: windows-950-2000 ibm-1375_P100-2003
    -no-feature-codecs # remember to include in ICU all the codecs mentioned at https://doc.qt.io/qt-5/qtextcodec.html
    -no-feature-lttng
    -no-feature-qeventtransition
    -no-feature-renameat2
    -no-openvg
    GLIB_PREFIX=${CMAKE_INSTALL_PREFIX}
    ICU_PREFIX=${CMAKE_INSTALL_PREFIX}
    OPENSSL_PREFIX=${CMAKE_INSTALL_PREFIX}
    ZLIB_PREFIX=${CMAKE_INSTALL_PREFIX}
    LIBPNG_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
    LIBJPEG_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
    DBUS_PREFIX=${CMAKE_INSTALL_PREFIX}
    SQLITE_PREFIX=${CMAKE_INSTALL_PREFIX}${install_suffix}
    PCRE2_PREFIX=${CMAKE_INSTALL_PREFIX}
    PSQL_PREFIX=${CMAKE_INSTALL_PREFIX}
    MYSQL_INCDIR=${CMAKE_INSTALL_PREFIX}/include/mariadb
    MYSQL_LIBDIR=${CMAKE_INSTALL_PREFIX}/lib
    MYSQL_LIBS="-lmariadb" # mingw used  MYSQL_LIBS="-llibmariadb" but MacOS fails on this
    ${platform_specific_options}

  ${CLEANUP_COMMAND}
  DEPENDS ext_dbus ext_pcre2 ext_icu ext_openssl ext_mariadb ext_postgresql ext_sqlite ext_png ext_jpeg
)

ExternalProject_Add(
  ext_qtsvg
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtsvg-everywhere-src-5.15.0.tar.xz
  URL_MD5 e982467c5f31b810c690f97a6ea33215

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qttools
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qttools-everywhere-src-5.15.0.tar.xz
  URL_MD5 cbf72f34c9d53508f95b9a4db5322ffa

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> -- -no-feature-qdoc
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtlocation
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtlocation-everywhere-src-5.15.0.tar.xz
  URL_MD5 5896e9e7844c54b47cfaf759b3d1a879

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtxmlpatterns
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtxmlpatterns-everywhere-src-5.15.0.tar.xz
  URL_MD5 47082f6a346300eb99aede54d87cb55c

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtspeech
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtspeech-everywhere-src-5.15.0.tar.xz
  URL_MD5 e4939722b84d5fff0ac0175dd57fae30

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

# Tested disables:
#  -no-qml-debug # required by KDeclarative
#  -no-feature-qml-animation # otherwise QtQuick will not be built
#  -no-feature-qml-locale # otherwise QtQuick2 will not be built
#  -no-feature-qml-network # required by KDeclarative for QQmlEngine::setNetworkAccessManagerFactory
#  -no-feature-qml-devtools required for qmlimportscanner to make dmg image on macOS

ExternalProject_Add(
  ext_qtdeclarative
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtdeclarative-everywhere-src-5.15.0.tar.xz
  URL_MD5 4f8eb9c5d9c7ceadd52c97eee81d0ad3

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> --
                    -no-feature-qml-jit
                    -no-feature-qml-preview
                    -no-feature-qml-profiler
                    -no-feature-qml-sequence-object
                    -no-feature-qml-worker-script
                    -no-feature-qml-xml-http-request
                    -no-feature-quick-designer
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtxmlpatterns ext_qtsvg
)

ExternalProject_Add(
  ext_qtquickcontrols
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtquickcontrols-everywhere-src-5.15.0.tar.xz
  URL_MD5 9851969f106699f5c6a6446e8b8e2008

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtquickcontrols2
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtquickcontrols2-everywhere-src-5.15.0.tar.xz
  URL_MD5 f781ef93700abb89bf97b88a15587191

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> --
                    -no-feature-quicktemplates2-multitouch
                    -no-feature-quicktemplates2-hover
                    -no-feature-quickcontrols2-imagine
                    -no-feature-quickcontrols2-material
                    -no-feature-quickcontrols2-universal
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtx11extras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtx11extras-everywhere-src-5.15.0.tar.xz
  URL_MD5 8f2880b5ad693b4c4e98e307ac3f7b2e

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtwinextras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtwinextras-everywhere-src-5.15.0.tar.xz
  URL_MD5 83d82192eaf67036ba97d1def754d510

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtmacextras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtmacextras-everywhere-src-5.15.0.tar.xz
  URL_MD5 471239e6569093669039cd5f3f89e498

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtwebchannel
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtwebchannel-everywhere-src-5.15.0.tar.xz
  URL_MD5 3b38221f05415a941649ecc5a14034bc

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtwebengine
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.15/5.15.0/submodules/qtwebengine-everywhere-src-5.15.0.tar.xz
  URL_MD5 738478b9409f8615ca0f63738b73bbae

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> #-- -system-webengine-icu #-no-webengine-spellchecker -no-webengine-proprietary-codecs -no-webengine-webrtc -no-webengine-pepper-plugins
  ${CLEANUP_COMMAND}

  DEPENDS ext_xml ext_qtbase ext_qtwebchannel
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_qtdeclarative correctcmake
    COMMAND bash -c "find . -type f -name Qt5\\*.pc -exec sed -i \
    -e 's/Qt5\\(.*\\)d$*/Qt5\\1/g' \
    -e 's/Wigets/Widgets/g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
  )
endif()
