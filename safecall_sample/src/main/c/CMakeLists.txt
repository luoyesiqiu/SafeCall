
cmake_minimum_required(VERSION 3.22.1)

project("safecaller")

find_package(safecall REQUIRED CONFIG)

add_library(safecaller
        SHARED
        native-lib.c)

find_library(log-lib
        log)

target_link_libraries(safecaller
        safecall::safecall
        ${log-lib})