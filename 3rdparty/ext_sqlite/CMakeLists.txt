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

# sqlite3 configuration set according to Qt 3rdparty build script
# withouth CPPFLAGS and LDFLAGS static zlib from mingw is picked up
# SQLITE_OMIT_ANALYZE for undefined reference to `sqlite3Analyze'
# SQLITE_OMIT_TRIGGER is required to build this library
# SQLITE_OMIT_ATTACH for undefined reference to `sqlite3Attach'
# SQLITE_OMIT_REINDEX for undefined reference to `sqlite3Reindex'
# SQLITE_OMIT_CTE for undefined reference to `sqlite3WithAdd'

set(SQLITE_CFLAGS "$ENV{CFLAGS} \
-DSQLITE_ENABLE_COLUMN_METADATA \
-DSQLITE_ENABLE_FTS3_PARENTHESIS \
-DSQLITE_ENABLE_UNLOCK_NOTIFY \
-DSQLITE_HAVE_ZLIB \
-DSQLITE_LIKE_DOESNT_MATCH_BLOBS \
-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 \
-DSQLITE_OMIT_BLOB_LITERAL \
-DSQLITE_OMIT_CAST \
-DSQLITE_OMIT_COMPILEOPTION_DIAGS \
-DSQLITE_OMIT_COMPLETE \
-DSQLITE_OMIT_COMPOUND_SELECT \
-DSQLITE_OMIT_DATETIME_FUNCS \
-DSQLITE_OMIT_DEPRECATED \
-DSQLITE_OMIT_EXPLAIN \
-DSQLITE_OMIT_GENERATED_COLUMNS \
-DSQLITE_OMIT_GET_TABLE \
-DSQLITE_OMIT_INTROSPECTION_PRAGMAS \
-DSQLITE_OMIT_LOAD_EXTENSION \
-DSQLITE_OMIT_PROGRESS_CALLBACK \
-DSQLITE_OMIT_TCL_VARIABLE \
-DSQLITE_OMIT_TEMPDB \
-DSQLITE_OMIT_TRACE"
)

if(APPLE)
  set(install_suffix /colisionlibs)
else()
  set(install_suffix "")
endif()

ExternalProject_Add(
  ext_sqlite
  ${PROJECT_GLOBAL_PROFILE}
  URL https://www.sqlite.org/2020/sqlite-autoconf-3320300.tar.gz
  URL_MD5 2e3911a3c15e85c2f2d040154bbe5ce3

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
    CPPFLAGS=${AT_CPPFLAGS}
    LDFLAGS=${AT_LDFLAGS}
    CFLAGS=${SQLITE_CFLAGS}

  ${CLEANUP_COMMAND}
)
