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
  URL http://download.kde.org/stable/frameworks/5.55/extra-cmake-modules-5.55.0.tar.xz
  URL_MD5 f8efb80c1c3c4bfe3ada218fa1eb6daa
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
)

ExternalProject_Add(
  ext_breezeicons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/breeze-icons-5.55.0.tar.xz
  URL_MD5 6830eded1a3222a3dd91ba16d9fd8f60
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBINARY_ICONS_RESOURCE=ON
             -DSKIP_INSTALL_ICONS=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_karchive
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/karchive-5.55.0.tar.xz
  URL_MD5 9feb5965a5438ea7900b10b6734b6b5d
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

if(MINGW)
  set(project_dependencies ext_gperf)
else()
  set(project_dependencies "" )
endif ()

ExternalProject_Add(
  ext_kcodecs
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kcodecs-5.55.0.tar.xz
  URL_MD5 9cf8636f8ecd5699dc4cff2cd003bdaa
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ${project_dependencies}
)

ExternalProject_Add(
  ext_kconfig
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kconfig-5.55.0.tar.xz
  URL_MD5 83c4f3c39797bb014ae3d8addda3ee84
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kdbusaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kdbusaddons-5.55.0.tar.xz
  URL_MD5 fbcc4a6e4daaf8dbbb623c8e58f48748
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kcoreaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kcoreaddons-5.55.0.tar.xz
  URL_MD5 b84b0d269fbbf63dba84ba7b345ab77f
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kguiaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kguiaddons-5.55.0.tar.xz
  URL_MD5 86cdc362ac9b086762725ce50ad02254
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_ki18n
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/ki18n-5.55.0.tar.xz
  URL_MD5 e22ba2c4be9a642d52202941f311092f
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_gettext
)

ExternalProject_Add(
  ext_kitemmodels
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kitemmodels-5.55.0.tar.xz
  URL_MD5 045f5c3b771e03c4d7d3ed8a61cce555
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kitemviews
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kitemviews-5.55.0.tar.xz
  URL_MD5 57f6924d2811f47b7805529f66a74f8b
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kwidgetsaddons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kwidgetsaddons-5.55.0.tar.xz
  URL_MD5 cec11096084112885bb70c8af54b6c18
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

if (APPLE)
  set(project_dependencies ext_qtmacextras )
elseif(UNIX)
  set(project_dependencies ext_qtx11extras )
elseif(MINGW)
#  set(project_dependencies ext_qtwinextras ext_kdewin )
  set(project_dependencies ext_kdewin )
endif ()

ExternalProject_Add(
  ext_kwindowsystem
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kwindowsystem-5.55.0.tar.xz
  URL_MD5 47b9fdf114802c1618be3a808d3fb5f9
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ${project_dependencies}
)

if (APPLE)
  set(project_dependencies ext_bison )
else(UNIX)
  set(project_dependencies "" )
endif ()

ExternalProject_Add(
  ext_solid
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/solid-5.55.0.tar.xz
  URL_MD5 d9d13af96a11f19b68a8787b07a9a8c4
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ${project_dependencies}
)

ExternalProject_Add(
  ext_sonnet
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/sonnet-5.55.0.tar.xz
  URL_MD5 9c2820d220904d527568ac3fd0ef901a
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_syntaxhighlighting
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/syntax-highlighting-5.55.0.tar.xz
  URL_MD5 c06977ee3f710c38da1bef4d5751fd7a
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_kholidays
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.kde.org/stable/frameworks/5.55/kholidays-5.55.0.tar.xz
  URL_MD5 a28376dc7e56982df710307bef1a63e8
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

# without "-DKACTIVITIES_LIBRARY_ONLY=ON" ext_boost is required
ExternalProject_Add(
  ext_kactivities
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kactivities-5.55.0.tar.xz
  URL_MD5 e643d57385e5bddb12cba895af11f93e
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DKACTIVITIES_LIBRARY_ONLY=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kauth
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kauth-5.55.0.tar.xz
  URL_MD5 9caa8c9db684df909a404c987b20bcd2
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons
)

ExternalProject_Add(
  ext_kcompletion
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kcompletion-5.55.0.tar.xz
  URL_MD5 51cfb4f5aafea5e85fb962da96e7eac3
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwidgetsaddons ext_kconfig
)

ExternalProject_Add(
  ext_kcrash
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kcrash-5.55.0.tar.xz
  URL_MD5 44f4b49602863fcfbaa2b142d913ebe2
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kjobwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kjobwidgets-5.55.0.tar.xz
  URL_MD5 550af897cf2b536c0b3370c414f2e607
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_phonon
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://download.kde.org/stable/phonon/4.10.2/phonon-4.10.2.tar.xz
  URL_MD5 ed828952beaa140d1c025513febd8e5d
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
              -DPHONON_BUILD_PHONON4QT5=ON
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules
)

ExternalProject_Add(
  ext_knotifications
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/knotifications-5.55.0.tar.xz
  URL_MD5 c6f26a73d4ea4835afbc07dd30f24f1b
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kwindowsystem ext_kconfig ext_kcodecs ext_kcoreaddons ext_phonon
)

ExternalProject_Add(
  ext_kpackage
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kpackage-5.55.0.tar.xz
  URL_MD5 d49e0c95cc12e210705f843b8c8ae624
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons
)

ExternalProject_Add(
  ext_kconfigwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kconfigwidgets-5.55.0.tar.xz
  URL_MD5 5f36ce8b06f046d14309afba6307531d
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kauth ext_kcoreaddons ext_kcodecs ext_kconfig ext_kguiaddons ext_ki18n ext_kwidgetsaddons
)

ExternalProject_Add(
  ext_kiconthemes
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kiconthemes-5.55.0.tar.xz
  URL_MD5 fcabfaf2a08be0bee4317dd7b48a46ac
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_ki18n ext_kcoreaddons ext_kconfigwidgets ext_kwidgetsaddons ext_kitemviews
)

ExternalProject_Add(
  ext_kservice
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kservice-5.55.0.tar.xz
  URL_MD5 2b7a006d241718cb3c411a42c67c84b4
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_ki18n
)

ExternalProject_Add(
  ext_kemoticons
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kemoticons-5.55.0.tar.xz
  URL_MD5 a088d5a2316ce177c17823a7383216e3
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_karchive ext_kconfig ext_kservice ext_kcoreaddons
)

ExternalProject_Add(
  ext_ktextwidgets
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/ktextwidgets-5.55.0.tar.xz
  URL_MD5 104ea71b7a1495649a6c2b10ec76d49a
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_kcompletion ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_kservice ext_kwidgetsaddons ext_kwindowsystem ext_sonnet
)

ExternalProject_Add(
  ext_kglobalaccel
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kglobalaccel-5.55.0.tar.xz
  URL_MD5 b7efe78d929ce60c2f6d6f91ffb99da5
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcrash ext_kdbusaddons ext_kwindowsystem
)

ExternalProject_Add(
  ext_kxmlgui
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kxmlgui-5.55.0.tar.xz
  URL_MD5 ee8eb1f361e86568f8d63e3eef52c3d9
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kcoreaddons ext_kitemviews ext_kconfig ext_kconfigwidgets ext_ki18n ext_kiconthemes ext_ktextwidgets ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel
)

ExternalProject_Add(
  ext_kbookmarks
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kbookmarks-5.55.0.tar.xz
  URL_MD5 2c6edfead8cbea6eb513b69bd81f352f
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_kcoreaddons ext_kcodecs ext_kconfigwidgets ext_kiconthemes ext_kwidgetsaddons ext_kxmlgui
)

ExternalProject_Add(
  ext_kwallet
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kwallet-5.55.0.tar.xz
  URL_MD5 e9aea329a955d888363452ef57dd351b
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_gcrypt ext_gpgme ext_kcoreaddons ext_kconfig ext_kwindowsystem ext_ki18n ext_kconfigwidgets ext_knotifications ext_kservice ext_kwidgetsaddons ext_kiconthemes ext_kdbusaddons
)

ExternalProject_Add(
  ext_kio
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kio-5.55.0.tar.xz
  URL_MD5 5ea8d5b98580eea963e20466fd0949cd
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_extra_cmake_modules ext_karchive ext_kconfig ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kservice ext_solid ext_kwindowsystem ext_kwidgetsaddons ext_kcompletion ext_kconfigwidgets ext_kiconthemes ext_kitemviews ext_kjobwidgets ext_kbookmarks ext_kwallet
)

ExternalProject_Add(
  ext_kdeclarative
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kdeclarative-5.55.0.tar.xz
  URL_MD5 df0b9df0c1241debaeae2acc634ce917
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kconfig ext_ki18n ext_kiconthemes ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kglobalaccel ext_kguiaddons ext_kpackage
)

ExternalProject_Add(
  ext_kcmutils
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kcmutils-5.55.0.tar.xz
  URL_MD5 b6903266cb40093a903e3f745c0b45dc
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews ext_kconfigwidgets ext_kcoreaddons ext_ki18n ext_kiconthemes ext_kservice ext_kxmlgui ext_kdeclarative
)

ExternalProject_Add(
  ext_kimageformats
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.kde.org/stable/frameworks/5.55/kimageformats-5.55.0.tar.xz
  URL_MD5 6d89478ffa273724e47d937cf88715ef
  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_kitemviews
)
