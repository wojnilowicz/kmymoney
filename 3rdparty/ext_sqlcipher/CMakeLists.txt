# Without LD_FLAGS "Library crypto not found. Install openssl!""
# Without CPPFLAGS "openssl/rand.h: No such file or directory"
# -DHAVE_USLEEP=1 is auto-appended
# Without "$(MAKE) -j1" lemon is not found
set (SQLCIPHER_CFLAGS "${CFLAGS} -DSQLITE_HAS_CODEC -DSQLITE_ENABLE_COLUMN_METADATA=1 -DSQLITE_OMIT_LOAD_EXTENSION=1 -DSQLITE_OMIT_COMPLETE=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_UNLOCK_NOTIFY=1")

ExternalProject_Add(
  ext_sqlcipher
  ${PROJECT_GLOBAL_PROFILE}
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
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}
    CFLAGS=${SQLCIPHER_CFLAGS}
    
  BUILD_COMMAND $(MAKE) -j1
  ${CLEANUP_COMMAND}

  DEPENDS ext_tcl ext_openssl
)
