#pragma once 

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <string.h>
#include <vector>

#include "window.hpp"

glm::mat4 model(1.0f);
glm::vec4 colour(1.0f);

GLuint VAO, VBO, EBO, shader, UniformModel, UniformColourIn, UniformTheTexture;
glm::vec2 renderPos = glm::vec2(0.0f);
float boxScale = 1.0f/6.0f;

const char* vshader = R"(
#version 330 core 

layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoord;

out vec2 texCoord;
uniform mat4 model;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    texCoord = aTexCoord;
})";

const char* fshader = R"(
#version 330 core

in vec2 texCoord;
out vec4 colour;

uniform sampler2D theTexture;
uniform vec4 colourIn;

void main() {

    if(colourIn.w == 0.0f) {
        colour = texture(theTexture, texCoord);
    } else {
        colour = colourIn;
    }
})";

void addShader(GLuint theProgram, const char* shadercode, GLenum shadertype) {
    GLuint theShader = glCreateShader(shadertype);

    const GLchar* thecode[1] = {0};
    thecode[0] = shadercode;
    int codelen[1] = {0};
    codelen[0] = strlen(shadercode);

    glShaderSource(theShader, 1, thecode, codelen);

    int result = 0;
    GLchar eLog[1024] = {0};

    glCompileShader(theShader);
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, 1024, nullptr, eLog);
        std::cout << eLog;
        return;
    }
    glAttachShader(theProgram, theShader);
    
}
void CompileShaderProgram() {
    shader = glCreateProgram();

    addShader(shader, vshader, GL_VERTEX_SHADER);
    addShader(shader, fshader, GL_FRAGMENT_SHADER);

    GLchar eLog[1024];
    int result = 0;

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, 1024, nullptr, eLog);
        std::cout << eLog;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, 1024, nullptr, eLog);
        std::cout << eLog;
    }

    
    UniformModel = glGetUniformLocation(shader, "model");
    UniformColourIn = glGetUniformLocation(shader, "colourIn");
    UniformTheTexture = glGetUniformLocation(shader, "theTexture");
}

void loadSquare() {
    GLfloat vertices[] = {
        -1.0, -1.0, 0.0,    0.0, 0.0,
        -1.0, 1.0, 0.0,     0.0, 1.0,
        1.0, -1.0, 0.0,     1.0, 0.0,
        1.0, 1.0, 0.0,       1.0, 1.0
    };
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, (void*)(3*sizeof(GLfloat)));
}

/*void addBox(glm::vec2 pos; GLfloat width=300; GLfloat height=300) {
    boxBuffer.emplace_back(pos, width, height);
}*/

struct textureStruct {
    int width, height, colourch;
    unsigned char* bytes;
    GLuint texID;

    textureStruct() {
        width = 100;
        height = 100;
    }
};

std::vector<textureStruct> textures;

void texturesetup() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    stbi_set_flip_vertically_on_load(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 0; i<26; i++) {
        std::string filename = "letters/"+std::to_string(i)+".png";
        const char* filename_c = filename.c_str();

        textures.push_back(textureStruct());

        glGenTextures(1, &textures.at(i).texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.at(i).texID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


        textures.at(i).bytes = stbi_load(filename_c, &textures.at(i).width, &textures.at(i).height, &textures.at(i).colourch, STBI_rgb_alpha);

        if(!textures.at(i).bytes)  {
            std::cout << "error loading texture (uses stbi_load)." << std::endl;
        }

        GLenum format = (textures.at(i).colourch == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures.at(i).width, textures.at(i).height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures.at(i).bytes);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(textures.at(i).bytes);
    }
}
void Clear() {
    glfwPollEvents();
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
} 

void Render() {
    glUseProgram(shader);
    glBindVertexArray(VAO);

    renderPos = glm::vec2(-1.0f, 1.25f);
    for(int j = 0; j<6; j++) {
        renderPos.y--;
        renderPos.x = -1.0f;

        for(int i = 0; i<5; i++) {

            renderPos.x+=2.0f;
            colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f+boxScale, 1.0f-boxScale, 0.0f));
            model = glm::scale(model, glm::vec3(boxScale, boxScale, 1.0f));
            model = glm::translate(model, glm::vec3(renderPos.x, renderPos.y, 0.0f));
            //model = glm::translate(model, glm::vec3(renderPos.x, renderPos.y, 0.0f));
            
            switch(letters[i][j]) {
                case grey:
                    colour = glm::vec4(0.15, 0.15, 0.15, 1.0);
                break;
                case yellow:
                    colour = glm::vec4(1.0, 0.6, 0.0, 1.0);
                break;
                case green:
                    colour = glm::vec4(0.0, 1.0, 0.3, 1.0);
                break;
            }
            //std::cout << "current letter colour is " << letters[i][j] << " at letter i#" << i << ", j#" << j << ".\n\n";

            glUniform4fv(UniformColourIn, 1, glm::value_ptr(colour));
            glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            //int lettertex = answers[j].at(i) - 65;
            int lettertex = 0;

            if(answers[j].length() == 5) {
                lettertex = answers[j].at(i) - 65;            
            }
                    
            model = glm::scale(model, glm::vec3(0.4f, 0.55f, 1.0f));
            glUniformMatrix4fv(UniformModel, 1, GL_FALSE, glm::value_ptr(model));//             model matrix

            colour = glm::vec4(0.0f);
            glUniform4fv(UniformColourIn, 1, glm::value_ptr(colour));//                             colouring

            glBindTexture(GL_TEXTURE_2D, textures.at(lettertex).texID);//                        texture bind
            glUniform1i(UniformTheTexture, 0);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        }
        renderPos.y--;
    }

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    glfwSwapBuffers(window);
}