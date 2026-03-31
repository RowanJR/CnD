#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <freetype2/freetype/freetype.h>
//#include FT_FREETYPE_H

#include "map2d.h"
#include "entities.h"
#include "abilities.h"
#include "ability_system.h"
#include "dice.h"
#include "tiles.h"
#include "turns.h"

int heldW, heldA, heldS, heldD = 0;

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }

    //single press detection
    if((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && !heldW)
    {
        printf("hi\n");
        heldW = 1;
    }
    else if((glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) && heldW)
    {
        heldW = 0;
    }

    //single press detection
    if((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !heldS)
    {
        heldS = 1;
    }
    else if((glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) && heldS)
    {
        heldS = 0;
    }
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW init failed\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(600, 600, "CND", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Window creation failed\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, 600, 600);

    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.1f, 0.6f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //maintainence
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}