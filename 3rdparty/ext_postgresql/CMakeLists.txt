# client-only installation is described on https://www.postgresql.org/docs/11/install-procedure.html
# without PKG_CONFIG_PATH set, backward slashes are used and that causes build errors
# without MAKELEVEL=0 set, some headers cannot be found
# building src/common and src/port is workaround for mingw
# fails on MacOS, Linux and MS Windows if LOG_BUILD is set to 1

# otherwise "cannot find -lpgcommon"
if(MINGW)
  set(platform_specific_build   BUILD_COMMAND $(MAKE) -C src/common MAKELEVEL=0
                                                            COMMAND $(MAKE) -C src/port MAKELEVEL=0)
else()
  set(platform_specific_build   BUILD_COMMAND "")
endif()

ExternalProject_Add(
  ext_postgresql
  ${PROJECT_GLOBAL_PROFILE}

  URL https://ftp.postgresql.org/pub/source/v11.3/postgresql-11.3.tar.bz2
  URL_MD5 c2a729b754b8de86a969c86ec25db076

  ${platform_specific_patch}
  CONFIGURE_COMMAND ./configure
                    ${AT_GLOBAL_PROFILE}
                    --disable-nls
                    --disable-debug
                    --disable-profiling
                    --disable-cassert
                    --disable-coverage
                    --with-icu
                    --without-tcl
                    --without-perl
                    --without-python
                    --without-gssapi
                    --enable-thread-safety
                    --without-readline
                    --without-bonjour
                    --with-openssl
                    --without-selinux
                    --without-systemd
                    --with-libraries=${UNIX_INSTALL_PREFIX}/lib
                    --with-includes=${UNIX_INSTALL_PREFIX}/include
                    --without-libxml
                    --without-libxslt
                    --with-zlib
                    --disable-dtrace
                    PKG_CONFIG_PATH=${UNIX_INSTALL_PREFIX}/lib/pkgconfig
                    CXXFLAGS=${CXXFLAGS//-w/} # if "-w" is given then "compiler does not report undeclared identifiers" during configuration
                    CFLAGS=${CFLAGS//-w/}

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/build-connector-only.diff
  ${platform_specific_build}
  COMMAND $(MAKE) -C src/bin MAKELEVEL=0 install
  COMMAND $(MAKE) -C src/include MAKELEVEL=0 install
  COMMAND $(MAKE) -C src/interfaces MAKELEVEL=0 install
  INSTALL_COMMAND ""

  BUILD_IN_SOURCE 1
  ${CLEANUP_COMMAND}
 DEPENDS ext_icu ext_openssl
)


