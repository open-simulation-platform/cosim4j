
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

add_library(csecorejni ${sources})
target_include_directories(csecorejni
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

if (Python3_FOUND)
  target_link_libraries(csecorejni
          PRIVATE
          Python3:python
          )
endif ()

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
