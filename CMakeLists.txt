cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

find_package(Boost 1.63 REQUIRED)

add_library(format INTERFACE)

target_include_directories(format
  INTERFACE include)

target_compile_features(format
  INTERFACE cxx_std_17)

add_subdirectory(example)
add_subdirectory(benchmark)
add_subdirectory(runtime)

enable_testing()
add_subdirectory(test)
