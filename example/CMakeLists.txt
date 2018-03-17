
add_executable(example
  main.cpp)

target_compile_options(example
  PRIVATE  
    -Wno-gnu-string-literal-operator-template
    -DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)

target_link_libraries(example
  PRIVATE format Boost::boost)
