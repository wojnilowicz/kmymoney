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
#   Contacts
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
  URL http://download.kde.org/stable/frameworks/5.71/extra-cmake-modules-5.71.0.tar.xz
  URL_MD5 39590c81474016b01ce1bc29fb808c3e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
)

ExternalProject_Add(
  ext_breezeicons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/breeze-icons-5.71.0.tar.xz
  URL_MD5 7f26815fd35fc6ffa5d9d7a6e21adb2d
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/breeze/breeze.diff
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DBINARY_ICONS_RESOURCE=ON
             -DSKIP_INSTALL_ICONS=ON
  BUILD_COMMAND ""
              COMMAND $(MAKE) breeze-icons-preprocess
              COMMAND $(MAKE) breeze-icons-dark-preprocess
              COMMAND bash -c "${CMAKE_CURRENT_SOURCE_DIR}/breeze/adjust-rcc.sh $ENV{KMYMONEY_SOURCES}/kmymoney/icons/icons.cpp <BINARY_DIR>/icons/res/breeze-icons.qrc"
              COMMAND bash -c "${CMAKE_CURRENT_SOURCE_DIR}/breeze/adjust-rcc.sh $ENV{KMYMONEY_SOURCES}/kmymoney/icons/icons.cpp <BINARY_DIR>/icons-dark/res/breeze-icons-dark.qrc"
              COMMAND $(MAKE)
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_karchive
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/karchive-5.71.0.tar.xz
  URL_MD5 67a3b31f2cce390437019f0611c0a313
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_xz
)

ExternalProject_Add(
  ext_kcodecs
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kcodecs-5.71.0.tar.xz
  URL_MD5 3f6780dda56469aa002890de7d20a122
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kconfig
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kconfig-5.71.0.tar.xz
  URL_MD5 8fc3e8aeec7a8b5c67f817a4bca52a34
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kdbusaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kdbusaddons-5.71.0.tar.xz
  URL_MD5 eba02af01de7e92141a3cff0c35405e3
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcoreaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kcoreaddons-5.71.0.tar.xz
  URL_MD5 ccd208ec8997895aa3c1e61d911e67b8
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kguiaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kguiaddons-5.71.0.tar.xz
  URL_MD5 db0122bec0e812c3386dfcc6171e9229
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_ki18n
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/ki18n-5.71.0.tar.xz
  URL_MD5 4662355650da204e6b8c317465230a03
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_gettext
)

ExternalProject_Add(
  ext_kunitconversion
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kunitconversion-5.71.0.tar.xz
  URL_MD5 7275bf0827398aa5f0158c4ae30cb68b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_ki18n
)

ExternalProject_Add(
  ext_kitemmodels
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kitemmodels-5.71.0.tar.xz
  URL_MD5 9c44cc7565df5df045429258a4906c6b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kitemviews
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kitemviews-5.71.0.tar.xz
  URL_MD5 8fe8dd2a382502a772c2d1cff449e534
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwidgetsaddons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kwidgetsaddons-5.71.0.tar.xz
  URL_MD5 c527f9fcf4601aaa125201956900b00a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwindowsystem
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kwindowsystem-5.71.0.tar.xz
  URL_MD5 c4e43f965d8def524e5271521f55dc65
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_solid
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/solid-5.71.0.tar.xz
  URL_MD5 6167f8aaf17bb09e9b9b029cd56291b0
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

# KTextWidgets doesn't work with -DSONNET_USE_WIDGETS=OFF
ExternalProject_Add(
  ext_sonnet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/sonnet-5.71.0.tar.xz
  URL_MD5 1992ff30b5b3638c06400b614009df8e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_syntaxhighlighting
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/syntax-highlighting-5.71.0.tar.xz
  URL_MD5 8d8c5bf7ccf842f529364d0f5020ff91
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKSYNTAXHIGHLIGHTING_USE_GUI=OFF
             -DQRC_SYNTAX=OFF
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kholidays
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kholidays-5.71.0.tar.xz
  URL_MD5 556cf8450d209b7ccdbade72eb6fd086
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kimageformats
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kimageformats-5.71.0.tar.xz
  URL_MD5 1148d408261f4e99689fa4d243004d9a
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews
)

# without "-DKACTIVITIES_LIBRARY_ONLY=ON" ext_boost is required
ExternalProject_Add(
  ext_kactivities
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kactivities-5.71.0.tar.xz
  URL_MD5 4eb067e151d0a2a9303fae9801e8d99b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DKACTIVITIES_LIBRARY_ONLY=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kauth
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kauth-5.71.0.tar.xz
  URL_MD5 8af884f84f6c07c03063f5c3b7018d64
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons
)

ExternalProject_Add(
  ext_kcompletion
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kcompletion-5.71.0.tar.xz
  URL_MD5 8a585781c675f3da27fed9538b388c12
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwidgetsaddons ext_kconfig
)

ExternalProject_Add(
  ext_kcontacts
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kcontacts-5.71.0.tar.xz
  URL_MD5 2f39fb4bcb6d1a6715d12d55720f754d

  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_kcoreaddons ext_ki18n ext_kconfig ext_kcodecs
)

ExternalProject_Add(
  ext_kcrash
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kcrash-5.71.0.tar.xz
  URL_MD5 646b532f823f1b6fdadf0885ff0e3d29
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/00a71a12f33dd423c395aeaeaddb709cd434296f.patch
        COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/a82b949adc1619ad44da002a207073b019865f4b.patch
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kdoctools
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kdoctools-5.71.0.tar.xz
  URL_MD5 e051f38d1c51eca58c54d986fcd12ed7
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
  URL http://download.kde.org/stable/frameworks/5.71/kjobwidgets-5.71.0.tar.xz
  URL_MD5 0857a968c2d46935d875d5863134487b
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_phonon
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/phonon/4.11.1/phonon-4.11.1.tar.xz
  URL_MD5 d3df5ba646e4b3f11623d998caa40e74
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_knotifications
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/knotifications-5.71.0.tar.xz
  URL_MD5 d051bef5a0c2e025acfc215ff64f9e72
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-snore-toast.diff # otherwise no type named 'setBadgeLabelText' in namespace 'QtMac' QtMac::setBadgeLabelText
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwindowsystem ext_kconfig ext_kcodecs ext_kcoreaddons ext_phonon
)

ExternalProject_Add(
  ext_kpackage
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kpackage-5.71.0.tar.xz
  URL_MD5 852f0cecc9b91a38816bf8e754049886
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons
)

ExternalProject_Add(
  ext_kconfigwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kconfigwidgets-5.71.0.tar.xz
  URL_MD5 c8b8a63018f367d357d757eefd0ba219
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kauth ext_kcoreaddons ext_kcodecs ext_kconfig ext_kguiaddons ext_ki18n ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_kiconthemes
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kiconthemes-5.71.0.tar.xz
  URL_MD5 5adefc2992c4adcb749bee99fb10de60
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons ext_kconfigwidgets ext_kwidgetsaddons ext_kitemviews
)
ExternalProject_Add(
  ext_kservice
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kservice-5.71.0.tar.xz
  URL_MD5 868583ac4038cf535a2b1bbefb9b7a48
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_ki18n
)

ExternalProject_Add(
  ext_kemoticons
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kemoticons-5.71.0.tar.xz
  URL_MD5 f02a73da7d6b7bafc9eeadcd82d65767
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kservice ext_kcoreaddons
)

ExternalProject_Add(
  ext_ktextwidgets
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/ktextwidgets-5.71.0.tar.xz
  URL_MD5 fa0e0f5e0715fbe9e35560fd8b4632e1
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_kcompletion ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_kservice ext_kwidgetsaddons ext_kwindowsystem ext_sonnet
)

ExternalProject_Add(
  ext_kglobalaccel
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kglobalaccel-5.71.0.tar.xz
  URL_MD5 d418a2438a2ce6d47c985b8aeed95b91
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kxmlgui
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kxmlgui-5.71.0.tar.xz
  URL_MD5 36183c3d85f6b92e4842273186f8e254
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                            -DFORCE_DISABLE_KGLOBALACCEL=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kitemviews ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_ktextwidgets ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel
)

ExternalProject_Add(
  ext_kbookmarks
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kbookmarks-5.71.0.tar.xz
  URL_MD5 85876496c90f830c389ed30d6952cbb3
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcodecs ext_kconfigwidgets ext_kiconthemes ext_kwidgetsaddons ext_kxmlgui
)

ExternalProject_Add(
  ext_kwallet
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kwallet-5.71.0.tar.xz
  URL_MD5 799b978ef498c0a7ff950d4ab5fd0a3c
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
                        -DBUILD_KWALLETD=ON
                        -DBUILD_KWALLET_QUERY=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_gcrypt ext_gpgme ext_kcoreaddons ext_kconfig ext_kwindowsystem ext_ki18n ext_kconfigwidgets ext_knotifications ext_kservice ext_kwidgetsaddons ext_kiconthemes ext_kdbusaddons
)

ExternalProject_Add(
  ext_kio
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kio-5.71.0.tar.xz
  URL_MD5 c35b70f492a845514887c26ed72137f6
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
             -DBUILD_DESIGNERPLUGIN=OFF
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kservice ext_solid ext_kwindowsystem ext_kwidgetsaddons ext_kcompletion ext_kconfigwidgets ext_kiconthemes ext_kitemviews ext_kjobwidgets ext_kbookmarks ext_kwallet
)

ExternalProject_Add(
  ext_kinit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kinit-5.71.0.tar.xz
  URL_MD5 9d9a7d6c3ea7529b541582d0861669fd
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
  URL https://download.kde.org/stable/frameworks/5.71/kded-5.71.0.tar.xz
  URL_MD5 dfcfde9d51a40425be55343cb212ae63
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
  URL https://download.kde.org/stable/frameworks/5.69/kdesignerplugin-5.69.0.tar.xz
  URL_MD5 d41d8cd98f00b204e9800998ecf8427e
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS
   ext_kcoreaddons
   ext_kconfig
)

ExternalProject_Add(
  ext_kparts
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/kparts-5.71.0.tar.xz
  URL_MD5 d4de7a6f0e1d2d245c0ac20fb9b68d7a
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
  URL http://download.kde.org/stable/frameworks/5.71/kdeclarative-5.71.0.tar.xz
  URL_MD5 ab14237d7a950b410478cbe43294b92c
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_ki18n ext_kiconthemes ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel ext_kguiaddons ext_kpackage
)

ExternalProject_Add(
  ext_kcmutils
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.kde.org/stable/frameworks/5.71/kcmutils-5.71.0.tar.xz
  URL_MD5 6d1b34b9eda374ccd564339ceaddf0f4
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews ext_kconfigwidgets ext_kcoreaddons ext_ki18n ext_kiconthemes ext_kservice ext_kxmlgui ext_kdeclarative
)

ExternalProject_Add(
  ext_kjs
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/portingAids/kjs-5.71.0.tar.xz
  URL_MD5 ac1a701b74186d8d4b0c4e2ee907199d
  CMAKE_ARGS ${CMAKE_FRAMEWORKS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_pcre
)

ExternalProject_Add(
  ext_kdelibs4support
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/frameworks/5.71/portingAids/kdelibs4support-5.71.0.tar.xz
  URL_MD5 c1eb25b83faa75d7a4609f82bf1f0f28
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
  URL https://download.kde.org/stable/frameworks/5.71/portingAids/khtml-5.71.0.tar.xz
  URL_MD5 8a792c28d538739325baf1fb8139807f
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
