set(alkimia_SOURCES
  alkvalue.cpp
  )

set(alkimia_HEADERS
  alkvalue.h
  ${CMAKE_CURRENT_BINARY_DIR}/alk_export.h
  )

add_library(alkimia SHARED ${alkimia_SOURCES})

generate_export_header(alkimia BASE_NAME alk)

target_link_libraries(alkimia
  PUBLIC
    Qt5::Core
    Qt5::DBus
    ${MP_LIBRARIES}
)

set_target_properties(alkimia PROPERTIES
  VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS alkimia ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES ${alkimia_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/kmymoney
        COMPONENT Devel)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
