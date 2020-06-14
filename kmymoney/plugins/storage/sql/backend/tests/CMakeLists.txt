include(ECMAddTests)

set(storagesqltesthelpers_SOURCES
  comparator.cpp
  mymoneytestutils.cpp
  ../../../xml/backend/tests/comparator.cpp
  )

add_library(storagesqltesthelpers STATIC ${storagesqltesthelpers_SOURCES})
target_link_libraries(storagesqltesthelpers
  PUBLIC
    Qt5::Sql
    KF5::I18n
    KF5::Archive
    kmm_mymoney
    kmm_istorage
    xmlstoragehelper
    onlinetask_interfaces
)

file(GLOB tests_sources "*-test.cpp")
# for now don't build database test as it will be replaced
message("mymoneydatabasemgr-test.cpp disabled in ${CMAKE_CURRENT_SOURCE_DIR}")
list(REMOVE_ITEM tests_sources ${CMAKE_CURRENT_SOURCE_DIR}/mymoneydatabasemgr-test.cpp)

ecm_add_tests(${tests_sources}
  LINK_LIBRARIES
    Qt5::Test
    "${PLUGIN_ID}_backend"
    storagesqltesthelpers
    onlinetask_interfaces
    kmm_testutils
    kmm_utils_platformtools
    kmm_mymoney
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sqlFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})