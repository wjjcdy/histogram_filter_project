cmake_minimum_required(VERSION 3.9)
project(histogram)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        include

)

add_executable(histogram
        tests.cpp
)

target_link_libraries(histogram)




