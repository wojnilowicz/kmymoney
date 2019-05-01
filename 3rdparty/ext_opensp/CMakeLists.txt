if(MINGW)
  set(platform_specific_patch COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/opensp-mingw.diff)
else()
  set(platform_specific_patch "")
endif()

ExternalProject_Add(
  ext_opensp
  ${PROJECT_GLOBAL_PROFILE}
  URL http://downloads.sourceforge.net/project/openjade/opensp/1.5.2/OpenSP-1.5.2.tar.gz
  URL_MD5 670b223c5d12cee40c9137be86b6c39b

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/opensp-lib-only.diff
        ${platform_specific_patch}

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-maintainer-mode
    --disable-nls
    --disable-xml-messages
    --disable-dtddecl
    --disable-doc-build
    --without-included-gettext
    --without-libintl-prefix

  ${CLEANUP_COMMAND}
)
