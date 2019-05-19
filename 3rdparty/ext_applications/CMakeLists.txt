set(CMAKE_APPLICATIONS_GLOBAL_PROFILE ${CMAKE_GLOBAL_PROFILE}
                                                                        -DBUILD_TESTING=FALSE
                                                                        $<$<PLATFORM_ID:Darwin>:-DAPPLE_SUPPRESS_X11_WARNING=ON>
)

#ExternalProject_Add(
#  ext_akonadi
#  ${PROJECT_GLOBAL_PROFILE}
#  URL https://download.kde.org/stable/applications/19.04.0/src/akonadi-19.04.0.tar.xz
#  URL_MD5 28d4caac890840df6e6197c6fa443dc2

#  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
#  ${CLEANUP_COMMAND}

#  DEPENDS ext_sharedmimeinfo ext_xslt ext_kcompletion ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kiconthemes ext_kitemmodels ext_kitemviews ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kxmlgui ext_kcrash ext_boost
#)

ExternalProject_Add(
  ext_kpimtextedit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.0/src/kpimtextedit-19.04.0.tar.xz
  URL_MD5 13199f2e20b3686d35cde3e5139f6731

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcodecs ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kemoticons ext_ki18n ext_kiconthemes ext_kio ext_sonnet ext_syntaxhighlighting ext_kwidgetsaddons ext_kxmlgui ext_grantlee
)

ExternalProject_Add(
  ext_kidentitymanagement
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.0/src/kidentitymanagement-19.04.0.tar.xz
  URL_MD5 e3ed4155f6c4e5a700e104dd7b0556f4

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcoreaddons ext_kcompletion ext_ktextwidgets ext_kxmlgui ext_kio ext_kconfig ext_kcodecs ext_kiconthemes ext_kpimtextedit
)

ExternalProject_Add(
  ext_kcontacts
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.0/src/kcontacts-19.04.0.tar.xz
  URL_MD5 2b347429c3b49749bed2df411ebd0e65

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_kcoreaddons ext_ki18n ext_kconfig ext_kcodecs
)
