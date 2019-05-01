# TextDocument is required by kpimtextedit
ExternalProject_Add(
  ext_grantlee
  ${PROJECT_GLOBAL_PROFILE}
  URL https://github.com/steveire/grantlee/archive/v5.1.0.zip
  URL_MD5 cf959612f577114ad7c9e3456bc41a62

  CMAKE_ARGS ${CMAKE_GLOBAL_PROFILE}
             -DBUILD_TEMPLATES=OFF
             -DBUILD_TEXTDOCUMENT=ON
             -DBUILD_MAIN_PLUGINS=OFF
             -DBUILD_I18N_PLUGIN=OFF
             -DBUILD_SCRIPT_PLUGIN=OFF
             -DBUILD_TESTS=OFF
  ${CLEANUP_COMMAND}
)
