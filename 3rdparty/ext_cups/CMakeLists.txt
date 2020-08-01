#  *
#  * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
#  *
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License as
#  * published by the Free Software Foundation; either version 2 of
#  * the License, or (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  *

ExternalProject_Add(
  ext_cups
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/apple/cups/releases/download/v2.3.3/cups-2.3.3-source.tar.gz
  URL_MD5 412434ceefbdf3ec71bc9188a035f589

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
  BUILD_IN_SOURCE 1

  ${CLEANUP_COMMAND}
)
