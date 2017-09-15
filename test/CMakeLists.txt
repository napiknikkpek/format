file(GLOB_RECURSE UNIT_TESTS "*.cpp")

foreach(file_ IN LISTS UNIT_TESTS)
  get_filename_component(target ${file_} NAME_WE)
  add_executable(${target} EXCLUDE_FROM_ALL ${file_})
  add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
  target_link_libraries(${target} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endforeach()
