if(MINGW)
  set(INSTALL_LIBDIR -DINSTALL_LIBDIR=lib)
  set(INSTALL_PLUGINDIR -DINSTALL_PLUGINDIR=bin/mariadb)
else()
  set(INSTALL_LIBDIR -DINSTALL_LIBDIR=lib)
  set(INSTALL_PLUGINDIR -DINSTALL_PLUGINDIR=plugins/mariadb)
endif()

ExternalProject_Add(
  ext_mariadb
  ${PROJECT_GLOBAL_PROFILE}
  URL https://downloads.mariadb.org/interstitial/connector-c-3.0.9/mariadb-connector-c-3.0.9-src.tar.gz
  URL_MD5 e7fdb17e26e779f4bd15b6a32e1c8472

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/mariadb.diff

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DWITH_EXTERNAL_ZLIB=ON
             -DWITH_MSI=OFF
             -DWITH_RTC=OFF
             -DWITH_SSL=OPENSSL
             -DWITH_MYSQLCOMPAT=ON
             -DWITH_UNIT_TESTS=OFF
             ${INSTALL_LIBDIR}
             ${INSTALL_PLUGINDIR}

  ${CLEANUP_COMMAND}
  DEPENDS ext_openssl
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_mariadb fixLibrariesOnMsys
    COMMAND mv -f ${CMAKE_INSTALL_PREFIX}/lib/libmariadb.dll ${CMAKE_INSTALL_PREFIX}/bin/mariadb.dll
    COMMAND mv -f ${CMAKE_INSTALL_PREFIX}/lib/liblibmariadb.dll.a ${CMAKE_INSTALL_PREFIX}/lib/libmariadb.dll.a
    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
  )
endif()
