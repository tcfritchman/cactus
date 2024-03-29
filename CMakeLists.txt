set(PROJECT_NAME nes_emulator)
set(TEST_PROJECT_NAME ${PROJECT_NAME}_tests)

# https://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/test_main.cpp" )

file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX "src/main.cpp" )

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})
target_compile_options(${TEST_PROJECT_NAME} PRIVATE -Wall -Wextra)
target_link_libraries(${TEST_PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

enable_testing()
add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME} ../res/nestest/)