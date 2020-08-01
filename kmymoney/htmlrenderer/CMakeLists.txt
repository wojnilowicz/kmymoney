configure_file(config-htmlrenderer.h.in config-htmlrenderer.h @ONLY)

add_library(kmm_htmlrenderer SHARED
  kmymoneyhtmlrenderer.cpp
  $<$<BOOL:${ENABLE_QTWEBENGINE}>:kmymoneyqtwebengine.cpp>
  $<$<BOOL:${ENABLE_KDEWEBKIT}>:kmymoneykdewebkit.cpp>
  $<$<BOOL:${ENABLE_KHTML}>:kmymoneykhtml.cpp>
)

generate_export_header(kmm_htmlrenderer)
set_target_properties(kmm_htmlrenderer
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

target_link_libraries(kmm_htmlrenderer
  PRIVATE
    Qt5::Core
    Qt5::PrintSupport
    $<$<BOOL:${ENABLE_QTWEBENGINE}>:Qt5::WebEngineWidgets>
    $<$<BOOL:${ENABLE_KDEWEBKIT}>:KF5::WebKit>
    $<$<BOOL:${ENABLE_KHTML}>:KF5::KHtml>
)

set_target_properties(kmm_htmlrenderer PROPERTIES
  VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS kmm_htmlrenderer
  ${INSTALL_TARGETS_DEFAULT_ARGS} )

