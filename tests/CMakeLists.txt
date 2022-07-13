function(arg_parse_test filename)
    add_executable(${filename} ${filename}.cpp)
    target_link_libraries(${filename} PRIVATE arg-parse gtest_main)
    gtest_discover_tests(${filename})
endfunction()

arg_parse_test(argparse)
arg_parse_test(parser)
