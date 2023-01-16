#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "util.h"
#include "lineal.h"
#include "renderer.h"

GLFWwindow *window;
int ww, wh;

Renderer r = {0};

void cleanup(void);
void glfw_process_keyboard_input(GLFWwindow *window);

int main()
{
    atexit(cleanup);
    // Initialize and configure GLFW
    set_up_glfw();
    // Create and configure window
    window = create_window();
    // Load OpenGL functions
    load_opengl_functions();

    glClearColor(0.7f, 0.9f, 0.1f, 1.0f);

    renderer_init(&r, "shaders/cterm.vert", "shaders/color.frag", "shaders/texture.frag");
    texture_new("resources/prister.jpg");


    // main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glfwPollEvents();
        glfw_process_keyboard_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderer_set_shader(&r, SHADER_TEXTURE);
        renderer_triangle(&r, vec2f(-0.5f, -0.5f), vec2f(0.5f, -0.5f), vec2f(0.f, 0.5f),
                              vec4f(1.f, 1.f, 1.f, 1.f), vec4f(1.f, 1.f, 1.f, 1.f), vec4f(1.f, 1.f, 1.f, 1.f),
                              vec2f(0.f, 0.f), vec2f(1.f, 0.f), vec2f(0.6f, 1.f));

        renderer_flush(&r);

        r.time = glfwGetTime();
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
