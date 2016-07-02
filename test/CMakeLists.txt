include_directories(..)

set(MARKDOWN_TEST_FILES
    test_markdown.cpp
    ../error.hpp
    ../error.cpp
    ../parser.hpp
    ../parser.cpp)
add_executable(test_markdown ${MARKDOWN_TEST_FILES})
set_target_properties(test_markdown PROPERTIES
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test)
target_link_libraries(test_markdown ${Boost_LIBRARIES} hoedown)
add_test(NAME test_markdown
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/test
    COMMAND ${PROJECT_BINARY_DIR}/test/test_markdown)
add_custom_command(TARGET test_markdown PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/test/test_markdown.md
    ${PROJECT_BINARY_DIR}/test)

set(REGEX_TEST_FILES
    test_md_input.cpp
    ../error.hpp
    ../error.cpp
    ../parser.hpp
    ../parser.cpp
    ../file.hpp
    ../file.cpp
    ../biz.hpp
    ../biz.cpp)
add_executable(test_regex ${REGEX_TEST_FILES})
set_target_properties(test_regex PROPERTIES
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test)
target_link_libraries(test_regex ${Boost_LIBRARIES} hoedown yaml mstch)
add_test(NAME test_regex
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/test
    COMMAND ${PROJECT_BINARY_DIR}/test/test_regex)
