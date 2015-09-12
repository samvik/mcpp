include_directories(../)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_executable(mcpp-request-example request_example.cpp)
target_link_libraries(mcpp-request-example mcpp)

add_executable(mcpp-dispatcher-example dispatcher_example.cpp)
target_link_libraries(mcpp-dispatcher-example mcpp)

