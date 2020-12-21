add_library(RenderSystem RenderSystem.cpp)

find_package(glfw3 REQUIRED)
target_include_directories(RenderSystem PUBLIC "/usr/local/include/horde3d/")
target_link_libraries(RenderSystem PUBLIC glfw)
