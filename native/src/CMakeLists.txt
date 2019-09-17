
set(sources
        "connection.cpp"
        "error.cpp"
        "execution.cpp"
        "initial.cpp"
        "logging.cpp"
        "manipulator.cpp"
        "observer.cpp"
        "samples.cpp"
        "slave.cpp"
        "step_event_listener.cpp"
        )

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

if(WIN32)
  set_target_properties(csecorejni
          PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../natives-win/src/main/resources/native/win"
          )
elseif(UNIX)
  set_target_properties(csecorejni
          PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../natives-linux/src/main/resources/native/linux"
          )
endif()
