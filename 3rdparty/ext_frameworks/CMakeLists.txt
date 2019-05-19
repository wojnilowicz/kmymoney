# All needed frameworks:
# Tier 1
#   BreezeIcons
#   Archive
#   Codecs
#   Config
#   CoreAddons
#   DBusAddons
#   GuiAddons
#   I18n
#   UnitConversion
#   ItemModels
#   ItemViews
#   WidgetsAddons
#   WindowSystem
#   Solid
#   Sonnet
#   SyntaxHighlighting
# Tier 2
#   ImageFormats
#   Activities
#   Auth
#   Completion
#   Crash
#   JobWidgets
#   Notifications
#   Package
# Tier 3
#   Emoticons
#   ConfigWidgets
#   IconThemes
#   Service
#   TextWidgets
#   XmlGui
#   Bookmarks
#   GlobalAccel
#   KIO
#   Init
#   KDED
#   DesignerPlugin
#   Parts
#   Declarative
#   KCMUtils
#   Wallet
# Porting aids
#   Js
#   Html

set(CMAKE_FRAMEWORKS_GLOBAL_PROFILE ${CMAKE_GLOBAL_PROFILE}
                                                                        -DBUILD_TESTING=FALSE
                                                                        $<$<PLATFORM_ID:Darwin>:-DAPPLE_SUPPRESS_X11_WARNING=ON>
)

ExternalProject_Add(
  ext_extra_cmake_modules
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/extra-cmake-modules-5.57.0.tar.xz
  URL_MD5 b1815b93489fbed46b1512c2567a889e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
)

ExternalProject_Add(
  ext_breezeicons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/breeze-icons-5.57.0.tar.xz
  URL_MD5 a378501cbbfd78829e4b36747f4a5ce7
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DBINARY_ICONS_RESOURCE=ON
             -DSKIP_INSTALL_ICONS=ON
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_karchive
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/karchive-5.57.0.tar.xz
  URL_MD5 059c3b6e24c30655ccf2bd91ef7af733
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcodecs
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kcodecs-5.57.0.tar.xz
  URL_MD5 a4f0d3fb375fb15e108166ee68d6c7a3
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kconfig
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kconfig-5.57.0.tar.xz
  URL_MD5 765a8599190cc7ebaebfb7d062e359a8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kdbusaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kdbusaddons-5.57.0.tar.xz
  URL_MD5 2fd487f58c349e7c5adcb4f96bd5948c
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcoreaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kcoreaddons-5.57.0.tar.xz
  URL_MD5 1825368e29fe50930a4d2be1769ed48b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kguiaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kguiaddons-5.57.0.tar.xz
  URL_MD5 74b61e8ef3efdfd834caf258c223a9c9
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_ki18n
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/ki18n-5.57.0.tar.xz
  URL_MD5 2178c765e29675b21a044fc074a01246
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_gettext
)

ExternalProject_Add(
  ext_kunitconversion
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kunitconversion-5.57.0.tar.xz
  URL_MD5 d023a19fa6fbff1479cc9144eb1d8393
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_ki18n
)

ExternalProject_Add(
  ext_kitemmodels
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kitemmodels-5.57.0.tar.xz
  URL_MD5 9c268db88630f1d62b5da51131c81a60
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kitemviews
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kitemviews-5.57.0.tar.xz
  URL_MD5 fc44f88d4dca7147887142f1a272711d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwidgetsaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kwidgetsaddons-5.57.0.tar.xz
  URL_MD5 b85e32d16c9d4c46a31a28ca63160c4d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwindowsystem
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kwindowsystem-5.57.0.tar.xz
  URL_MD5 1789e26654314ce785a1d7618f5a59ae
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_solid
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/solid-5.57.0.tar.xz
  URL_MD5 d3155b3f6301645173273c898bae488f
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/solid.diff
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

# KTextWidgets doesn't work with -DSONNET_USE_WIDGETS=OFF
ExternalProject_Add(
  ext_sonnet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/sonnet-5.57.0.tar.xz
  URL_MD5 af5d91569b7f978417fb1859e62a4984
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_syntaxhighlighting
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/syntax-highlighting-5.57.0.tar.xz
  URL_MD5 b979ea0b6b2d6fa27f61c152dada814a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKSYNTAXHIGHLIGHTING_USE_GUI=OFF
             -DQRC_SYNTAX=OFF
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kholidays
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kholidays-5.57.0.tar.xz
  URL_MD5 f4656e414e5d17f6efd203a0583d83d5
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kimageformats
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kimageformats-5.57.0.tar.xz
  URL_MD5 c46aaf66f3ac9100b09f829f40e6cce8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews
)

# without "-DKACTIVITIES_LIBRARY_ONLY=ON" ext_boost is required
ExternalProject_Add(
  ext_kactivities
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kactivities-5.57.0.tar.xz
  URL_MD5 a17a913ae89cdf8b959ed49b1a97512e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKACTIVITIES_LIBRARY_ONLY=ON
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kauth
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kauth-5.57.0.tar.xz
  URL_MD5 3e88ca2dbc20c47dab00c40b747e0899
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons
)

ExternalProject_Add(
  ext_kcompletion
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kcompletion-5.57.0.tar.xz
  URL_MD5 f33439a380b21163c1f04c780f7c820a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwidgetsaddons ext_kconfig
)

ExternalProject_Add(
  ext_kcrash
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kcrash-5.57.0.tar.xz
  URL_MD5 4a806db956de0a3c76612fa2156fae34
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kdoctools
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kdoctools-5.57.0.tar.xz
  URL_MD5 90a180ef293a66ccf18ee79cc7f33776
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
          -MEINPROC_NO_KARCHIVE=ON
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_extra_cmake_modules
   ext_xml
   ext_xslt
)

ExternalProject_Add(
  ext_kjobwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kjobwidgets-5.57.0.tar.xz
  URL_MD5 be37764f55cb5deed630da21af9385a9
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_phonon
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/phonon/4.10.2/phonon-4.10.2.tar.xz
  URL_MD5 ed828952beaa140d1c025513febd8e5d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
              -DPHONON_BUILD_PHONON4QT5=ON
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_knotifications
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/knotifications-5.57.0.tar.xz
  URL_MD5 a5381274fb940b1ce06850c2c8995b5e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/knotifications.diff # otherwise no type named 'setBadgeLabelText' in namespace 'QtMac' QtMac::setBadgeLabelText
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwindowsystem ext_kconfig ext_kcodecs ext_kcoreaddons ext_phonon
)

ExternalProject_Add(
  ext_kpackage
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kpackage-5.57.0.tar.xz
  URL_MD5 121f814f4ad12c56928ce7b857ea87b7
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons
)

ExternalProject_Add(
  ext_kconfigwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kconfigwidgets-5.57.0.tar.xz
  URL_MD5 676fdcd53f6a5b43a729881304c3fbbd
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kauth ext_kcoreaddons ext_kcodecs ext_kconfig ext_kguiaddons ext_ki18n ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_kiconthemes
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kiconthemes-5.57.0.tar.xz
  URL_MD5 a0812ab8df20b5e85cccd117378caa21
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons ext_kconfigwidgets ext_kwidgetsaddons ext_kitemviews
)
ExternalProject_Add(
  ext_kservice
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kservice-5.57.0.tar.xz
  URL_MD5 556b7cc6960cee88256acf40fd756a2b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_ki18n
)

ExternalProject_Add(
  ext_kemoticons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kemoticons-5.57.0.tar.xz
  URL_MD5 f4fce4280e9f0c6330d385710a68d686
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kservice ext_kcoreaddons
)

ExternalProject_Add(
  ext_ktextwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/ktextwidgets-5.57.0.tar.xz
  URL_MD5 4d7fd58963b27fb64c80365e9e366caa
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_kcompletion ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_kservice ext_kwidgetsaddons ext_kwindowsystem ext_sonnet
)

ExternalProject_Add(
  ext_kglobalaccel
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kglobalaccel-5.57.0.tar.xz
  URL_MD5 2c750aaf52b8178abca68ac437e41487
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kxmlgui
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kxmlgui-5.57.0.tar.xz
  URL_MD5 5716ffbd7ad51c55544e1cf924cb1dcc
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                            -DFORCE_DISABLE_KGLOBALACCEL=ON
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kitemviews ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_ktextwidgets ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel
)

ExternalProject_Add(
  ext_kbookmarks
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kbookmarks-5.57.0.tar.xz
  URL_MD5 afdb9602598bdebfa1a01c2ebd0c1565
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcodecs ext_kconfigwidgets ext_kiconthemes ext_kwidgetsaddons ext_kxmlgui
)

ExternalProject_Add(
  ext_kwallet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kwallet-5.57.0.tar.xz
  URL_MD5 96323e110c0a2f04414c6cedf6603ea0
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_gcrypt ext_gpgme ext_kcoreaddons ext_kconfig ext_kwindowsystem ext_ki18n ext_kconfigwidgets ext_knotifications ext_kservice ext_kwidgetsaddons ext_kiconthemes ext_kdbusaddons
)

ExternalProject_Add(
  ext_kio
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kio-5.57.0.tar.xz
  URL_MD5 d21d5e216aabd0bc5836e5a073d7857d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kservice ext_solid ext_kwindowsystem ext_kwidgetsaddons ext_kcompletion ext_kconfigwidgets ext_kiconthemes ext_kitemviews ext_kjobwidgets ext_kbookmarks ext_kwallet
)

ExternalProject_Add(
  ext_kinit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kinit-5.57.0.tar.xz
  URL_MD5 9d6893978fcb67c4d993cd30087d0ca2
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
    ext_kservice
    ext_kio
    ext_ki18n
    ext_kwindowsystem
    ext_kcrash
    ext_kconfig
)

ExternalProject_Add(
  ext_kded
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kded-5.57.0.tar.xz
  URL_MD5 694b8640be69230123758181d7678455
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_kconfig
   ext_kcoreaddons
   ext_kcrash
   ext_kdbusaddons
   ext_kinit
   ext_kservice
)

ExternalProject_Add(
  ext_kdesignerplugin
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/kdesignerplugin-5.57.0.tar.xz
  URL_MD5 8c5c4c6366999ebc72d32ed6eb88c1c4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_kcoreaddons
   ext_kconfig
)

ExternalProject_Add(
  ext_kparts
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.pbone.net/pub/kde/stable/frameworks/5.57/kparts-5.57.0.tar.xz
  URL_MD5 ea1ae4cd89d10b7ec4c3ca19f2ddb5f2
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
 DEPENDS
   ext_kconfig
   ext_kcoreaddons
   ext_ki18n
   ext_kiconthemes
   ext_kio
   ext_kjobwidgets
   ext_kservice
   ext_ktextwidgets
   ext_kwidgetsaddons
   ext_kxmlgui
)

ExternalProject_Add(
  ext_kdeclarative
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kdeclarative-5.57.0.tar.xz
  URL_MD5 a9af38c4d6f819cd71006c5e58ef7da3
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_ki18n ext_kiconthemes ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel ext_kguiaddons ext_kpackage
)

ExternalProject_Add(
  ext_kcmutils
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.57/kcmutils-5.57.0.tar.xz
  URL_MD5 8863d6867d344e85295220f7fb8fc8b6
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews ext_kconfigwidgets ext_kcoreaddons ext_ki18n ext_kiconthemes ext_kservice ext_kxmlgui ext_kdeclarative
)

ExternalProject_Add(
  ext_kjs
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/portingAids/kjs-5.57.0.tar.xz
  URL_MD5 94bd611099b302b4b708974a639d6480
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_pcre
)

ExternalProject_Add(
  ext_kdelibs4support
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/portingAids/kdelibs4support-5.57.0.tar.xz
  URL_MD5 dd3e37b0b374466734c6f567877f5a85
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
#  DEPENDS
#   ext_kcompletion
#   ext_kconfig
#   ext_kconfigwidgets
#   ext_kcrash
#   ext_kdesignerplugin
#   ext_kglobalaccel
#   ext_kdoctools
#   ext_kemoticons
#   ext_kguiaddons
#   ext_ki18n
#   ext_kiconthemes
#   ext_knotifications
#   ext_kparts
#   ext_kservice
#   ext_ktextwidgets
#   ext_kunitconversion
#   ext_kwidgetsaddons
#   ext_kwindowsystem
#   ext_kxmlgui
#   ext_kdbusaddons
#   ext_kded
#   ext_kdewin
)

if(MINGW)
  set(platform_specific_path ${UNIX_INSTALL_PREFIX}/bin/data/kservicetypes5)
else()
  set(platform_specific_path ${UNIX_INSTALL_PREFIX}/share/kservicetypes5)
endif()

ExternalProject_Add_Step(
  ext_kdelibs4support getservicetype
  COMMAND cp qimageio_plugin.desktop ${platform_specific_path}

  DEPENDEES install
  WORKING_DIRECTORY <SOURCE_DIR>/src/kio/imageformats
)

ExternalProject_Add(
  ext_khtml
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.57/portingAids/khtml-5.57.0.tar.xz
  URL_MD5 425132b82608fe5b619bae07b20bf6be
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                            -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}/imagelibs
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS
    ext_karchive
    ext_kcodecs
    ext_kglobalaccel
    ext_ki18n
    ext_kiconthemes
    ext_kio
    ext_kjs
    ext_knotifications
    ext_kparts
    ext_sonnet
    ext_ktextwidgets
    ext_kwallet
    ext_kwidgetsaddons
    ext_kwindowsystem
    ext_kxmlgui
    ext_kdelibs4support
    ext_gif
)
