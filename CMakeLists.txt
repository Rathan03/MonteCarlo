cmake_minimum_required(VERSION 3.20)

project(MonteCarlo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MonteCarlo
    src/random_generator.cpp
)

target_include_directories(MonteCarlo PUBLIC include)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    MonteCarloTests
    tests/random_generator_test.cpp
)

target_link_libraries(
    MonteCarloTests
    PRIVATE
        MonteCarlo
        GTest::gtest_main
)

enable_testing()

add_test(MonteCarloTests MonteCarloTests)