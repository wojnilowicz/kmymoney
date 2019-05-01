if(MINGW)
  set(platform_specific_dir win)
else()
  set(platform_specific_dir unix)
endif()

ExternalProject_Add(
  ext_tcl
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/tcltk/tcl/archive/core-8-6-9.zip
  URL_MD5 f402cc573e6b486b65bda0dcbd74000a

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/do-not-use-precompiled-zlib.diff

  CONFIGURE_COMMAND <SOURCE_DIR>/${platform_specific_dir}/configure
    ${AT_GLOBAL_PROFILE}
    --enable-threads
    --enable-64bit
    
  ${CLEANUP_COMMAND}
)

if(MINGW)
  ExternalProject_Add_Step(
    ext_tcl correcttclsh
    COMMAND cp -f tclsh86.exe ${UNIX_INSTALL_PREFIX}/bin/tclsh.exe

    DEPENDEES build
    DEPENDERS install
    WORKING_DIRECTORY <BINARY_DIR>
  )
endif()