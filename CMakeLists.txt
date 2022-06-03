cmake_minimum_required(VERSION 3.22)
project(Ex6)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(Ex6 poll_server.cpp reactor.hpp queue.hpp active_object.hpp guard.hpp singleton.hpp)
target_link_libraries(Ex6 PRIVATE Threads::Threads)

