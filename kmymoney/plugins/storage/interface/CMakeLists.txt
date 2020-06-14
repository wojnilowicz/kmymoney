set(kmm_istorage_SOURCES
  iplugin.cpp
  idialogpartopensave.cpp
  iurl.cpp
  ui.cpp
  iquestioner.cpp
  imessagebox.cpp
  imessage.cpp
  issuesprocessor.cpp
  wallet.cpp
  widget/messagebox.cpp
)

if(ENABLE_QUICK)
  list(APPEND kmm_istorage_SOURCES
    quick/messagebox.cpp
  )

  qt5_add_resources(kmm_istorage_SOURCES quick/resources.qrc)
endif()

add_library(kmm_istorage SHARED ${kmm_istorage_SOURCES})
generate_export_header(kmm_istorage)

target_link_libraries(kmm_istorage
  PUBLIC
    kmm_plugin
    $<$<BOOL:${ENABLE_QUICK}>:Qt5::Qml>
    Qt5::Widgets
    
  PRIVATE
    KF5::Wallet
)

set_target_properties(kmm_istorage PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS kmm_istorage
  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )
