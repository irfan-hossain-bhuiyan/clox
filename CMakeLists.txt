
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


add_executable(myTest test/tests.c)
# Enable runtime sanitizers for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		foreach(target  my_app myTest)
			target_compile_options(${target} PRIVATE -Wall -Wextra -Wuninitialized 
				-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)
			target_link_options(${target} PRIVATE -fsanitize=undefined
				-fsanitize=address)
		endforeach()
	endif()
endif()

enable_testing()
add_test(NAME tokenTest COMMAND myTest token)
add_test(NAME compilerTest COMMAND myTest compiler)
add_test(NAME vmTest COMMAND myTest vm)

