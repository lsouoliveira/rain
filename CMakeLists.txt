cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rain)

include_directories(include)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/application.cpp
  src/particle_system.cpp
  src/pool.cpp
)

target_link_libraries(${PROJECT_NAME} libraylib.a)
target_include_directories(${PROJECT_NAME} PRIVATE include)
