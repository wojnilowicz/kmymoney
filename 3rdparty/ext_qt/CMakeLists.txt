# qtquickcontrols and qtwebchannel are required by qtwebengine
# "WebRTC" is required by "Pepper Plugins" is required by "Printing and PDF"
# don't use -I and -L switches as they will cause explicit link to
# OpenSSL instead of BoringSSL in QWebEngine

ExternalProject_Add(
  ext_qt
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/official_releases/qt/5.12/5.12.2/single/qt-everywhere-src-5.12.2.tar.xz
  URL_MD5 99c2eb46e533371798b4ca2d1458e065

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qsql_mysql.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix_GenericDataLocation_mac.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qdbus-manager-quit-5.12.2.diff
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qstandardpaths-extra-dirs.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
  -prefix ${CMAKE_INSTALL_PREFIX}
  -opensource -confirm-license -verbose -nomake examples -nomake tests -nomake tools
  ICU_LIBDIR="${CMAKE_INSTALL_PREFIX}/lib" ICU_INCDIR="${CMAKE_INSTALL_PREFIX}/include"
  OPENSSL_LIBDIR="${CMAKE_INSTALL_PREFIX}/openssl/lib" OPENSSL_INCDIR="${CMAKE_INSTALL_PREFIX}/openssl/include"
  -openssl-linked
  -skip qt3d
  -skip qtactiveqt
  -skip qtcanvas3d
  -skip qtconnectivity
  -skip qtenginio
  -skip qtgraphicaleffects
  -skip qtlocation
  -skip qtwayland
  -skip qtandroidextras
  -skip qtserialport
  -skip qtdatavis3d
  -skip qtvirtualkeyboard
  -skip qtspeech
  -skip qtsensors
  -skip qtgamepad
  -skip qtscxml
  -skip qtremoteobjects
  -skip qtxmlpatterns
  -skip qtcharts
  -skip qtpurchasing
  -skip qtserialbus
  -skip qtdoc
  -skip qtwebglplugin
  -skip qtwebsockets
  -skip qtwebview
  -skip qtmultimedia
  -skip qtnetworkauth
  -skip qtimageformats
  -system-webengine-icu
  ${qt_extras_option}

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
  DEPENDS ext_xslt ext_mysql ext_sqlite ext_postgresql ext_pcre2
)


if (APPLE)
  set(qt_platform_specific_option -macos-additional-datadirs ${CMAKE_INSTALL_PREFIX}/share)
elseif(UNIX)
  set(qt_platform_specific_option -recheck-all -xcb -macos-additional-datadirs ${CMAKE_INSTALL_PREFIX}/share)
endif ()

ExternalProject_Add(
  ext_qtbase
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtbase-everywhere-src-5.12.2.tar.xz
  URL_MD5 a8f4329015a25e3488078d905379bedd
  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qstandardpaths-extra-dirs.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/fix_GenericDataLocation_mac.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qdbus-manager-quit-5.7.patch
#         COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/qsql_mysql.diff
  CONFIGURE_COMMAND <SOURCE_DIR>/configure -prefix ${CMAKE_INSTALL_PREFIX} -opensource -confirm-license -verbose -pkg-config -release -nomake examples -nomake tests -nomake tools ICU_PREFIX=${CMAKE_INSTALL_PREFIX} ${qt_platform_specific_option}

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
  DEPENDS ext_xslt ext_sqlite ext_icu ext_png #ext_mysql #ext_postgresql
)

ExternalProject_Add(
  ext_qtsvg
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtsvg-everywhere-src-5.12.2.tar.xz
  URL_MD5 175641313dbf21c31e5aae33d8757cad

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qttools
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qttools-everywhere-src-5.12.2.tar.xz
  URL_MD5 ac44a458945e801b82efd97f2c836237

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtlocation
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtlocation-everywhere-src-5.12.2.tar.xz
  URL_MD5 cd4b62e31d33c7f0d68bec523af828b8

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtxmlpatterns
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtxmlpatterns-everywhere-src-5.12.2.tar.xz
  URL_MD5 d84dd8a7659a5f209835ccee82792d13

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase
)

ExternalProject_Add(
  ext_qtdeclarative
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtdeclarative-everywhere-src-5.12.2.tar.xz
  URL_MD5 7b6abe476da9368ae73de430def58c53

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtxmlpatterns ext_qtsvg
)

ExternalProject_Add(
  ext_qtx11extras
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtx11extras-everywhere-src-5.12.2.tar.xz
  URL_MD5 08eb87bb411d96e62f91fab121df8e81

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j2
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j2 install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwinextras
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtwinextras-everywhere-src-5.12.2.tar.xz
  URL_MD5 a60da2661b4c76a48efee7249028aa0a

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtmacextras
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtmacextras-everywhere-src-5.12.2.tar.xz
  URL_MD5 254f05233b041080a401e4769c1d9a60

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwebchannel
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtwebchannel-everywhere-src-5.12.2.tar.xz
  URL_MD5 e71c7ba1c932ec777150384a5c72a129

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR>
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative
)

ExternalProject_Add(
  ext_qtwebengine
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.qt.io/archive/qt/5.12/5.12.2/submodules/qtwebengine-everywhere-src-5.12.2.tar.xz
  URL_MD5 a5182f10645103b54a35f8611fd13506

  CONFIGURE_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/qmake <SOURCE_DIR> #-- -system-webengine-icu #-no-webengine-spellchecker -no-webengine-proprietary-codecs -no-webengine-webrtc -no-webengine-pepper-plugins
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtbase ext_qtdeclarative ext_qtwebchannel ext_qtlocation
)


