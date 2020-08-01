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

set(CMAKE_APPLICATIONS_GLOBAL_PROFILE ${CMAKE_GLOBAL_PROFILE}
                                                                        -DBUILD_TESTING=FALSE
                                                                        $<$<PLATFORM_ID:Darwin>:-DAPPLE_SUPPRESS_X11_WARNING=ON>
)

#ExternalProject_Add(
#  ext_akonadi
#  ${PROJECT_GLOBAL_PROFILE}
#  URL https://download.kde.org/stable/release-service/20.04.2/src/akonadi-20.04.2.tar.xz
#  URL_MD5 13062ed9f540e2e5cae42b9a249886da

#  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
#  ${CLEANUP_COMMAND}

#  DEPENDS ext_sharedmimeinfo ext_xslt ext_kcompletion ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kdbusaddons ext_ki18n ext_kiconthemes ext_kitemmodels ext_kitemviews ext_kio ext_kwidgetsaddons ext_kwindowsystem ext_kxmlgui ext_kcrash ext_boost
#)

ExternalProject_Add(
  ext_kpimtextedit
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/release-service/20.04.2/src/kpimtextedit-20.04.2.tar.xz
  URL_MD5 af46d94e16e348bb60af93f5d8ae4ad9

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcodecs ext_kconfig ext_kconfigwidgets ext_kcoreaddons ext_kemoticons ext_ki18n ext_kiconthemes ext_kio ext_sonnet ext_syntaxhighlighting ext_kwidgetsaddons ext_kxmlgui ext_grantlee
)

ExternalProject_Add(
  ext_kidentitymanagement
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/release-service/20.04.2/src/kidentitymanagement-20.04.2.tar.xz
  URL_MD5 a8c6e7733a88dadf5987c9fbc18a6ec2

  CMAKE_ARGS ${CMAKE_APPLICATIONS_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}

 DEPENDS ext_kcoreaddons ext_kcompletion ext_ktextwidgets ext_kxmlgui ext_kio ext_kconfig ext_kcodecs ext_kiconthemes ext_kpimtextedit
)
