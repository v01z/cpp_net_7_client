cmake_minimum_required(VERSION 3.22)
project(client_homework_7)

set(CMAKE_CXX_STANDARD 17)

find_package ( cpr REQUIRED )

add_executable(client main.cpp FileDownloader.cpp)

target_link_libraries(client PRIVATE cpr::cpr)
