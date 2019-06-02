ExternalProject_Add(
  ext_cups
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/apple/cups/releases/download/v2.2.11/cups-2.2.11-source.tar.gz
  URL_MD5 7afbbcd2497e7d742583c492f6de40cd

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
  --disable-mallinfo
  --disable-libpaper
  --disable-libusb
  --disable-tcp-wrappers
  --disable-acl
  --disable-dbus
  --disable-debug
  --disable-debug-guards
  --disable-debug-printfs
  --disable-unit-tests
  --disable-relro
  --disable-gssapi
  --disable-ssl
  --disable-cdsassl
  --disable-gnutls
  --disable-pam
  --disable-avahi
  --disable-dnssd
  --disable-launchd
  --disable-systemd
  --disable-upstart
  --disable-webif
  --with-components=core
  --without-docdir
  --without-logdir
  --without-systemd
  --without-rcdir
  --without-languages
  --without-java
  --without-perl
  --without-php
  --without-python
  --libdir=${UNIX_INSTALL_PREFIX}/lib
  BUILD_IN_SOURCE 1

  ${CLEANUP_COMMAND}
)
