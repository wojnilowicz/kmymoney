include(ECMAddTests)

set(mymoneystoragexml_SOURCES
  ../mymoneystoragexml.cpp
  ../mymoneystoragenames.cpp
  ../xmlstorageupdater.cpp
  xmlfiletesthelper.cpp
  )

add_library(mymoneystoragexml STATIC ${mymoneystoragexml_SOURCES})
target_link_libraries(mymoneystoragexml
  PUBLIC
    Qt5::Xml
    KF5::I18n
    KF5::Archive
    kmm_mymoney
  PRIVATE
    xmlstoragehelper
)

file(GLOB tests_sources "*-test.cpp")
ecm_add_tests(${tests_sources}
  LINK_LIBRARIES
    Qt5::Test
    mymoneystoragexml
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/xmlFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# add_custom_command(TARGET mymoneyxmlfiles-test PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy_directory
#                        ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})