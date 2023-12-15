cmake_minimum_required(VERSION 3.22)
project(recoilpp)
set(CMAKE_CXX_STANDARD 20)

set(SRCS
    pch.h
    recoilpp.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
add_executable(recoilpp ${SRCS})
target_precompile_headers(recoilpp PRIVATE pch.h)
