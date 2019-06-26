
add_library(csecorejni cse.cpp)

target_link_libraries(csecorejni csecore)
target_include_directories(csecorejni PUBLIC ${JNI_INCLUDE_DIRS})