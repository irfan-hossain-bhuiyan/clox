
cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set the C standard as needed
set(CMAKE_C_STANDARD 99)

# Add the header directory so your compiler finds your headers
include_directories(${PROJECT_SOURCE_DIR}/header)

# Gather all C source files in the src directory
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

# Create an executable from the source files
add_executable(my_app ${SOURCES})

# Enable runtime sanitizers for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(my_app PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
        target_link_options(my_app PRIVATE -fsanitize=undefined)
    elseif(MSVC)
        # For MSVC, enable runtime checks
        set_property(TARGET my_app PROPERTY MSVC_RUNTIME_CHECKS "StackFrameErrorCheck;UninitializedVariable")
    endif()
endif()
