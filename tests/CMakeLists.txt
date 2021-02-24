list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(CMakeDependentOption)
find_package(Libcheck REQUIRED)

add_executable(macro_test api_test.c)
target_link_libraries(macro_test PUBLIC libcheck::check macro)
