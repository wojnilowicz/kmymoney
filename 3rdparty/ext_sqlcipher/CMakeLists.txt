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

# Without LD_FLAGS "Library crypto not found. Install openssl!""
# Without CPPFLAGS "openssl/rand.h: No such file or directory"
# -DHAVE_USLEEP=1 is auto-appended
# Without "$(MAKE) -j1" lemon is not found
# SQLITE_OMIT_DECLTYPE is required by Qt for sqlite3_column_decltype16() 
# SQLITE_OMIT_FLOATING_POINT is required by Qt for sqlite3_column_double
# SQLITE_OMIT_UTF16 is required by Qt for sqlite3_column_text16()
# SQLITE_OMIT_INCRBLOB is required by SQLCipher for sqlite3_blob_open
# speed up guide https://www.sqlite.org/footprint.html
# SQLITE_OMIT_VIRTUALTABLE is required by SQLCipher for sqlite3_declare_vtab
# SQLITE_OMIT_AUTHORIZATION is required by SQLCipher for sqlite3_set_authorizer

set (SQLCIPHER_CFLAGS "$ENV{CFLAGS} \
-DSQLITE_HAS_CODEC \
-DSQLITE_ENABLE_COLUMN_METADATA \
-DSQLITE_ENABLE_FTS3_PARENTHESIS \
-DSQLITE_ENABLE_UNLOCK_NOTIFY \
-DSQLITE_HAVE_ZLIB \
-DSQLITE_LIKE_DOESNT_MATCH_BLOBS \
-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 \
-DSQLITE_OMIT_ANALYZE \
-DSQLITE_OMIT_ATTACH \
-DSQLITE_OMIT_BLOB_LITERAL \
-DSQLITE_OMIT_CAST \
-DSQLITE_OMIT_COMPILEOPTION_DIAGS \
-DSQLITE_OMIT_COMPLETE \
-DSQLITE_OMIT_COMPOUND_SELECT \
-DSQLITE_OMIT_CTE \
-DSQLITE_OMIT_DATETIME_FUNCS \
-DSQLITE_OMIT_DEPRECATED \
-DSQLITE_OMIT_EXPLAIN \
-DSQLITE_OMIT_GENERATED_COLUMNS \
-DSQLITE_OMIT_GET_TABLE \
-DSQLITE_OMIT_INTROSPECTION_PRAGMAS \
-DSQLITE_OMIT_LOAD_EXTENSION \
-DSQLITE_OMIT_PROGRESS_CALLBACK \
-DSQLITE_OMIT_REINDEX \
-DSQLITE_OMIT_TCL_VARIABLE \
-DSQLITE_OMIT_TEMPDB \
-DSQLITE_OMIT_TRACE \
-DSQLITE_OMIT_TRIGGER"
)

ExternalProject_Add(
  ext_sqlcipher
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/sqlcipher/sqlcipher/archive/v4.4.0.tar.gz
  URL_MD5 f25403879dd30ad990221c052c9dfb20

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
