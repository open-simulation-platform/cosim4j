cmake_minimum_required(VERSION 3.10)
project(csecorejni VERSION 0.0.1)

# ==============================================================================
# Build settings
# ==============================================================================

option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)
option(CSECOREJNI_USING_CONAN "Whether Conan is used for package management" ON)

# ==============================================================================
# Global internal configuration
# ==============================================================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/wd4996")
    if (CSECOREJNI_USING_CONAN)
        # C4251: 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
        add_compile_options("/wd4251")
    endif()
endif()

# Automatically export all symbols in Windows DLLs.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ==============================================================================
# Dependencies
# ==============================================================================

if(CSECOREJNI_USING_CONAN)
    if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
        conan_basic_setup(NO_OUTPUT_DIRS)
    else()
        message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
    endif()
endif()

find_package(JNI REQUIRED)
find_package(csecore REQUIRED)
add_subdirectory(src)
