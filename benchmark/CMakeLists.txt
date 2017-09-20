include(metabench)

add_compile_options(-fconstexpr-steps=999999999)

metabench_add_dataset(my_dataset
  "my.cpp.erb" "(1...10001).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_chart(format_chart DATASETS my_dataset)
