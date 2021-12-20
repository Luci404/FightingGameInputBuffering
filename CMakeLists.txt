cmake_minimum_required(VERSION 3.0.0)
project(FightingGameInputBufferingDemo)
add_executable(FightingGameInputBufferingDemo "${CMAKE_SOURCE_DIR}/src/main.cpp")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/GLFW")      
target_link_libraries(FightingGameInputBufferingDemo glfw)

# GLAD
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/glad")
target_link_libraries(FightingGameInputBufferingDemo glad)
target_link_libraries(FightingGameInputBufferingDemo "opengl32.lib")