add_subdirectory(interface)
add_subdirectory(manager)

if(ENABLE_XMLSTORAGE)
  add_subdirectory(xml)
endif()

if(ENABLE_SQLSTORAGE)
  add_subdirectory(sql)
endif()
  