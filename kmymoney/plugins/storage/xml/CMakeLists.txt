set(PLUGIN_ID "newxmlstorage")
set(PLUGIN_NAME "XML storage")
set(PLUGIN_DESCRIPTION "Support for XML based storages")

configure_file(plugin.json.in plugin.json @ONLY)
configure_file(cmakedefine.h.in cmakedefine.h @ONLY)

set(plugin_SOURCES
  plugin.cpp
)

kconfig_add_kcfg_files(plugin_SOURCES configuration.kcfgc)

# === Backend ===
set(backend_SOURCES
  backend/backend.cpp
  backend/url.cpp
  backend/upgrader.cpp
  backend/converter.cpp
  backend/anonymizer.cpp
  backend/domhelper.cpp
  backend/names.cpp
  backend/readerwriter.cpp
)

list(APPEND plugin_SOURCES ${backend_SOURCES})
# === Backend ===

# === Frontend ===
set(frontend_SOURCES
  frontend/opensave.cpp
  frontend/message.cpp
  frontend/questioner.cpp
)

if(ENABLE_GPG)
  list(APPEND frontend_SOURCES
        frontend/saveasencrypted.cpp
        )
endif()

list(APPEND plugin_SOURCES ${frontend_SOURCES})
# === Frontend ===

# === Quick frontend ===
if(ENABLE_QUICK)
  set(quick_SOURCES
    frontend/quick/opensave.cpp
    frontend/quick/questioner.cpp
  )

  qt5_add_resources(quick_SOURCES frontend/quick/resources.qrc)
  
  if(ENABLE_GPG)
    list(APPEND quick_SOURCES
      frontend/quick/saveasencrypted.cpp
      )
  endif()

  list(APPEND plugin_SOURCES ${quick_SOURCES})
endif()
# === Quick frontend ===

# === Widget frontend ===
set(widget_SOURCES
  frontend/widget/opensave.cpp
  frontend/widget/questioner.cpp
)

ki18n_wrap_ui(widget_SOURCES
  frontend/widget/opensave.ui
  )

if(ENABLE_GPG)
  list(APPEND widget_SOURCES
    frontend/widget/saveasencrypted.cpp
    )
    
  ki18n_wrap_ui(widget_SOURCES
    frontend/widget/saveasencrypted.ui
    )
endif()

list(APPEND plugin_SOURCES ${widget_SOURCES})
# === Widget frontend ===


# === Gpg support ===

if(ENABLE_GPG)
# === Gpg backend ===
set(gpgbackend_SOURCES
  gpg/backend/backend.cpp
  gpg/backend/keysmodel.cpp
)

list(APPEND gpgsupport_SOURCES ${gpgbackend_SOURCES})
# === Gpg backend ===

# === Gpg frontend ===
set(gpgfrontend_SOURCES
  gpg/frontend/keysselector.cpp
  gpg/frontend/questioner.cpp
  gpg/frontend/message.cpp
)

if(ENABLE_KHTML)
  list(APPEND gpgfrontend_SOURCES
    gpg/frontend/keydownloader.cpp
    )
endif()

list(APPEND gpgsupport_SOURCES ${gpgfrontend_SOURCES})
# === Gpg frontend ===

# === Gpg widget frontend ===
set(gpgwidget_SOURCES
  gpg/frontend/widget/keysselector.cpp
  gpg/frontend/widget/unusedkeysselector.cpp
  
  gpg/frontend/widget/passphraseprovider.cpp
  gpg/frontend/widget/questioner.cpp
)

ki18n_wrap_ui(gpgwidget_SOURCES
  gpg/frontend/widget/keysselector.ui
  gpg/frontend/widget/unusedkeysselector.ui
  gpg/frontend/widget/passphraseprovider.ui
)

  if(ENABLE_KHTML)
    list(APPEND gpgwidget_SOURCES
      gpg/frontend/widget/keydownloader.cpp
      )
  endif()

list(APPEND gpgsupport_SOURCES ${gpgwidget_SOURCES})
# === Gpg widget frontend ===

# === Gpg Quick frontend ===
if(ENABLE_QUICK)
  set(gpgquick_SOURCES
    gpg/frontend/quick/keysselector.cpp
    gpg/frontend/quick/questioner.cpp
  )

  qt5_add_resources(gpgquick_SOURCES gpg/frontend/quick/gpgresources.qrc)
  if(ENABLE_KHTML)
    list(APPEND gpgquick_SOURCES
      gpg/frontend/quick/keydownloader.cpp
      )
  endif()

  list(APPEND gpgsupport_SOURCES ${gpgquick_SOURCES})
endif()
# === Gpg Quick frontend ===

add_library(gpgsupport STATIC ${gpgsupport_SOURCES})
target_link_libraries(gpgsupport
  PRIVATE
    kmm_istorage
    Qt5::Widgets
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Qml>
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Quick>
    KF5::I18n
    $<$<BOOL:${ENABLE_KHTML}>:KF5::KHtml>
    Gpgmepp
)

target_compile_definitions(gpgsupport PUBLIC -DENABLE_GPG) # it isn't in cmakedefine.h, so that test static library won't pick it up

# === GPG support ===
endif()

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
    Qt5::Xml
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Qml>
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Quick>
    KF5::Archive
    KF5::KIOFileWidgets
    KF5::I18n
    $<$<BOOL:${ENABLE_GPG}>:gpgsupport>
    xmlstoragehelper
    onlinetask_interfaces
)

set(KCM_ID kcm_${PLUGIN_ID})
set(KCM_NAME "XML storages - configuration")
set(KCM_DESCRIPTION "Configuration for XML based storages")
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
    Qt5::Xml
    KF5::Archive
    kmm_mymoney
    kmm_istorage
    xmlstoragehelper
    onlinetask_interfaces
  )
  add_subdirectory(backend/tests)
endif()