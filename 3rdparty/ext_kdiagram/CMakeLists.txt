# silence a lot of warnings on MacOS

ExternalProject_Add(
  ext_kdiagram
  ${PROJECT_GLOBAL_PROFILE}
  URL https://download.kde.org/stable/kdiagram/2.6.1/kdiagram-2.6.1.tar.xz
  URL_MD5 d0f8624a2decad2ba6a871e13799da23

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_TESTING=FALSE
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtsvg
)