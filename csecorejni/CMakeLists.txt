
cmake_minimum_required(VERSION 3.12)
project(csecorejni VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(NO_OUTPUT_DIRS)
else()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time fiber filesystem log)
find_package(FMILibrary REQUIRED)
find_package(LIBZIP REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(csecore REQUIRED)
find_package(JNI REQUIRED)

add_subdirectory(src)
