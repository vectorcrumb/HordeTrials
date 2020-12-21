add_library(InputSystem InputSystem.cpp)

find_package(glfw3 REQUIRED)
target_include_directories(InputSystem PUBLIC "/usr/local/include/horde3d/")
target_link_libraries(InputSystem PUBLIC glfw)
