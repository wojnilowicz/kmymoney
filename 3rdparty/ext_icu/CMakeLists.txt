# --enable-icuio is required by mysql
if(MINGW)
  set(platform_specific_patches
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0004-move-to-bin.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0007-actually-move-to-bin.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0008-data-install-dir.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0009-fix-bindir-in-config.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0010-msys-rules-for-makefiles.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0012-libprefix.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0014-mingwize-pkgdata.mingw.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0016-icu-pkgconfig.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/0017-icu-config-versioning.patch
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/icu-custom-data-mingw-fix.diff
    COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/icu-custom-data-mingw-fix2.diff
  )
else()
  set(platform_specific_patches "")
endif()

ExternalProject_Add(
  ext_icu
  ${PROJECT_GLOBAL_PROFILE}
  URL http://download.icu-project.org/files/icu4c/64.2/icu4c-64_2-src.tgz
  URL_MD5 a3d18213beec454e3cdec9a3116d6b05
  ${platform_specific_patches}

  CONFIGURE_COMMAND export ICU_DATA_FILTER_FILE=${CMAKE_CURRENT_SOURCE_DIR}/filters.json &&
    <SOURCE_DIR>/source/configure
    ${AT_GLOBAL_PROFILE}
    --disable-debug
    --enable-release
    --disable-draft
    --enable-rpath
    --disable-extras
    --disable-icuio
    --enable-icu-config
    --disable-tests
    --disable-samples
    --with-data-packaging=library

  ${CLEANUP_COMMAND}
)

ExternalProject_Add_Step(
  ext_icu icudata
  COMMAND wget -nc http://download.icu-project.org/files/icu4c/64.2/icu4c-64_2-data.zip
  COMMAND rm -fr data
  COMMAND ${CMAKE_COMMAND} -E tar xf icu4c-64_2-data.zip
  DEPENDEES download
  DEPENDERS patch
  WORKING_DIRECTORY <SOURCE_DIR>/source
)
