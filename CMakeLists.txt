cmake_minimum_required(VERSION 3.1.0)
project(HordeTutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/usr/local/include/)
find_package(glfw3 REQUIRED)

add_subdirectory(InputSystem)
add_subdirectory(RenderSystem)
list(APPEND EXTRA_LIBS InputSystem RenderSystem)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/InputSystem" "${PROJECT_SOURCE_DIR}/RenderSystem")

#add_executable(HordeTutorial main.cpp)
add_executable(HordeTutorial tutorial.cpp)

target_link_libraries(HordeTutorial PUBLIC Horde3D Horde3DUtils glfw ${EXTRA_LIBS})

#target_include_directories(HordeTutorial PUBLIC "/usr/local/include/horde3d/")
#target_include_directories(HordeTutorial PUBLIC "/usr/local/include/GLFW/")
target_include_directories(HordeTutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${EXTRA_INCLUDES}"
        )

#list(APPEND CMAKE_PREFIX_PATH "/usr/local/include/**")
#find_package(glfw3 REQUIRED)
#include_directories("./InputSystem/")
#include_directories("./RenderSystem")
#file(GLOB INCLUDE_LIST
#        InputSystem/*
#        RenderSystem/*
#    )
#
#
#
#
#add_executable(HordeTutorial main.cpp)
#
#target_include_directories(HordeTutorial PUBLIC "/usr/local/include/horde3d/")
#target_include_directories(HordeTutorial PUBLIC "/usr/local/include/GLFW/")
#
#target_link_libraries(HordeTutorial Horde3D)
#target_link_libraries(HordeTutorial Horde3DUtils)
#target_link_libraries(HordeTutorial glfw)
