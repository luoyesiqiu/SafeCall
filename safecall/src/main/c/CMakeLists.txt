cmake_minimum_required(VERSION 3.22.1)

project("safecall")

include_directories(include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
add_library(safecall SHARED safe_call.c)

target_link_libraries(safecall)
