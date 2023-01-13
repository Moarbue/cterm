#include <stdio.h>
#include <stdlib.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "util.h"

extern int ww, wh;

void glfw_error_callback(int error, char *description)
{
    (void)error;
    fprintf(stderr, "ERROR: %s\n", description);
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    ww = width;
    wh = height;
    glViewport(0, 0, ww, wh);
}


void set_up_glfw(void)
{
    if (glfwInit() == GLFW_FALSE)
        PANIC("ERROR: Failed to initialize GLFW!\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwSetErrorCallback((GLFWerrorfun)glfw_error_callback);
}

GLFWwindow * create_winow()
{
    GLFWwindow *window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "cterm", NULL, NULL);
    if (window == NULL)
        PANIC("ERROR: Failed to create window!\n");

    glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)glfw_framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    return window;
}

void load_opengl_functions(void)
{
    int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (version == GL_FALSE)
        PANIC("ERROR: Failed to load OpenGL functions!\n");

    LOG_MESSAGE("INFO: Using OpenGL Version %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}