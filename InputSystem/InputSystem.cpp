//
// Created by lucas on 12/20/20.
//

#include "InputSystem.h"

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::registerWindowCloseCallback(GLFWwindow *win, GLFWwindowclosefun cb) {
    glfwSetWindowCloseCallback(win, cb);
}

void InputSystem::registerWindowSizeCallback(GLFWwindow *win, GLFWwindowsizefun cb) {
    glfwSetWindowSizeCallback(win, cb);
}

void InputSystem::registerKeyCallback(GLFWwindow *win, GLFWkeyfun cb) {
    glfwSetKeyCallback(win, cb);
}