set(ofx_openSPIncludeDir "${CMAKE_INSTALL_PREFIX}/include/OpenSP")
set(ofx_openSPLibDir "${CMAKE_INSTALL_PREFIX}/lib")

ExternalProject_Add(
  ext_ofx
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://downloads.sourceforge.net/project/libofx/libofx/libofx-0.9.13.tar.gz
  URL_MD5 dde6276ffe2999f86bed5bf2cc80f7b2

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} ${AT_HOST} ${AT_SHARED_ONLY} --with-opensp-includes=${ofx_openSPIncludeDir} --with-opensp-libs=${ofx_openSPLibDir}
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install

  DEPENDS ext_opensp
)
