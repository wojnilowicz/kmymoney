set(sqlite_cFlags "-DSQLITE_ENABLE_COLUMN_METADATA=1 -DSQLITE_DISABLE_DIRSYNC=1 -DSQLITE_SECURE_DELETE=1 -DSQLITE_ENABLE_UNLOCK_NOTIFY=1 -DSQLITE_ENABLE_DBSTAT_VTAB=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1")

ExternalProject_Add(
  ext_sqlite
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://www.sqlite.org/2019/sqlite-autoconf-3270200.tar.gz
  URL_MD5 1f72631ce6e8efa5b4a6e55a43b3bdc0

  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${CMAKE_INSTALL_PREFIX} --disable-static --enable-shared --enable-fts3 --disable-static-shell --enable-session --enable-json1 CFLAGS=${sqlite_cFlags}
  BUILD_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT}
  INSTALL_COMMAND ${MAKE_EXECUTABLE} -j${CMAKE_CPU_COUNT} install
)
