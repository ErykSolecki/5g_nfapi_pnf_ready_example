cmake_minimum_required(VERSION 3.5.0)
project(pnf_sender VERSION 0.1.0 LANGUAGES C CXX)

add_executable(pnf_sender main.cpp)

target_link_libraries(pnf_sender sctp)
