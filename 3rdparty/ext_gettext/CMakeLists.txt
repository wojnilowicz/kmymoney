# --disable-nls won't build libintl

if(MINGW)
  # msys2  has gettext tools, so build library only
  set(platform_specific_patch PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-tools.diff)
  set(platform_specific_dependencies "")
else()
  set(platform_specific_patch "")
  set(platform_specific_dependencies ext_glib ext_xml)
endif()

ExternalProject_Add(
  ext_gettext
  ${PROJECT_GLOBAL_PROFILE}
  URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.19.8.1.tar.gz
  URL_MD5 97e034cf8ce5ba73a28ff6c3c0638092

  ${platform_specific_patch}
  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
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
    --enable-c++ # is required by ki18n
    --enable-relocatable
    --with-included-gettext

  ${CLEANUP_COMMAND}
    DEPENDS ${platform_specific_dependencies}
)
