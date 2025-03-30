cmake_minimum_required(VERSION 3.7)

project(WebServer)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(WebServer
    src/main.cpp
    src/CartManager.cpp
    src/InventoryManager.cpp
    src/AuthManager.cpp
)

target_link_libraries(WebServer ${Boost_LIBRARIES} Threads::Threads)
