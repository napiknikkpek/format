find_package(benchmark)

if(NOT benchmark-FOUND)
  return()
endif()

add_executable(runtime runtime.cpp)

target_link_libraries(runtime 
  PRIVATE format benchmark::benchmark)
