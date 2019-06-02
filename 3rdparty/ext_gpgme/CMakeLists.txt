if(APPLE OR MINGW)
  ExternalProject_Add(
    ext_gpgme
    ${PROJECT_GLOBAL_PROFILE}
    URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.13.0.tar.bz2
    URL_MD5 e511a0d95c507ab87e713140c82fc7d0

    PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gpgme-1.1.11-20180620.diff

    CONFIGURE_COMMAND <SOURCE_DIR>/configure
      ${AT_GLOBAL_PROFILE}
      --enable-languages=cpp,qt
      --disable-gpgconf-test
      --disable-gpg-test
      --disable-gpgsm-test
      --disable-g13-test

    ${CLEANUP_COMMAND}

    DEPENDS ext_assuan2 ext_gcrypt
  )
else()
  # gpgme-1.10 depends on libgpgerror 1.24 and version available in Ubuntu 16.04 is 1.21
  ExternalProject_Add(
    ext_gpgme
    ${PROJECT_GLOBAL_PROFILE}
    URL https://www.gnupg.org/ftp/gcrypt/gpgme/gpgme-1.9.0.tar.bz2
    URL_MD5 1e00bb8ef04d1d05d5a0f19e143854c3

    PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/gpgme-1.1.11-20180620.diff

    CONFIGURE_COMMAND <SOURCE_DIR>/configure
      ${AT_GLOBAL_PROFILE}
      --enable-languages=cpp,qt
      --disable-gpgconf-test
      --disable-gpg-test
      --disable-gpgsm-test
      --disable-g13-test

    ${CLEANUP_COMMAND}

    DEPENDS ext_assuan2 ext_gpgerror
  )
endif()

if(MINGW)
  ExternalProject_Add_Step(
    ext_gpgme correctcmake
    COMMAND bash -c "find . -type f -name GpgmeppConfig.cmake -exec sed -i \
    -e 's+'${UNIX_INSTALL_PREFIX}'+'${CMAKE_INSTALL_PREFIX}'+g' \
    {} \\+"

    DEPENDEES install
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/cmake
  )
endif()
