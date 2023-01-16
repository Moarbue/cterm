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
        renderer_texture_rect(&r, -.5f, -.5f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f);

        renderer_set_shader(&r, SHADER_COLOR);
        renderer_solid_rect(&r, 0.3f, 0.35f, 0.21f, 0.3f, vec4fs(1.f));
        Vec4f red   = vec4f(1.f, 0.f, 0.f, 1.f);
        Vec4f black = vec4f(0.f, 0.f, 0.f, 1.f);
        renderer_set_shader(&r, SHADER_COLOR);
        renderer_line(&r, vec2f(-1.f, 1.f),  vec2f(1.f, -1.f), black, 0.05);
        renderer_line(&r, vec2f(-1.f, -1.f), vec2f(1.f, 1.f),  black, 0.05);
        renderer_line(&r, vec2f(0.f, .9f),   vec2f(0.f, -.9f), black, 0.05);
        renderer_line(&r, vec2f(-1.f, 0.f),  vec2f(1.f, 0.f),  black, 0.05);
        renderer_line(&r, vec2f(0.f, .9f),   vec2f(0.f, -.9f), red,   0.006);
        renderer_line(&r, vec2f(-1.f, 0.f),  vec2f(1.f, 0.f),  red,   0.006);

        renderer_set_shader(&r, SHADER_COLOR);
        renderer_cubic_bezier(&r, vec2f(-.5f, -.5f), vec2f(-.45, .5f), vec2f(.45f, .5f), vec2f(.5f, -.5f), black, 0.01);
        renderer_cubic_bezier(&r, vec2f(-.6f, -.14f), vec2f(-.45, .5f), vec2f(.45f, .5f), vec2f(.9f, -.46f), black, 0.01);
        renderer_cubic_bezier(&r, vec2f(-.6f, .14f), vec2f(-.45, -.5f), vec2f(.45f, -.5f), vec2f(.9f, .46f), black, 0.01);

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
