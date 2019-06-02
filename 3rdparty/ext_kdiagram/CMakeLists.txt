# silence a lot of warnings on MacOS
if(NOT MINGW)
  set(platform_specific_option LOG_BUILD 1)
else()
  set(platform_specific_option "")
endif()

ExternalProject_Add(
  ext_kdiagram
  ${PROJECT_GLOBAL_PROFILE}
  ${platform_specific_option}
  URL https://download.kde.org/stable/kdiagram/2.6.1/kdiagram-2.6.1.tar.xz
  URL_MD5 d0f8624a2decad2ba6a871e13799da23

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_TESTING=FALSE
  ${CLEANUP_COMMAND}

  DEPENDS ext_qtsvg
)