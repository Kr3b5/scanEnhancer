cmake_minimum_required(VERSION 3.24)
project(scanEnhancer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenMP
find_package(OpenMP REQUIRED)

add_executable(scanEnhancer main.cpp fileHelper.cpp fileHelper.h filter.cpp filter.h terminal_helper.cpp terminal_helper.h)
target_link_libraries(scanEnhancer PUBLIC OpenMP::OpenMP_CXX)

# Catch
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(scanEnhancer_tests catch_test.cpp fileHelper.cpp filter.cpp)
target_link_libraries(scanEnhancer_tests PRIVATE Catch2::Catch2WithMain PUBLIC OpenMP::OpenMP_CXX)

add_test(NAME SE_TEST COMMAND scanEnhancer_tests)