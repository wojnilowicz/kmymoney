include(ECMAddTests)

set(storagexmltesthelpers_SOURCES
  readerwriterhelper.cpp
  comparator.cpp
  )

add_library(storagexmltesthelpers STATIC ${storagexmltesthelpers_SOURCES})
target_link_libraries(storagexmltesthelpers
  PUBLIC
  "${PLUGIN_ID}_backend"
)

file(GLOB tests_sources "*-test.cpp")
ecm_add_tests(${tests_sources}
  LINK_LIBRARIES
    Qt5::Test
    storagexmltesthelpers
    kmm_testutilities
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/xmlFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})