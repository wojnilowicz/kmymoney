# Without CPPFLAGS "error: libintl.h: No such file or directory" on MinGW
# Without LDFLAGS "Undefined symbols for architecture x86_64: _libintl_bind_textdomain_codeset" on MacOS

if(MINGW)
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/help-setting-up-qt5.diff)
  set(platform_specific_options
    --with-plugin-searchdir=bin/gwenhywfar
    --with-data-searchdir=bin/data/gwenhywfar
    --with-sysdata-searchdir=bin/data
    --with-locale-searchdir=bin/data/locale
  )
elseif(APPLE)
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gwenhywfar-4.20.0-20180503.diff.txt)
  set(platform_specific_options
    --with-plugin-searchdir=PlugIns/gwenhywfar
    --with-data-searchdir=Resources/gwenhywfar
    --with-sysdata-searchdir=Resources
    --with-locale-searchdir=Resources/locale
  )
else()
  set(platform_specific_patch "")
  set(platform_specific_options
    --with-plugin-searchdir=plugins/gwenhywfar
    --with-data-searchdir=share/gwenhywfar
    --with-sysdata-searchdir=share
    --with-locale-searchdir=share/locale
  )
endif()

if (APPLE OR MINGW)
ExternalProject_Add(
  ext_gwenhywfar
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/10/gwenhywfar-4.20.0.tar.gz
  URL_MD5 0744255cae68adca6dff81d3a9f3932d

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/build-qt5-as-dll.diff
  ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --build=${AT_SYSTEM_TYPE}
    $<$<NOT:$<PLATFORM_ID:Linux>>:--host=${AT_SYSTEM_TYPE}>
#     --host=${AT_SYSTEM_TYPE}
    --disable-static
    --enable-shared
    --enable-silent-rules
    $<$<PLATFORM_ID:Windows>:--disable-binreloc> # otherwise "undefined reference to `S_ISLNK'" on MS WIndows
    --enable-local-install
    --disable-debug
    --disable-warnings
    --with-guis=qt5
    --disable-ssl
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gnutls ext_gcrypt
)

else()
ExternalProject_Add(
  ext_gwenhywfar
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/10/gwenhywfar-4.20.0.tar.gz
  URL_MD5 0744255cae68adca6dff81d3a9f3932d

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/build-qt5-as-dll.diff
#               COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gwenhywfar-4.20.0-20180503.diff.txt
  ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --build=${AT_SYSTEM_TYPE}
    $<$<NOT:$<PLATFORM_ID:Linux>>:--host=${AT_SYSTEM_TYPE}>
    --disable-static
    --enable-shared
    --enable-silent-rules
    --enable-local-install
    --disable-debug
    --disable-warnings
    --with-guis=qt5
    --disable-ssl
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}

  DEPENDS ext_gnutls ext_gcrypt
)
endif()

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
elseif(APPLE)
  ExternalProject_Add_Step(
    ext_gwenhywfar correctcmake
    COMMAND bash -c "find ./gwen* -type f -name gwen*config.cmake -exec sed -i '' \
    -e 's+libgwengui-cpp.so+libgwengui-cpp.dylib+g' \
    -e 's+libgwengui-qt5.so+libgwengui-qt5.dylib+g' \
    -e 's+libgwenhywfar.so.\\([0-9]*\\)+libgwenhywfar.\\1.dylib+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
endif()
