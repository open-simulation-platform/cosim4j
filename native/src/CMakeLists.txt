
file(GLOB public_headers "${CMAKE_SOURCE_DIR}/include/cse/*.hpp")
file(GLOB private_headers "${CMAKE_CURRENT_SOURCE_DIR}/cse/*.hpp")
file(GLOB sources
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/jni/*.cpp"
        )

set(publicHeadersFull)
foreach(h IN LISTS publicHeaders)
  list(APPEND publicHeadersFull "${publicHeaderDir}/${h}")
endforeach()
set(privateHeadersFull)
foreach(h IN LISTS privateHeaders)
  list(APPEND privateHeadersFull "${privateHeaderDir}/${h}")
endforeach()
set(allSources ${publicHeadersFull} ${privateHeadersFull} ${sources})

add_library(csecorejni ${allSources})
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
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/../natives-win/src/main/resources/native/win"
          )
elseif (UNIX)
  set_target_properties(csecorejni
          PROPERTIES
          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../natives-linux/src/main/resources/native/linux"
          )
endif ()
