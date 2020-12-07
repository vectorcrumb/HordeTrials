cmake_minimum_required(VERSION 3.0.0)
project(HordeTutorial VERSION 0.1.0)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/include/**")
find_package(glfw3 REQUIRED)

add_executable(HordeTutorial tutorial.cpp)

target_include_directories(HordeTutorial PUBLIC "/usr/local/include/horde3d/")
target_include_directories(HordeTutorial PUBLIC "/usr/local/include/GLFW/")

target_link_libraries(HordeTutorial Horde3D)
target_link_libraries(HordeTutorial Horde3DUtils)
target_link_libraries(HordeTutorial glfw)
