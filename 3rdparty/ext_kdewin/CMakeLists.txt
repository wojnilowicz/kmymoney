ExternalProject_Add( ext_kdewin
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/KDE/kdewin/archive/v0.6.3.zip
  URL_MD5 a135e8791669505def656132a8c91e45

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/kdewin.diff

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${BUILD_IN_SOURCE}
  ${CLEANUP_COMMAND}
  DEPENDS ext_qtbase
)
