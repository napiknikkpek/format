cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost 1.63 REQUIRED)

add_library(format INTERFACE)

target_include_directories(format
  INTERFACE include)

target_link_libraries(format
  INTERFACE Boost::boost)

target_compile_features(format
  INTERFACE cxx_std_17)

add_subdirectory(example)
add_subdirectory(benchmark)
add_subdirectory(runtime)

enable_testing()
add_subdirectory(test)
