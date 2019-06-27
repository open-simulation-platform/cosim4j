
include_directories(../include)

add_library(csecorejni SHARED jniwrapper.cpp)
add_library(csecorejni::csecorejni ALIAS csecorejni)
target_compile_features(csecorejni PUBLIC "cxx_std_17")

target_include_directories(csecorejni
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        ${JNI_INCLUDE_DIRS}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(csecorejni
        PUBLIC
        ${JNI_LIBRARIES}
        csecore::csecorec)

set_target_properties(csecorejni
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
