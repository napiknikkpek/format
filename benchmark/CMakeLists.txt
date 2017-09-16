include(metabench)

include(benchmark/dependencies/pprintf)

add_compile_options(-fconstexpr-steps=999999999)
add_compile_options(-ftemplate-depth=999999999)

metabench_add_dataset(my_dataset
  "my.cpp.erb" "(10...200).step(20)"
  ENV "{templ: '{}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
  MEDIAN_OF 2)

metabench_add_dataset(pprintf_dataset
  "pprintf.cpp.erb" "(10...40).step(5)"
  ENV "{templ: '{}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
  MEDIAN_OF 2)

target_include_directories(pprintf_dataset PUBLIC ${Pprintf_INCLUDE_DIR})
add_dependencies(pprintf_dataset Pprintf)

metabench_add_chart(format_chart DATASETS my_dataset pprintf_dataset)
