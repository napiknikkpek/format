file(GLOB_RECURSE UNIT_TESTS "*.cpp")

foreach(file_ IN LISTS UNIT_TESTS)
  get_filename_component(target ${file_} NAME_WE)
  add_executable(${target} ${file_})
  add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
  target_link_libraries(${target} 
    PRIVATE Boost::boost format)
endforeach()
