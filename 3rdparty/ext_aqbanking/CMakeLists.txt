# Without CPPFLAGS and LDFLAGS "error: GMP is required"
# "make -j1" prevents "cannot find the library libaqhbci.la or unhandled argument libaqhbci.la"
# "make -j1 install" is needed on Linux, otherwise it fails to find libaqhbci.so

if(MINGW)
  set(platform_specific_options
    --with-plugin-searchdir=bin/aqbanking
    --with-data-searchdir=bin/data # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=bin/data
    --with-locale-searchdir=bin/data/locale
  )
elseif(APPLE)
  set(platform_specific_options
    --with-plugin-searchdir=PlugIns/aqbanking
    --with-data-searchdir=Resources # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=Resources
    --with-locale-searchdir=Resources/locale
  )
else()
  set(platform_specific_options
    --with-plugin-searchdir=plugins/aqbanking
    --with-data-searchdir=share # watch out, it differs from gwenhywfar
    --with-sysdata-searchdir=share
    --with-locale-searchdir=share/locale
  )
endif()

if(APPLE OR MINGW)
ExternalProject_Add(
  ext_aqbanking
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/27/aqbanking-5.7.8.tar.gz
  URL_MD5 6aceeaad3a1000f22f08986e68946db3

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-gwenhywfar-test
    $<$<NOT:$<PLATFORM_ID:Linux>>:--enable-local-install>
    --enable-release
    --disable-debug
    --disable-warnings
    --disable-cli
    --disable-tutorials
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  BUILD_COMMAND $(MAKE) -j1
  INSTALL_COMMAND $(MAKE) install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gwenhywfar ext_ktoblzcheck ext_xmlsec1 ext_gmp ext_xslt
)
else()
ExternalProject_Add(
  ext_aqbanking
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/27/aqbanking-5.7.8.tar.gz
  URL_MD5 6aceeaad3a1000f22f08986e68946db3

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-gwenhywfar-test
    --enable-local-install
    --enable-release
    --disable-debug
    --disable-warnings
    --disable-cli
    --disable-tutorials
    ${platform_specific_options}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  BUILD_COMMAND $(MAKE) -j1
  INSTALL_COMMAND $(MAKE) -j1 install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gwenhywfar ext_ktoblzcheck ext_xmlsec1 ext_gmp ext_xslt
)
endif()

if(MINGW)
  ExternalProject_Add_Step(
    ext_aqbanking correctcmake
    COMMAND bash -c "find . -type f -name aqbanking-config.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    -e 's+lib/libaqbanking.so+lib/libaqbanking.dll.a+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
elseif(APPLE)
  ExternalProject_Add_Step(
    ext_aqbanking correctcmake
    COMMAND bash -c "find . -type f -name aqbanking-config.cmake -exec sed -i '' \
    -e 's+lib/libaqbanking.so+lib/libaqbanking.dylib+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
endif()
