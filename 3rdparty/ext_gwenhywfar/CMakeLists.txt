# Without CPPFLAGS "error: libintl.h: No such file or directory"

if(MINGW)
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/help-setting-up-qt5.diff)
elseif(APPLE)
  set(platform_specific_patch "")
else()
  set(platform_specific_patch "")
endif()

ExternalProject_Add(
  ext_gwenhywfar
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/10/gwenhywfar-4.20.0.tar.gz
  URL_MD5 0744255cae68adca6dff81d3a9f3932d

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/build-qt5-as-dll.diff
  ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-binreloc
    --disable-debug
    --disable-warnings
    --with-guis=qt5
    CPPFLAGS=${AT_CPPFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gnutls ext_gcrypt
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_gwenhywfar correctcmake
    COMMAND bash -c "find ./gwen* -type f -name gwen*config.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    -e 's+lib/libgwengui-cpp.so+lib/libgwengui-cpp.dll.a+g' \
    -e 's+lib/libgwengui-qt5.so+lib/libgwengui-qt5.dll.a+g' \
    -e 's+lib/libgwenhywfar.so.\\([0-9]*\\)+lib/libgwenhywfar.dll.a+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
endif()


