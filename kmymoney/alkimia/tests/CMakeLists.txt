include(ECMAddTests)

file(GLOB tests_sources "*-test.cpp")

ecm_add_tests(${tests_sources}
  LINK_LIBRARIES
    alkimia
    Qt5::Test
)
