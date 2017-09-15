include(metabench)

add_compile_options(-fconstexpr-steps=999999999)
add_compile_options(-ftemplate-depth=999999999)

metabench_add_dataset(my_dataset
  "my.cpp.erb" "(10...300).step(50)")

metabench_add_dataset(pprintf_dataset
  "pprintf.cpp.erb" "(10...40).step(5)")

metabench_add_chart(format_chart DATASETS my_dataset pprintf_dataset)
