if(APPLE)
  set (project_dependencies "")
  set (qt_platform_specific_option "")
elseif(UNIX OR MINGW)
  set (project_dependencies ext_iconv )
  set (qt_platform_specific_option --with-libiconv-prefix=${UNIX_INSTALL_PREFIX})
endif ()

# --disable-nls won't build libintl
ExternalProject_Add(
  ext_gettext
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.19.8.1.tar.gz
  URL_MD5 97e034cf8ce5ba73a28ff6c3c0638092

  PATCH_COMMAND ${PATCH_COMMAND} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-tools.diff
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}
    --host=${AT_SYSTEM_TYPE}
    ${AT_SHARED_ONLY}
    ${AT_LESS_PROCESSING}
    --disable-java
    --disable-native-java
    --disable-curses
    --disable-csharp
    --disable-libasprintf
    --disable-openmp
    --disable-acl
    --without-git
    --without-cvs
    --without-emacs
    --enable-c++
    --enable-relocatable
    --with-included-gettext
#    ${qt_platform_specific_option}
  BUILD_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} ${MAKE_ARGS} install
  ${CLEANUP_COMMAND}

  DEPENDS ext_xml ext_glib ${project_dependencies}
)
