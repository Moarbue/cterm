#include <stdio.h>
#include <stdlib.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "util.h"

GLFWwindow *window;

int ww, wh;

void cleanup(void);
void glfw_process_keyboard_input(GLFWwindow *window);

int main()
{
    atexit(cleanup);
    // Initialize and configure GLFW
    set_up_glfw();
    // Create and configure window
    window = create_winow();
    // Load OpenGL functions
    load_opengl_functions();

    glClearColor(0.7f, 0.9f, 0.1f, 1.0f);

    // main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glfwPollEvents();
        glfw_process_keyboard_input(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    return 0;
}

void cleanup(void)
{
    LOG_MESSAGE("Exiting...\n");
    fflush(stdin);
    glfwTerminate();
}

void glfw_process_keyboard_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
