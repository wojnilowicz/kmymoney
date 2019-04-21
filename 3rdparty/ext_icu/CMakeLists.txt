ExternalProject_Add(
  ext_icu
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://download.icu-project.org/files/icu4c/64.2/icu4c-64_2-src.tgz
  URL_MD5 a3d18213beec454e3cdec9a3116d6b05

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0004-move-to-bin.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0007-actually-move-to-bin.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0008-data-install-dir.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0009-fix-bindir-in-config.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0010-msys-rules-for-makefiles.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0012-libprefix.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0014-mingwize-pkgdata.mingw.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0016-icu-pkgconfig.patch
        COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0017-icu-config-versioning.patch

  CONFIGURE_COMMAND <SOURCE_DIR>/source/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --build=${AT_SYSTEM_TYPE}
    --host=${AT_SYSTEM_TYPE}
    --with-data-packaging=library
    --disable-draft
    --enable-rpath
    --disable-extras
    --disable-icuio
    --enable-icu-config
    --disable-tests
    --disable-samples

  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}
)
