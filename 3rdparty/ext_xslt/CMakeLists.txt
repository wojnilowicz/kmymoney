ExternalProject_Add(
  ext_xslt
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL http://xmlsoft.org/sources/libxslt-1.1.33.tar.gz
  URL_MD5 b3bd254a03e46d58f8ad1e4559cd2c2f

  CONFIGURE_COMMAND <SOURCE_DIR>/configure
    ${AT_GLOBAL_PROFILE}
    --without-python
    --without-debug
    --without-debugger

  ${CLEANUP_COMMAND}

  DEPENDS ext_xml ext_gcrypt
)
