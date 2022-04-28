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

include(cmake/conan.cmake)
conan_cmake_autodetect(settings)

# Force add osp remote
conan_add_remote(NAME osp URL https://osp.jfrog.io/artifactory/api/conan/conan-local)

# Run conan install with auto-detected settings and -b missing flag
conan_cmake_run(
        CONANFILE conanfile.txt
        BUILD missing
        SETTINGS ${settings}
        BASIC_SETUP NO_OUTPUT_DIRS)


find_package(JNI REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(libcosim REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)

add_subdirectory(src)
