
set(sources
        jvm_slave.cpp
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

if (Python3_FOUND)
  add_executable(pytest python_test.cpp)
  target_link_libraries(pytest
          PUBLIC
          Python3::Python
          )
  target_link_libraries(pytest PRIVATE ${Python3_LIBRARIES})
endif ()

add_library(csecorejni ${sources})
target_compile_definitions(csecorejni PUBLIC "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
target_compile_features(csecorejni PUBLIC "cxx_std_17")
target_include_directories(csecorejni
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        PRIVATE
        "${JNI_INCLUDE_DIRS}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_link_libraries(csecorejni
        PRIVATE
        "${JNI_LIBRARIES}"
        csecore::csecorec
        csecore::csecorecpp
        )

if (WIN32)
  set_target_properties(csecorejni
          PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../../java/cse-core4j/natives-win/src/main/resources/native/win"
          )
elseif (UNIX)
  set_target_properties(csecorejni
          PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../java/cse-core4j/natives-linux/src/main/resources/native/linux"
          )
endif ()
