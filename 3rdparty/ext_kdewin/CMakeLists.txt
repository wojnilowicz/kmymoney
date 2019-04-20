ExternalProject_Add( ext_kdewin
  DOWNLOAD_DIR ${EXT_DOWNLOAD_DIR}
  URL https://github.com/KDE/kdewin/archive/v0.6.3.zip
  URL_MD5 a135e8791669505def656132a8c91e45

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
  ${CLEANUP_COMMAND}
)
