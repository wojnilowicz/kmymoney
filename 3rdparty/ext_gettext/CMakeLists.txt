# --disable-nls won't build libintl
ExternalProject_Add(
  ext_gettext
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.19.8.1.tar.gz
  URL_MD5 97e034cf8ce5ba73a28ff6c3c0638092

  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/disable-tools.diff
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
    --enable-c++
    --enable-relocatable
    --with-included-gettext

  ${CLEANUP_COMMAND}

  DEPENDS ext_glib
)
