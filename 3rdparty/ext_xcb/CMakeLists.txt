ExternalProject_Add(
  ext_xcb-proto
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-proto-1.13.tar.bz2
  URL_MD5 abe9aa4886138150bbc04ae4f29b90e3
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}

  ${CLEANUP_COMMAND}
)

ExternalProject_Add(
  ext_xcb
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/libxcb-1.13.tar.bz2
  URL_MD5 c2b6cf928afa16b0047c974e7aaa783f

  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --disable-composite
    --disable-damage
    --disable-dpms
    --disable-dri2
    --disable-dri3
    --disable-ge
    --enable-glx
    --enable-present
    --enable-randr
    --disable-record
    --enable-render
    --disable-resource
    --disable-screensaver
    --enable-shape
    --enable-shm
    --enable-sync
    --disable-xevie
    --enable-xfixes
    --disable-xfree86-dri
    --enable-xinerama
    --enable-xinput
    --enable-xkb
    --disable-xprint
    --disable-selinux
    --disable-xtest
    --disable-xv
    --disable-xvmc
    --without-doxygen
    --without-launchd

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-proto
)

ExternalProject_Add(
  ext_xcb-util
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-util-0.4.0.tar.bz2
  URL_MD5 2e97feed81919465a04ccc71e4073313
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb
)

ExternalProject_Add(
  ext_xcb-util-image
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-util-image-0.4.0.tar.bz2
  URL_MD5 08fe8ffecc8d4e37c0ade7906b3f4c87
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-util
)

ExternalProject_Add(
  ext_xcb-util-keysyms
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-util-keysyms-0.4.0.tar.bz2
  URL_MD5 1022293083eec9e62d5659261c29e367
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-util
)

ExternalProject_Add(
  ext_xcb-util-renderutil
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-util-renderutil-0.3.9.tar.bz2
  URL_MD5 468b119c94da910e1291f3ffab91019a
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-util
)

ExternalProject_Add(
  ext_xcb-util-wm
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xcb.freedesktop.org/dist/xcb-util-wm-0.4.1.tar.bz2
  URL_MD5 87b19a1cd7bfcb65a24e36c300e03129
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-devel-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-util
)

ExternalProject_Add(
  ext_xkbcommon
  ${PROJECT_GLOBAL_PROFILE}
  URL https://xkbcommon.org/download/libxkbcommon-0.8.4.tar.xz
  URL_MD5 3c4409058dfd203f641a563358e0187d
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${AT_CONFIGURE_ARGS}
    ${AT_GLOBAL_PROFILE}
    --disable-selective-werror
    --disable-strict-compilation
    --disable-docs
    --without-doxygen

  ${CLEANUP_COMMAND}
  DEPENDS ext_xcb-util-keysyms
)