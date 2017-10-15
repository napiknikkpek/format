include(metabench)

metabench_add_dataset(my_dataset
  "my.cpp.erb" "(1...10001).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_chart(format_chart DATASETS my_dataset)

target_compile_options(my_dataset
  PRIVATE -fconstexpr-steps=999999999)

target_link_libraries(my_dataset
  PRIVATE format Boost::boost)
