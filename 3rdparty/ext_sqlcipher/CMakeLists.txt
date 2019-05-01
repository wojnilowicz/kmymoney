# without LD_FLAGS "Library crypto not found. Install openssl!""
set (SQLITE_CFLAGS "${CFLAGS} -DSQLITE_HAS_CODEC -DSQLITE_ENABLE_COLUMN_METADATA=1 -DSQLITE_OMIT_LOAD_EXTENSION=1 -DSQLITE_OMIT_COMPLETE=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_UNLOCK_NOTIFY=1")

if(UNIX)
  set(SQLITE_CFLAGS "${SQLITE_CFLAGS} DHAVE_USLEEP=1")
endif()

ExternalProject_Add(
  ext_sqlcipher
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://github.com/sqlcipher/sqlcipher/archive/v4.1.0.tar.gz
  URL_MD5 d159eab5c7dd0349d4ab500490e6f6f9

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --disable-tcl
    --disable-readline
    --disable-debug
    --enable-fts3
    --enable-fts5
    --enable-rtree
    --with-crypto-lib=openssl
    --enable-tempstore=yes
    LDFLAGS=${AT_LDFLAGS}
    CFLAGS=${SQLITE_CFLAGS}
    
  ${CLEANUP_COMMAND}

  DEPENDS ext_tcl
)
