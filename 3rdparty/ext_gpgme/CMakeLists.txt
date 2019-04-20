if (APPLE OR MINGW)
  set (project_dependencies ext_glib)
else()
  set (project_dependencies "" )
endif ()

ExternalProject_Add(
  ext_gpgme
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.13.0.tar.bz2
  URL_MD5 e511a0d95c507ab87e713140c82fc7d0

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gpgme-1.1.11-20180620.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --enable-silent-rules
    --enable-languages=cpp,qt
    --disable-gpgconf-test
    --disable-gpg-test
    --disable-gpgsm-test
    --disable-g13-test
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
          COMMAND bash -c "sed -i 's+${UNIX_INSTALL_PREFIX}+${CMAKE_INSTALL_PREFIX}+g' ${UNIX_INSTALL_PREFIX}/lib/cmake/Gpgmepp/GpgmeppConfig.cmake"
  ${CLEANUP_COMMAND}

  DEPENDS ext_assuan2 ext_gcrypt ext_gpgerror ${project_dependencies}
)