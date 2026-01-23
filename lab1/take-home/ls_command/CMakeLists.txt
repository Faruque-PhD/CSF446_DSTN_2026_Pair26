cmake_minimum_required(VERSION 3.22)
project(seekme_lab1_takehome_ls)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the ls source files
add_library(ls_lib
        ls.c
)

# Add test executable
add_executable(ls_test
        ls_test.c
)

# Link ls library to test
target_link_libraries(ls_test
        ls_lib
)

# Add a custom target to run tests
add_custom_target(run_tests
        COMMAND ls_test
        DEPENDS ls_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Define the root directory where the folder structure will be created
set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

# Custom command to create the folder structure with Indian language names
add_custom_target(create_test_data ALL
        # Create the directory structure with Indian language names
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_DATA_DIR}/hindi/tamil/telugu"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_DATA_DIR}/hindi/kannada"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_DATA_DIR}/marathi/assamese"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_DATA_DIR}/bengali/malayalam"

        # Create regular files with "hello" in different Indian languages
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/namaste.txt"
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/hindi/vanakkam.txt"
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/hindi/tamil/namaskaram.txt"
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/bengali/malayalam/swagatham.txt"

        # Create hidden files
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/hindi/tamil/.hidden_file1"
        COMMAND ${CMAKE_COMMAND} -E touch "${TEST_DATA_DIR}/hindi/tamil/.hidden_file2"

        # Create symbolic links
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${TEST_DATA_DIR}/hindi/tamil/namaskaram.txt" "${TEST_DATA_DIR}/hindi/tamil/telugu/link_to_namaskaram.txt"
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${TEST_DATA_DIR}/hindi" "${TEST_DATA_DIR}/hindi/tamil/link_to_hindi"

        COMMENT "Creating test data folder structure"
)

# Create hardlinks for h flag tests
add_custom_target(create_hardlinks
        DEPENDS create_test_data
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/namaste.txt" "${TEST_DATA_DIR}/marathi/assamese/namaste_hardlink.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/hindi/vanakkam.txt" "${TEST_DATA_DIR}/marathi/vanakkam_hardlink.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/hindi/tamil/namaskaram.txt" "${TEST_DATA_DIR}/hindi/kannada/namaskaram.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/hindi/tamil/namaskaram.txt" "${TEST_DATA_DIR}/hindi/kannada/.namaskaram_hardlink.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/hindi/tamil/namaskaram.txt" "${TEST_DATA_DIR}/bengali/namaskaram_hardlink.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/hindi/tamil/.hidden_file1" "${TEST_DATA_DIR}/hindi/tamil/telugu/kannada_swagatha_hardlink.txt"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${TEST_DATA_DIR}/bengali/malayalam/swagatham.txt" "${TEST_DATA_DIR}/hindi/tamil/telugu/.malayalam_swagatham_hardlink.txt"
        COMMENT "Creating hardlinks for test data"
)

# Define the root directory for hidden test data
set(HIDDEN_TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/hidden_data")

# Custom command to create the folder structure with Indian language names
add_custom_target(create_hidden_test_data ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HIDDEN_TEST_DATA_DIR}/home/abc/Downloads"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HIDDEN_TEST_DATA_DIR}/home/abc/.cache"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HIDDEN_TEST_DATA_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${HIDDEN_TEST_DATA_DIR}/etc/pacman"

        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/swapfile"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/bin/ls"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/bin/cat.exe"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/etc/pacman/pacman.conf"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Downloads/random.jpeg"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Downloads/assignment.pdf"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/RanDOM.c"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/hello.c"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp/hello.cpp"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp/hello.java"

        # Create hidden files
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/.cache/jdk"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/etc/.pacmanrc"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/.bash_history"
        COMMAND ${CMAKE_COMMAND} -E touch "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp/.gitignore"

        # Create symbolic links
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp/hello.cpp" "${HIDDEN_TEST_DATA_DIR}/home/abc/hello.cpp"
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${HIDDEN_TEST_DATA_DIR}/home/abc/.bash_history" "${HIDDEN_TEST_DATA_DIR}/etc/.bashrcetc_history"
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${HIDDEN_TEST_DATA_DIR}/bin/ls" "${HIDDEN_TEST_DATA_DIR}/home/abc/ls"

        COMMENT "Creating hidden test data folder structure"
)

# Create hardlinks for hidden test data
add_custom_target(create_hardlinks_hidden
        DEPENDS create_hidden_test_data
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/swapfile" "${HIDDEN_TEST_DATA_DIR}/home/abc/Documents/temp/.swapfile"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/swapfile" "${HIDDEN_TEST_DATA_DIR}/swapfile_pc"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/bin/ls" "${HIDDEN_TEST_DATA_DIR}/home/abc/Downloads/.ls"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/bin/ls" "${HIDDEN_TEST_DATA_DIR}/etc/ls_etc"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/bin/cat.exe" "${HIDDEN_TEST_DATA_DIR}/home/abc/cat"
        COMMAND ${CMAKE_COMMAND} -E create_hardlink "${HIDDEN_TEST_DATA_DIR}/home/abc/.cache/jdk" "${HIDDEN_TEST_DATA_DIR}/home/abc/.cache/jdk1"
        COMMENT "Creating hardlinks for hidden test data"
)

add_dependencies(run_tests create_hidden_test_data create_hardlinks create_hardlinks_hidden)

# Add a test target to be used with CTest
enable_testing()
add_test(NAME LsTests COMMAND ls_test)

# Ensure that `make` will build and run tests
add_dependencies(run_tests ls_test)