//
// Created by lucas on 12/20/20.
//

#ifndef HORDETUTORIAL_INPUTSYSTEM_H
#define HORDETUTORIAL_INPUTSYSTEM_H

#include <GLFW/glfw3.h>

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    void registerWindowCloseCallback(GLFWwindow* win, GLFWwindowclosefun cb);
    void registerWindowSizeCallback(GLFWwindow* win, GLFWwindowsizefun cb);
    void registerKeyCallback(GLFWwindow* win, GLFWkeyfun cb);

};


#endif //HORDETUTORIAL_INPUTSYSTEM_H
