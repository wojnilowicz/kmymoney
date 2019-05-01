set(ofx_openSPIncludeDir "${UNIX_INSTALL_PREFIX}/include/OpenSP")
set(ofx_openSPLibDir "${UNIX_INSTALL_PREFIX}/lib")

ExternalProject_Add(
  ext_ofx
  ${PROJECT_GLOBAL_PROFILE}
  URL http://downloads.sourceforge.net/project/libofx/libofx/libofx-0.9.14.tar.gz
  URL_MD5 b92b35b6f90e2a9d693367af397670f9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-doxygen
    --disable-html-docs
    --with-opensp-includes=${ofx_openSPIncludeDir}
    --with-opensp-libs=${ofx_openSPLibDir}

  ${CLEANUP_COMMAND}

  DEPENDS ext_opensp
)
