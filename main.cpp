#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string.h>
#include <ctime>
#include <cstdlib>

#include "window.hpp"
#include "graphics+shaders.hpp"
#include "gamemechanics.hpp"

int gameIteration = 0;

int main() {
    std::cout << "welcome to my wordle game.\n This was coded using c++ and raw opengl because i need that 5 merits (possibly more because of extra effort - btw you're cool mr mallinson). The source code is on Github in case you want to check it.\n";
    std::cout << "GAME RULES>\n\nit's the same as wordle, enter your guess into the terminal (only pops up when word is completed), the terminal shows what word it is in case you're struggling.\nA different word is randomised from the array of 20.\nPress ESC to exit the program\nThe window cannot be resized.\n\n";
    if (!setupWindow()) {
        return -1;
    }

    CompileShaderProgram();
    loadSquare();
    texturesetup();
    setArrays();

    word = randomNumber(1, 20);
    gameIteration = 0;
    std::cout << "word # is "<< word << ", word is " << words[word] << ".\n\n";

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {//app loop
        Clear();

        if(keys[GLFW_KEY_ESCAPE]) {
            std::cout << "ESC pressed\n";
            return 0;
        }

        //do the game loop thing

        if(gameIteration!=7) {
            std::cout << "enter your 5 letter guess.\n>";
            while(answers[gameIteration].length() != 5) {
                Clear();
                if(keys[GLFW_KEY_ESCAPE]) {
                    std::cout << "ESC pressed\n";
                    return 0;
                }

                Render();
                //std::cout << "answers of this iteration is equal to" << answers[gameIteration] << std::endl;
            }

            for(int i = 0; i<5; i++) {//do logic
                if(charSearch(answers[gameIteration].at(i), words[word])) {
                    letters[i][gameIteration] = yellow;
                }
                if(answers[gameIteration].at(i) == words[word].at(i)) {
                    letters[i][gameIteration] = green;
                }
            }
        }
        
        Clear();

        Render();

        //gameIteration++;
        //if(gameIteration == 6) gameIteration = 0;

        glfwSwapBuffers(window);

        if(gameIteration!=7)gameIteration++;
        //std::cout << "game iteration is " << gameIteration << ".\n\n";
        if(gameIteration==7) {
            setArrays();
            std::cout << "new game has started.\n";
            word = randomNumber(1, 20);
            gameIteration = 0;
            Clear();
        }
    }

    std::cout << "thanks for playing \n";
    return 0;
}