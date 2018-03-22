find_package(fmt)

if (NOT fmt_FOUND)
  return()
endif()

include(metabench)

metabench_add_dataset(fast
  "fast.cpp.erb" "(0..20000).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_dataset(handy
  "handy.cpp.erb" "(0..10000).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_dataset(fmt_1
  "fmt_1.cpp.erb" "(0..20000).step(1000)"
  ENV "{templ: '{0}asdfsdfsadfasdfsadfasdfasdfasdfsadfsa'}"
)

metabench_add_chart(format_chart DATASETS fast fmt_1)

metabench_add_dataset(args
  "args.cpp.erb" "(0..1000).step(100)"
  ENV "{templ: '{}asdf'}"
)

metabench_add_dataset(fmt_args
  "fmt_args.cpp.erb" "(0..500).step(100)"
  ENV "{templ: '{}asdf'}"
)

metabench_add_chart(args_chart DATASETS args fmt_args)

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

target_compile_options(fmt_1
  PRIVATE -fconstexpr-steps=999999999)
target_link_libraries(fmt_1
  PRIVATE fmt)

target_compile_options(args
  PRIVATE -fconstexpr-steps=999999999)
target_link_libraries(args
  PRIVATE format)

target_compile_options(fmt_args
  PRIVATE -fconstexpr-steps=999999999)
target_link_libraries(fmt_args
  PRIVATE fmt)
