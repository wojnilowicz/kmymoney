set(PLUGIN_ID "storagemanager")
set(PLUGIN_NAME "Storage manager")
set(PLUGIN_DESCRIPTION "Manager for handling storage plugins")

configure_file(plugin.json.in plugin.json @ONLY)
configure_file(cmakedefine.h.in cmakedefine.h @ONLY)

set(plugin_SOURCES
  plugin.cpp
)

kconfig_add_kcfg_files(plugin_SOURCES configuration.kcfgc)

# === Frontend ===
set(frontend_SOURCES
  frontend/opensave.cpp
  frontend/message.cpp
  frontend/questioner.cpp
)

list(APPEND plugin_SOURCES ${frontend_SOURCES})
# === Frontend ===

# === Quick frontend ===
if(ENABLE_QUICK)
  set(quick_SOURCES
    frontend/quick/opensave.cpp
  )

  qt5_add_resources(quick_SOURCES frontend/quick/resources.qrc)

  list(APPEND plugin_SOURCES ${quick_SOURCES})
endif()
# === Quick frontend ===

# === Widget frontend ===
set(widget_SOURCES
  frontend/widget/opensave.cpp
)

ki18n_wrap_ui(widget_SOURCES
  frontend/widget/opensave.ui
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
    kmm_istorage
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Qml>
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Quick>
    KF5::CoreAddons
)
      

set(KCM_ID kcm_${PLUGIN_ID})
set(KCM_NAME "Storage manager - configuration")
set(KCM_DESCRIPTION "Configuration for storage manager")
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
    kmm_istorage
    KF5::I18n
    KF5::ConfigWidgets
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kcm.desktop
        DESTINATION ${SERVICES_INSTALL_DIR}
        RENAME ${KCM_ID}.desktop)
