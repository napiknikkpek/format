include(metabench)

metabench_add_dataset(fast
  "fast.cpp.erb" "(0...10001).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_dataset(handy
  "handy.cpp.erb" "(0...10001).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_chart(format_chart DATASETS fast handy)

metabench_add_dataset(args
  "args.cpp.erb" "(0...10001).step(1000)"
  ENV "{templ: '{}asdf'}"
)

metabench_add_chart(args_chart DATASETS args)

target_compile_options(fast
  PRIVATE -fconstexpr-steps=999999999)
target_link_libraries(fast
  PRIVATE format)

target_compile_options(handy PRIVATE 
  -fconstexpr-steps=999999999
  -Wno-gnu-string-literal-operator-template
  -DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)
target_link_libraries(handy
  PRIVATE format Boost::boost)

target_compile_options(args
  PRIVATE -fconstexpr-steps=999999999)
target_link_libraries(args
  PRIVATE format)
