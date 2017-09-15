cmake_minimum_required(VERSION 3.7)

project(format LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

find_package(Boost 1.63 REQUIRED)

add_compile_options(-std=c++1z)
add_compile_options(-stdlib=libc++)
add_compile_options(-Wno-gnu-string-literal-operator-template)
set(CMAKE_EXE_LINKER_FLAGS -stdlib=libc++)

add_definitions(-DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)
# boost use removed features
add_definitions(-D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES)

include_directories(${Boost_INCLUDE_DIRS})

add_library(format INTERFACE)
target_include_directories(format INTERFACE include)

include_directories(${format_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(example)
add_subdirectory(benchmark)

enable_testing()
add_subdirectory(test)
