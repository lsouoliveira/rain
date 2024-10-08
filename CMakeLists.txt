cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rain)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

include_directories(include)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/application.cpp
  src/particle_system.cpp
  src/pool.cpp
  src/interactive_pool.cpp
  src/rigidbody_2d.cpp
  src/duck.cpp

  include/entity.h
  include/utils.h
  include/particle.h
  include/renderer.h
  include/rigidbody_2d.h

  include/earcut.hpp
)

target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE include)
