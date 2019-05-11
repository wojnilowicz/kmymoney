configure_file(${CMAKE_CURRENT_SOURCE_DIR}/homeview.json.cmake ${CMAKE_CURRENT_BINARY_DIR}/homeview.json @ONLY)

set(homeview_SOURCES
  homeview.cpp
  khomeview.cpp
  kwelcomepage.cpp
  )

kcoreaddons_add_plugin(homeview
                       SOURCES ${homeview_SOURCES}
                       JSON "${CMAKE_CURRENT_BINARY_DIR}/homeview.json"
                       INSTALL_NAMESPACE "kmymoney")

#kcoreaddons_add_plugin sets LIBRARY_OUTPUT_DIRECTORY to ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${INSTALL_NAMESPACE}
set_target_properties(homeview
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_link_libraries(homeview
  PUBLIC
    kmm_plugin
    kmm_widgets
    kmm_menus
    kmymoney_common
  PRIVATE
    kmm_htmlrenderer
    Qt5::PrintSupport
    KF5::KHtml
)
