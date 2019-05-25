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
  URL http://download.kde.org/stable/frameworks/5.58/extra-cmake-modules-5.58.0.tar.xz
  URL_MD5 ab2e42031a1aa96eca27d029827fe9d8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
)

ExternalProject_Add(
  ext_breezeicons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/breeze-icons-5.58.0.tar.xz
  URL_MD5 551a7d46848ec758464a1cc96c68672f
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DBINARY_ICONS_RESOURCE=ON
             -DSKIP_INSTALL_ICONS=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_karchive
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/karchive-5.58.0.tar.xz
  URL_MD5 8d386da9f2e3a64018b979a8a0512a18
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_xz
)

ExternalProject_Add(
  ext_kcodecs
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kcodecs-5.58.0.tar.xz
  URL_MD5 5e2fd0f6588523b30ab9c8ab53bb1d76
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kconfig
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kconfig-5.58.0.tar.xz
  URL_MD5 f0a5d4f90ff74a764691e1d31caa378c
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kdbusaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kdbusaddons-5.58.0.tar.xz
  URL_MD5 2c0b68f305c296ab785c65d356ae0f45
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcoreaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kcoreaddons-5.58.0.tar.xz
  URL_MD5 ea7e5f1f3f880e1814e9952998476f76
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kguiaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kguiaddons-5.58.0.tar.xz
  URL_MD5 4485550ff755fd84f3126a2079dda66b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_ki18n
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/ki18n-5.58.0.tar.xz
  URL_MD5 82e0f37a01ad489db993833470ea59a8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_gettext-tools
)

ExternalProject_Add(
  ext_kunitconversion
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/kunitconversion-5.58.0.tar.xz
  URL_MD5 dc840b7be1e9c8e4ba715b3fee264714
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_ki18n
)

ExternalProject_Add(
  ext_kitemmodels
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kitemmodels-5.58.0.tar.xz
  URL_MD5 7d1a541f0e3ecb0e31a1e02bf14ec043
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kitemviews
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kitemviews-5.58.0.tar.xz
  URL_MD5 aa0444b0e1d59198eb5909931dacc8d7
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwidgetsaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kwidgetsaddons-5.58.0.tar.xz
  URL_MD5 05f0c66e03250369f296dfedf99ae9b4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwindowsystem
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kwindowsystem-5.58.0.tar.xz
  URL_MD5 596760bde216cb7335193fec16baeec4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_solid
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/solid-5.58.0.tar.xz
  URL_MD5 f6a9c58ce4e5a6ff82dd11f431aa3610
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

# KTextWidgets doesn't work with -DSONNET_USE_WIDGETS=OFF
ExternalProject_Add(
  ext_sonnet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/sonnet-5.58.0.tar.xz
  URL_MD5 1bb0c7eaaa57e91364da4ff023eaa7fd
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_syntaxhighlighting
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/syntax-highlighting-5.58.0.tar.xz
  URL_MD5 a93c3854a98a7616f02676cd2d6f1ed3
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKSYNTAXHIGHLIGHTING_USE_GUI=OFF
             -DQRC_SYNTAX=OFF
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kholidays
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/kholidays-5.58.0.tar.xz
  URL_MD5 d3f3e84e2658cd46489b591d79d85226
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kimageformats
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kimageformats-5.58.0.tar.xz
  URL_MD5 77c013f9a37934fc20239d0a3da205ba
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews
)

# without "-DKACTIVITIES_LIBRARY_ONLY=ON" ext_boost is required
ExternalProject_Add(
  ext_kactivities
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kactivities-5.58.0.tar.xz
  URL_MD5 e38977ae6c504a4f092c1c6ad3b6d28a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKACTIVITIES_LIBRARY_ONLY=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kauth
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kauth-5.58.0.tar.xz
  URL_MD5 a5df25b21f6aab84e310cd1d4f201141
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons
)

ExternalProject_Add(
  ext_kcompletion
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kcompletion-5.58.0.tar.xz
  URL_MD5 920cf624a739e63290743f5c3fa418f1
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwidgetsaddons ext_kconfig
)

ExternalProject_Add(
  ext_kcrash
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kcrash-5.58.0.tar.xz
  URL_MD5 c526a1714cf265711e0a20e9888978eb
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kdoctools
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/kdoctools-5.58.0.tar.xz
  URL_MD5 0fb5dd4eb907b55782e321f3d86eb825
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
          -MEINPROC_NO_KARCHIVE=ON
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_extra_cmake_modules
   ext_xml
   ext_xslt
)

ExternalProject_Add(
  ext_kjobwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kjobwidgets-5.58.0.tar.xz
  URL_MD5 ba814954d8506f44f3f683a427f4628c
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
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
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_knotifications
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/knotifications-5.58.0.tar.xz
  URL_MD5 4865198f8406068c67254ee3b59faab4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/knotifications.diff # otherwise no type named 'setBadgeLabelText' in namespace 'QtMac' QtMac::setBadgeLabelText
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwindowsystem ext_kconfig ext_kcodecs ext_kcoreaddons ext_phonon
)

ExternalProject_Add(
  ext_kpackage
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kpackage-5.58.0.tar.xz
  URL_MD5 cddda5df856b84d82289056e2521e0f5
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons
)

ExternalProject_Add(
  ext_kconfigwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kconfigwidgets-5.58.0.tar.xz
  URL_MD5 1c3ef53c26083ccd886144d794f56b3f
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kauth ext_kcoreaddons ext_kcodecs ext_kconfig ext_kguiaddons ext_ki18n ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_kiconthemes
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kiconthemes-5.58.0.tar.xz
  URL_MD5 2111f28ff4051914be2b7caf355befb4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons ext_kconfigwidgets ext_kwidgetsaddons ext_kitemviews
)
ExternalProject_Add(
  ext_kservice
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kservice-5.58.0.tar.xz
  URL_MD5 748c4fe209139249efaa669edb07fc91
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_ki18n
)

ExternalProject_Add(
  ext_kemoticons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kemoticons-5.58.0.tar.xz
  URL_MD5 b5ccd7868f3b506c238c3492a56ba65a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kservice ext_kcoreaddons
)

ExternalProject_Add(
  ext_ktextwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/ktextwidgets-5.58.0.tar.xz
  URL_MD5 1d030a5e1a52bac6528d39737d52ffb8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_kcompletion ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_kservice ext_kwidgetsaddons ext_kwindowsystem ext_sonnet
)

ExternalProject_Add(
  ext_kglobalaccel
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kglobalaccel-5.58.0.tar.xz
  URL_MD5 36c37994da9057dbda764229c847e873
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kxmlgui
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kxmlgui-5.58.0.tar.xz
  URL_MD5 899f9dca54c336c0034fa0aee2b5bf8f
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                            -DFORCE_DISABLE_KGLOBALACCEL=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kitemviews ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_ktextwidgets ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel
)

ExternalProject_Add(
  ext_kbookmarks
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kbookmarks-5.58.0.tar.xz
  URL_MD5 979b5612fd76e0959736347b4dbccce7
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcodecs ext_kconfigwidgets ext_kiconthemes ext_kwidgetsaddons ext_kxmlgui
)

ExternalProject_Add(
  ext_kwallet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kwallet-5.58.0.tar.xz
  URL_MD5 b27baeea7c21967609a4507cf616d47a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_gcrypt ext_gpgme ext_kcoreaddons ext_kconfig ext_kwindowsystem ext_ki18n ext_kconfigwidgets ext_knotifications ext_kservice ext_kwidgetsaddons ext_kiconthemes ext_kdbusaddons
)

ExternalProject_Add(
  ext_kio
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kio-5.58.0.tar.xz
  URL_MD5 1c8b5a0d4eddab6a97f387c5e5fd67a2
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kservice ext_solid ext_kwindowsystem ext_kwidgetsaddons ext_kcompletion ext_kconfigwidgets ext_kiconthemes ext_kitemviews ext_kjobwidgets ext_kbookmarks ext_kwallet
)

ExternalProject_Add(
  ext_kinit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/kinit-5.58.0.tar.xz
  URL_MD5 4d914c5ae3010ae600c858e9ccdeecb2
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
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
  URL https://download.kde.org/stable/frameworks/5.58/kded-5.58.0.tar.xz
  URL_MD5 4cb98a0531c0c9aea801f7678400c280
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
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
  URL https://download.kde.org/stable/frameworks/5.58/kdesignerplugin-5.58.0.tar.xz
  URL_MD5 2b96b2eab04f75ec34b2017ca95d5a16
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_kcoreaddons
   ext_kconfig
)

ExternalProject_Add(
  ext_kparts
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.pbone.net/pub/kde/stable/frameworks/5.58/kparts-5.58.0.tar.xz
  URL_MD5 79b028e61cbab0cfeb167e02363fad44
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
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
  URL http://download.kde.org/stable/frameworks/5.58/kdeclarative-5.58.0.tar.xz
  URL_MD5 c431a6562b643dcb1d56f3170d05025d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_ki18n ext_kiconthemes ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel ext_kguiaddons ext_kpackage
)

ExternalProject_Add(
  ext_kcmutils
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.58/kcmutils-5.58.0.tar.xz
  URL_MD5 cb3326fe23c757af9588f97f44882d71
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews ext_kconfigwidgets ext_kcoreaddons ext_ki18n ext_kiconthemes ext_kservice ext_kxmlgui ext_kdeclarative
)

ExternalProject_Add(
  ext_kjs
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/portingAids/kjs-5.58.0.tar.xz
  URL_MD5 7c88ae56084c5b43538344af72b4b6d2
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_pcre
)

ExternalProject_Add(
  ext_kdelibs4support
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/portingAids/kdelibs4support-5.58.0.tar.xz
  URL_MD5 1c4d788d910c3d7c06734aed2524ad0d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
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

if(APPLE)
  set(platform_specific_options -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}/colisionlibs;${CMAKE_INSTALL_PREFIX})
elseif(MINGW)
  set(platform_specific_options "")
else()
  # otherwise gets libpng and lipjpeg from system
  set(platform_specific_options -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX})
endif()

ExternalProject_Add(
  ext_khtml
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.58/portingAids/khtml-5.58.0.tar.xz
  URL_MD5 d62ea2b2a7eecd46f3b95460b8a94956
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                        ${platform_specific_options}
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
