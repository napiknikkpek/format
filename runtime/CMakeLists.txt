find_package(benchmark)

if(NOT benchmark_FOUND)
  return()
endif()

add_executable(runtime runtime.cpp)

target_link_libraries(runtime 
  PRIVATE format Boost::boost benchmark::benchmark)
