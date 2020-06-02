
set(sources
        cosim.cpp
        jvm_slave.cpp
        model_description_parser.cpp
        step_event_listener.cpp
        thread_worker.cpp

        jni/connection.cpp
        jni/error.cpp
        jni/execution.cpp
        jni/initial.cpp
        jni/logging.cpp
        jni/manipulator.cpp
        jni/observer.cpp
        jni/samples.cpp
        jni/slave.cpp
        )

add_library(cosimjni ${sources})
target_compile_definitions(cosimjni PUBLIC "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
target_include_directories(cosimjni
        PUBLIC
        ../include
        PRIVATE
        "${JNI_INCLUDE_DIRS}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_link_libraries(cosimjni
        PRIVATE
        "${JNI_LIBRARIES}"
        libcosim::cosim
        )

if (WIN32)
    set_target_properties(cosimjni
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../natives-win/src/main/resources/native/win"
            )
elseif (UNIX)
    set_target_properties(cosimjni
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../natives-linux/src/main/resources/native/linux"
            )
endif ()
