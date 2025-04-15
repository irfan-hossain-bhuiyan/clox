
cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set the C standard as needed
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add the header directory so your compiler finds your headers
add_compile_options(-Wno-c11-extensions -Wall -Wextra -Wuninitialized
                    -fno-omit-frame-pointer)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		#	foreach(target  my_app myTest)
		#	foreach	target_compile_options(${target} PRIVATE
		#-Wall -Wextra -Wuninitialized
		#	foreach		-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -g)
		#	foreach	target_link_options(${target} PRIVATE -fsanitize=undefined
		#	foreach		-fsanitize=address)
		#	foreachendforeach()
		add_compile_options( -fsanitize=address -fsanitize=undefined -g)
		add_link_options(-fsanitize=undefined -fsanitize=address)
	endif()
endif()

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

enable_testing()
add_test(NAME tokenTest COMMAND myTest token)
add_test(NAME compilerTest COMMAND myTest compiler)
add_test(NAME vmTest COMMAND myTest vm)
add_test(NAME strHashTest COMMAND myTest strHash)
add_test(NAME tableTest COMMAND myTest table)
add_test(NAME strInternTest COMMAND myTest strIntern)
add_test(NAME vmStatementTest COMMAND myTest vmVarDecl)
add_test(NAME vmPrintTest COMMAND myTest vmPrint)
