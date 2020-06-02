cmake_minimum_required(VERSION 3.10)
project(cosimjni VERSION 0.1.0)

# ==============================================================================
# Build settings
# ==============================================================================

option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)

# ==============================================================================
# Global internal configuration
# ==============================================================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/wd4996")
    # C4251: 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    add_compile_options("/wd4251")
endif()

# Automatically export all symbols in Windows DLLs.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ==============================================================================
# Dependencies
# ==============================================================================

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(NO_OUTPUT_DIRS)
else()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
endif()


find_package(JNI REQUIRED)
find_package(libcosim REQUIRED)
add_subdirectory(src)
