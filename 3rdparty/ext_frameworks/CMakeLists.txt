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
#   ItemModels
#   ItemViews
#   WidgetsAddons
#   WindowSystem
#   Solid
#   Sonnet
#   SyntaxHighlighting
# Tier 2
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
#   Declarative
#   KCMUtils
#   Wallet

ExternalProject_Add(
  ext_extra_cmake_modules
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/extra-cmake-modules-5.56.0.tar.xz
  URL_MD5 89be75b53098e1e6cd9b2e30b0bb9e44
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
)

ExternalProject_Add(
  ext_breezeicons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/breeze-icons-5.56.0.tar.xz
  URL_MD5 e708f4df6128da1ea7c7493d3da836e4
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBINARY_ICONS_RESOURCE=ON
             -DSKIP_INSTALL_ICONS=ON
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_karchive
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/karchive-5.56.0.tar.xz
  URL_MD5 681afd14a1c294795d509930418b83d0
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcodecs
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kcodecs-5.56.0.tar.xz
  URL_MD5 a3cda4ef6700a9975225f2fc6544ceb6
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kconfig
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kconfig-5.56.0.tar.xz
  URL_MD5 8064e6bbb1703c42e22055ea8e4f7250
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kdbusaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kdbusaddons-5.56.0.tar.xz
  URL_MD5 7e9071c078a7a56aaa74a7554a338f04
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcoreaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kcoreaddons-5.56.0.tar.xz
  URL_MD5 61056e358e4a90e15023573bf73bbbfc
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kguiaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kguiaddons-5.56.0.tar.xz
  URL_MD5 57cf67c1d6911c244f6d79a24da7425e
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_ki18n
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/ki18n-5.56.0.tar.xz
  URL_MD5 d3b4ea17742aba2a24b8b3115a5df47d
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules ext_gettext
)

ExternalProject_Add(
  ext_kitemmodels
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kitemmodels-5.56.0.tar.xz
  URL_MD5 2b83a402d77edce5d472bd8c593b7717
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kitemviews
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kitemviews-5.56.0.tar.xz
  URL_MD5 e050ff0c4362abe8ebeb4aa31b0617bb
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwidgetsaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kwidgetsaddons-5.56.0.tar.xz
  URL_MD5 59fef0ceb831b41a514aea9d9ab59bab
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwindowsystem
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kwindowsystem-5.56.0.tar.xz
  URL_MD5 4cec43e5c481322f0b36e78d6b5688a5
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_solid
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/solid-5.56.0.tar.xz
  URL_MD5 2b3708251bb4f86b5b9aae5c4c61348b
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_sonnet
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/sonnet-5.56.0.tar.xz
  URL_MD5 0b42e1dd0873c54a9d06dc4ef95bdfff
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_syntaxhighlighting
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/syntax-highlighting-5.56.0.tar.xz
  URL_MD5 9cb5b2eb63776bf69fa196f45fd53675
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kholidays
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.kde.org/stable/frameworks/5.56/kholidays-5.56.0.tar.xz
  URL_MD5 8abfaeee4518335b2c9881f77004263f
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kactivities
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kactivities-5.56.0.tar.xz
  URL_MD5 73e6b1ddb49999ffad14ea3485bd6121
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_boost ext_kconfig ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kauth
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kauth-5.56.0.tar.xz
  URL_MD5 3d4fe8eda589cca76ca846e8a151c638
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kcoreaddons
)

ExternalProject_Add(
  ext_kcompletion
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kcompletion-5.56.0.tar.xz
  URL_MD5 8f5717c167558618c0fa62504f4204af
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kwidgetsaddons ext_kconfig
)

ExternalProject_Add(
  ext_kcrash
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kcrash-5.56.0.tar.xz
  URL_MD5 db49cf5fb083bf45dcca7c7813ee92ac
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kjobwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kjobwidgets-5.56.0.tar.xz
  URL_MD5 f61a3f12abe0fb2b8c25cfa834dcf68e
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kcoreaddons ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_phonon
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.kde.org/stable/phonon/4.10.2/phonon-4.10.2.tar.xz
  URL_MD5 ed828952beaa140d1c025513febd8e5d
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
              -DPHONON_BUILD_PHONON4QT5=ON
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_knotifications
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/knotifications-5.56.0.tar.xz
  URL_MD5 ee5bffb6757af216ceb0f8c7d984fbc9
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kwindowsystem ext_kconfig ext_kcodecs ext_kcoreaddons ext_phonon
)

ExternalProject_Add(
  ext_kpackage
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kpackage-5.56.0.tar.xz
  URL_MD5 3754136eb861119e747d56d6d5c26bc5
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons
)

ExternalProject_Add(
  ext_kconfigwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kconfigwidgets-5.56.0.tar.xz
  URL_MD5 7f5ba2cf7ef7190c554d24967848a4a1
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kauth ext_kcoreaddons ext_kcodecs ext_kconfig ext_kguiaddons ext_ki18n ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_kiconthemes
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kiconthemes-5.56.0.tar.xz
  URL_MD5 f48bc1e2268193f0abe07cb18a1d5b25
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons ext_kconfigwidgets ext_kwidgetsaddons ext_kitemviews
)

ExternalProject_Add(
  ext_kservice
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kservice-5.56.0.tar.xz
  URL_MD5 a66a5bba0556be3da8487f13f524c3bb
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_ki18n
)

ExternalProject_Add(
  ext_kemoticons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kemoticons-5.56.0.tar.xz
  URL_MD5 85a0f162dd40f5b5f99e942e476540cc
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_karchive ext_kconfig ext_kservice ext_kcoreaddons
)

ExternalProject_Add(
  ext_ktextwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/ktextwidgets-5.56.0.tar.xz
  URL_MD5 4071bcd54668bcb0977ad3af03ec9500
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules ext_kcompletion ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_kservice ext_kwidgetsaddons ext_kwindowsystem ext_sonnet
)

ExternalProject_Add(
  ext_kglobalaccel
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kglobalaccel-5.56.0.tar.xz
  URL_MD5 1dd09569d7824f2bef24a110b3fde08e
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kxmlgui
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kxmlgui-5.56.0.tar.xz
  URL_MD5 4686179511ed09440032b3dcd6f1815c
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kcoreaddons ext_kitemviews ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_ktextwidgets ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel
)

ExternalProject_Add(
  ext_kbookmarks
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kbookmarks-5.56.0.tar.xz
  URL_MD5 07d9185bfacd575c4ed57b04ca495e82
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcodecs ext_kconfigwidgets ext_kiconthemes ext_kwidgetsaddons ext_kxmlgui
)

ExternalProject_Add(
  ext_kwallet
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kwallet-5.56.0.tar.xz
  URL_MD5 65c4168de9bae3b745f2f2a347dba1ba
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_gcrypt ext_gpgme ext_kcoreaddons ext_kconfig ext_kwindowsystem ext_ki18n ext_kconfigwidgets ext_knotifications ext_kservice ext_kwidgetsaddons ext_kiconthemes ext_kdbusaddons
)

ExternalProject_Add(
  ext_kio
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kio-5.56.0.tar.xz
  URL_MD5 262629a89383652f57c4f3925d6b4424
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_extra_cmake_modules ext_karchive ext_kconfig ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kservice ext_solid ext_kwindowsystem ext_kwidgetsaddons ext_kcompletion ext_kconfigwidgets ext_kiconthemes ext_kitemviews ext_kjobwidgets ext_kbookmarks ext_kwallet
)

ExternalProject_Add(
  ext_kdeclarative
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kdeclarative-5.56.0.tar.xz
  URL_MD5 026ce4fdb02dbc05549742756f004d1e
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kconfig ext_ki18n ext_kiconthemes ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel ext_kguiaddons ext_kpackage
)

ExternalProject_Add(
  ext_kcmutils
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kcmutils-5.56.0.tar.xz
  URL_MD5 9507cf7bd35be9c5435302e41e2f7a99
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kitemviews ext_kconfigwidgets ext_kcoreaddons ext_ki18n ext_kiconthemes ext_kservice ext_kxmlgui ext_kdeclarative
)

ExternalProject_Add(
  ext_kimageformats
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.56/kimageformats-5.56.0.tar.xz
  URL_MD5 540cd0efb02b1302bdca5cc6aa16f468
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  DEPENDS ext_kitemviews
)