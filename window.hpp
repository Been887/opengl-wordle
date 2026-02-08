#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "gamemechanics.hpp"

GLFWwindow* window;

const int width = 1000, height = 1000;
int bufferwidth, bufferheight;

extern int gameIteration;

bool setupWindow() {
    if(!glfwInit()) {
        std::cout << "failed to initialise GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Ben's Simple Wordle", nullptr, nullptr);
    if(!window) {
        std::cout << "failed to create window\n";
        return false;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &bufferwidth, &bufferheight);

    if(glewInit() != GLEW_OK) {
        std::cout << "GLEW failed to initialise!\n";
        return false;
    }

    glViewport(0, 0, bufferwidth, bufferheight);

    return true;
}

bool keys[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            if(!keys[key]) {
                if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                    char c = 'A' + (key - GLFW_KEY_A); // gives 'A'..'Z'
                    answers[gameIteration]+=c;
                }
                if (key == GLFW_KEY_SPACE) {
                    char c = ' ';
                    answers[gameIteration]+=c;
                }
                if (key == GLFW_KEY_BACKSPACE && !keys[key]) {
                    std::cout << "\n\nlength of string at time of backspace > " << answers[gameIteration].length() << std::endl;
                    if(answers[gameIteration].length() == 0) {
                        //std::cout << "game iteration at time of backspace > " << gameIteration << ", and after backspace > ";
                        gameIteration--;
                        //std::cout << gameIteration << std::endl;
                    } else {
                        //std::cout << "\n\nanswers[gameIteration] before popback > " << answers[gameIteration] << ",\nand after pop back ";
                        answers[gameIteration].pop_back();
                        //std::cout << answers[gameIteration] << std::endl;
                    }
                }
            }
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}