set(PLUGIN_ID "newsqlstorage")
set(PLUGIN_NAME "SQL storage")
set(PLUGIN_DESCRIPTION "Support for SQL based storages")

configure_file(plugin.json.in plugin.json @ONLY)
configure_file(cmakedefine.h.in cmakedefine.h @ONLY)

set(plugin_SOURCES
  plugin.cpp
)

set(plugin_SOURCES
  plugin.cpp
)

kconfig_add_kcfg_files(plugin_SOURCES configuration.kcfgc)

# === Backend ===
set(backend_SOURCES
  backend/backend.cpp
  backend/url.cpp
  backend/names.cpp
  backend/upgrader.cpp
  backend/converter.cpp
  backend/sqlhelper.cpp
  backend/mymoneystoragesql.cpp
  backend/mymoneydbdef.cpp
  backend/mymoneydbdriver.cpp
  mysql/backend.cpp
  postgresql/backend.cpp
  sqlite/backend.cpp
  sqlcipher/backend.cpp
)

list(APPEND plugin_SOURCES ${backend_SOURCES})
# === Backend ===

# === Frontend ===
set(frontend_SOURCES
  frontend/opensave.cpp
  frontend/opensaveserver.cpp
  frontend/message.cpp
  frontend/questioner.cpp
  frontend/credentials.cpp
)

list(APPEND plugin_SOURCES ${frontend_SOURCES})
# === Frontend ===

# === Quick frontend ===
if(ENABLE_QUICK)
  set(quick_SOURCES
    frontend/quick/opensave.cpp
    frontend/quick/opensaveserver.cpp
    frontend/quick/credentials.cpp
  )

  qt5_add_resources(quick_SOURCES frontend/quick/resources.qrc)

  list(APPEND plugin_SOURCES ${quick_SOURCES})
endif()
# === Quick frontend ===

# === Widget frontend ===
set(widget_SOURCES
  frontend/widget/opensave.cpp
  frontend/widget/opensaveserver.cpp
  frontend/widget/credentials.cpp
)

ki18n_wrap_ui(widget_SOURCES
  frontend/widget/opensave.ui
  frontend/widget/opensaveserver.ui
  frontend/widget/credentials.ui
)

list(APPEND plugin_SOURCES ${widget_SOURCES})
# === Widget frontend ===

kcoreaddons_add_plugin(${PLUGIN_ID}
                       SOURCES ${plugin_SOURCES}
                       JSON "${CMAKE_CURRENT_BINARY_DIR}/plugin.json"
                       INSTALL_NAMESPACE "kmymoney")

set_target_properties(${PLUGIN_ID}
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_link_libraries(${PLUGIN_ID}
  PRIVATE
    kmm_plugin
    kmm_istorage
    Qt5::Sql
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Qml>
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Quick>
    KF5::KIOFileWidgets
    xmlstoragehelper
    onlinetask_interfaces
)

set(KCM_ID kcm_${PLUGIN_ID})
set(KCM_NAME "SQL storages - configuration")
set(KCM_DESCRIPTION "Configuration for SQL based storages")
configure_file(kcm.desktop.in kcm.desktop @ONLY)


set(kcm_SOURCES
  kcm.cpp
)
  
kconfig_add_kcfg_files(kcm_SOURCES configuration.kcfgc)

ki18n_wrap_ui(kcm_SOURCES
  frontend/widget/kcm.ui
)

kcoreaddons_add_plugin(${KCM_ID}
                       SOURCES ${kcm_SOURCES}
                       JSON ${CMAKE_CURRENT_BINARY_DIR}/kcm.json
                       INSTALL_NAMESPACE "kmymoney")

kcoreaddons_desktop_to_json(${KCM_ID} ${CMAKE_CURRENT_BINARY_DIR}/kcm.desktop)

set_target_properties(${KCM_ID}
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_link_libraries(${KCM_ID}
  PRIVATE
    KF5::I18n
    KF5::ConfigWidgets
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kcm.desktop
        DESTINATION ${SERVICES_INSTALL_DIR}
        RENAME ${KCM_ID}.desktop)

if(BUILD_TESTING)
  add_library("${PLUGIN_ID}_backend" STATIC ${backend_SOURCES})
  target_link_libraries("${PLUGIN_ID}_backend"
  PUBLIC
    Qt5::Sql
    kmm_mymoney
    kmm_istorage
    xmlstoragehelper
    onlinetask_interfaces
)
  add_subdirectory(backend/tests)
endif()
