cmake_minimum_required(VERSION 3.16)
project(grcp)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.0
)

FetchContent_MakeAvailable(spdlog)

add_subdirectory(./common/broadcastUtil)
add_subdirectory(./common/api)

add_subdirectory(./client)
add_subdirectory(./server)