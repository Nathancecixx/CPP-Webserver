cmake_minimum_required(VERSION 3.7)

project(WebServer)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(WebServer main.cpp CartManager.cpp InventoryManager.cpp)
target_link_libraries(WebServer ${Boost_LIBRARIES} Threads::Threads)
