cmake_minimum_required(VERSION 3.22)
project(seekme_lab1_takehome_grep)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the grep source files 
add_library(grep_lib
        grep.c
)

# Add grep test executable 
add_executable(grep_test
        grep_test.c
)

target_link_libraries(grep_test
        grep_lib
)

# Add a custom target to run grep tests
add_custom_target(run_grep_test
        COMMAND grep_test
        DEPENDS grep_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add a test target to be used with CTest
enable_testing()
add_test(NAME GrepTests COMMAND grep_test)

# Ensure that `make` will build and run tests
add_dependencies(run_grep_test grep_test)