# client-only installation is described on https://www.postgresql.org/docs/11/install-procedure.html
# without PKG_CONFIG_PATH set, backward slashes are used and that causes build errors
# without MAKELEVEL=0 set, some headers cannot be found
# building src/common and src/port is workaround for mingw

if(MINGW)
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/build-connector-only.diff)
  set(platform_specific_build   BUILD_COMMAND $(MAKE) -C src/common MAKELEVEL=0
                                                            COMMAND $(MAKE) -C src/port MAKELEVEL=0)
else()
  set(platform_specific_patch "")
  set(platform_specific_build   BUILD_COMMAND "")
endif()

ExternalProject_Add(
  ext_postgresql
   DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
   DOWNLOAD_NO_PROGRESS 1

  URL https://ftp.postgresql.org/pub/source/v11.2/postgresql-11.2.tar.bz2
  URL_MD5 19d43be679cb0d55363feb8926af3a0f

  ${platform_specific_patch}
  CONFIGURE_COMMAND ./configure
                    ${AT_GLOBAL_PROFILE}
                    --disable-nls
                    --disable-debug
                    --disable-profiling
                    --disable-coverage
                    --with-icu
                    --without-tcl
                    --without-perl
                    --without-python
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
                    PKG_CONFIG_PATH=${UNIX_INSTALL_PREFIX}/lib/pkgconfig

  ${platform_specific_build}
  COMMAND $(MAKE) -C src/bin MAKELEVEL=0 install
  COMMAND $(MAKE) -C src/include MAKELEVEL=0 install
  COMMAND $(MAKE) -C src/interfaces MAKELEVEL=0 install
  INSTALL_COMMAND ""

  BUILD_IN_SOURCE 1
  ${CLEANUP_COMMAND}
  DEPENDS ext_icu ext_openssl
)


