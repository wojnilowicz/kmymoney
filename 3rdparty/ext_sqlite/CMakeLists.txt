# sqlite3 configuration set according to Qt 3rdparty build script
# withouth CPPFLAGS and LDFLAGS static zlib from mingw is picked up

set(SQLITE_C_FLAGS "${C_FLAGS} -DSQLITE_ENABLE_COLUMN_METADATA=1 -DSQLITE_OMIT_LOAD_EXTENSION=1 -DSQLITE_OMIT_COMPLETE=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_UNLOCK_NOTIFY=1")

if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_sqlite
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.sqlite.org/2019/sqlite-autoconf-3280000.tar.gz
  URL_MD5 3c68eb400f8354605736cd55400e1572

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    --prefix=${UNIX_INSTALL_PREFIX}${install_suffix}
    --build=${AT_SYSTEM_TYPE}
    --host=${AT_SYSTEM_TYPE}
    --disable-static
    --enable-shared
    --enable-silent-rules
    $<$<BOOL:$ENV{CI}>:--disable-dependency-tracking>
    --enable-fts3
    --enable-fts5
    --enable-json1
    --enable-rtree
    --disable-static-shell
    --disable-session
    CFLAGS=${SQLITE_C_FLAGS}
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}

  ${CLEANUP_COMMAND}
)
