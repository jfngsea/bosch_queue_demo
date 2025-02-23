cmake_minimum_required(VERSION 3.30)
project(bosch_demo)

set(CMAKE_CXX_STANDARD 20)



set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)


add_executable(bosch_demo
        src/main.cpp
)

target_include_directories(bosch_demo PRIVATE include)

# TESTS

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        queue_test
        test/QueueTest.cpp
)
target_include_directories(
        queue_test
        PRIVATE
        include
)
target_link_libraries(
        queue_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(queue_test)
