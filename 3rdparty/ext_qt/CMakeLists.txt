# qtquickcontrols and qtwebchannel are required by qtwebengine
# "WebRTC" is required by "Pepper Plugins" is required by "Printing and PDF"
# don't use -I and -L switches as they will cause explicit link to
# OpenSSL instead of BoringSSL in QWebEngine

# kxmlgui uses QSslSocket and fails to build without that, so openssl must be present

if (APPLE)
  set(qt_platform_specific_option -macos-additional-datadirs ${CMAKE_INSTALL_PREFIX}/share)
elseif(MINGW)
  set(qt_platform_specific_option -platform win32-g++ -opengl desktop)
elseif(UNIX)
  set(qt_platform_specific_option -xcb -macos-additional-datadirs ${CMAKE_INSTALL_PREFIX}/share)
endif ()

ExternalProject_Add(
  ext_qtbase
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtbase-everywhere-src-5.12.3.tar.xz
  URL_MD5 fe47852113fc27581dbc0cf71644d6f1
 PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qstandardpaths-extra-dirs.patch
       COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix_GenericDataLocation_mac.patch
       COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qdbus-manager-quit-5.7.patch

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    -prefix ${CMAKE_INSTALL_PREFIX}
    -opensource
    -confirm-license
    -verbose
    -release
    -shared
    -no-static
    -nomake examples
    -nomake tests
    -nomake tools
    -pkg-config
    -icu
    -ssl
    -openssl-runtime
    -dbus
    -dbus-runtime
    -no-sql-odbc
    -sql-psql
    -sql-mysql
    -system-sqlite
    ICU_PREFIX=${CMAKE_INSTALL_PREFIX}
    OPENSSL_PREFIX=${CMAKE_INSTALL_PREFIX}
    ZLIB_PREFIX=${CMAKE_INSTALL_PREFIX}
    LIBPNG_PREFIX=${CMAKE_INSTALL_PREFIX}
    LIBJPEG_PREFIX=${CMAKE_INSTALL_PREFIX}
    DBUS_PREFIX=${CMAKE_INSTALL_PREFIX}
    SQLITE_PREFIX=${CMAKE_INSTALL_PREFIX}
    PSQL_PREFIX=${CMAKE_INSTALL_PREFIX}
    MYSQL_INCDIR=${CMAKE_INSTALL_PREFIX}/include/mariadb
    MYSQL_LIBDIR=${CMAKE_INSTALL_PREFIX}/lib
    MYSQL_LIBS="-llibmariadb"

    ${qt_platform_specific_option}

  ${CLEANUP_COMMAND}
  DEPENDS ext_openssl ext_sqlite ext_icu ext_png ext_jpeg ext_mariadb ext_postgresql
)


ExternalProject_Add(
  ext_qtsvg
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtsvg-everywhere-src-5.12.3.tar.xz
  URL_MD5 f8fa6e39584ed631f2795d8d80d5229c

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qttools
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qttools-everywhere-src-5.12.3.tar.xz
  URL_MD5 d2e4776ca7ca1bbb39330adffa17a65f

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtlocation
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtlocation-everywhere-src-5.12.3.tar.xz
  URL_MD5 0ccaa6be759513650158d552c6130529

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtxmlpatterns
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtxmlpatterns-everywhere-src-5.12.3.tar.xz
  URL_MD5 f1f6b4c1253d2b833e849663c7661110

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtspeech
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtspeech-everywhere-src-5.12.3.tar.xz
  URL_MD5 1aa38a37cd52a4a0403fc41715992070

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

# kdeclarative uses QQmlDebuggingEnabler, so -no-qml-debug cannot be passed
ExternalProject_Add(
  ext_qtdeclarative
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtdeclarative-everywhere-src-5.12.3.tar.xz
  URL_MD5 2f8e8a30507adbec8f4c10fa56b06027

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtxmlpatterns ext_qtsvg
)

ExternalProject_Add(
  ext_qtx11extras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtx11extras-everywhere-src-5.12.3.tar.xz
  URL_MD5 4069ca615b14c0c2b5be8744c9a75633

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwinextras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtwinextras-everywhere-src-5.12.3.tar.xz
  URL_MD5 1e10104f161cccecbb9c1c2638b1a719

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtmacextras
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtmacextras-everywhere-src-5.12.3.tar.xz
  URL_MD5 66ef1fe02a59abdd5ad20152618475b1

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwebchannel
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtwebchannel-everywhere-src-5.12.3.tar.xz
  URL_MD5 bd4e964b35ab060a733262a8c3243718

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwebengine
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.qt.io/archive/qt/5.12/5.12.3/submodules/qtwebengine-everywhere-src-5.12.3.tar.xz
  URL_MD5 9841599435095b16f12870bd48ef5342

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> #-- -system-webengine-icu #-no-webengine-spellchecker -no-webengine-proprietary-codecs -no-webengine-webrtc -no-webengine-pepper-plugins
  ${CLEANUP_COMMAND}

  DEPENDS ext_xml ext_qtbase ext_qtdeclarative ext_qtwebchannel
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_qtdeclarative correctcmake
    COMMAND bash -c "find . -type f -name Qt5*.pc -exec sed -i \
    -e 's/Qt5\\(.*\\)d$*/Qt5\\1/g' {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
  )
endif()
