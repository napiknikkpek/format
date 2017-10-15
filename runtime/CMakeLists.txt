include(benchmarkConfig)

add_executable(runtime runtime.cpp)

target_link_libraries(runtime 
  PRIVATE format benchmark::benchmark)
