set(CMAKE_APPLICATIONS_GLOBAL_PROFILE ${CMAKE_GLOBAL_PROFILE}
                                                                        -DBUILD_TESTING=FALSE
                                                                        $<$<PLATFORM_ID:Darwin>:-DAPPLE_SUPPRESS_X11_WARNING=ON>
)

#ExternalProject_Add(
#  ext_akonadi
#  ${PROJECT_GLOBAL_PROFILE}
#  URL https://download.kde.org/stable/applications/19.04.1/src/akonadi-19.04.1.tar.xz
#  URL_MD5 d41d8cd98f00b204e9800998ecf8427e

#  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
#  ${CLEANUP_COMMAND}

#  DEPENDS ext_sharedmimeinfo ext_xslt ext_kcompletion ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kiconthemes ext_kitemmodels ext_kitemviews ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kxmlgui ext_kcrash ext_boost
#)

ExternalProject_Add(
  ext_kpimtextedit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.1/src/kpimtextedit-19.04.1.tar.xz
  URL_MD5 0f49b6caf016bbd2a7b3ccb0b70670f0

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcodecs ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kemoticons ext_ki18n ext_kiconthemes ext_kio ext_sonnet ext_syntaxhighlighting ext_kwidgetsaddons ext_kxmlgui ext_grantlee
)

ExternalProject_Add(
  ext_kidentitymanagement
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.1/src/kidentitymanagement-19.04.1.tar.xz
  URL_MD5 d5459c53182ee744bbc55f531d1c2082

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcoreaddons ext_kcompletion ext_ktextwidgets ext_kxmlgui ext_kio ext_kconfig ext_kcodecs ext_kiconthemes ext_kpimtextedit
)

ExternalProject_Add(
  ext_kcontacts
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/applications/19.04.1/src/kcontacts-19.04.1.tar.xz
  URL_MD5 e7c766c8a166e73152e5b71b18c74c34

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

  DEPENDS ext_kcoreaddons ext_ki18n ext_kconfig ext_kcodecs
)
