
cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set the C standard as needed
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add the header directory so your compiler finds your headers
include_directories(${PROJECT_SOURCE_DIR}/header)

# Gather all C source files in the src directory

# This file fucntion matches pattern for everyfile and save it to SOURCES
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

# Create an executable from the source files
add_library(appLib STATIC ${SOURCES})
link_libraries(appLib)

add_executable(my_app main.c)

# Enable runtime sanitizers for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(my_app PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
        target_link_options(my_app PRIVATE -fsanitize=undefined)
	endif()
endif()

enable_testing()
add_executable(myTest test/tests.c)
add_test(NAME myTest COMMAND myTest)
