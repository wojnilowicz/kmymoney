# Without CPPFLAGS and LDFLAGS "error: GMP is required"
# "make -j1" prevents "cannot find the library libaqhbci.la or unhandled argument libaqhbci.la"
ExternalProject_Add(
  ext_aqbanking
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.aquamaniac.de/rdm/attachments/download/27/aqbanking-5.7.8.tar.gz
  URL_MD5 6aceeaad3a1000f22f08986e68946db3

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-debug
    --disable-warnings
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  BUILD_COMMAND $(MAKE) -j1
  INSTALL_COMMAND $(MAKE) install
  ${CLEANUP_COMMAND}

  DEPENDS ext_gwenhywfar ext_ktoblzcheck ext_xmlsec1 ext_gmp ext_xslt
)


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
endif()
