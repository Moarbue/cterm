#include <stdio.h>
#include <stdlib.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#define PANIC(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
#define LOG_MESSAGE(...) ( printf(__VA_ARGS__) )

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

GLFWwindow *window = NULL;

int ww, wh;

void cleanup(void);
void glfw_error_callback(int error, char *description);
void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);
void glfw_process_keyboard_input(GLFWwindow *window);

int main()
{
    atexit(cleanup);
    // Initialize and configure GLFW
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
    // Create and configure window
    {
        window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "cterm", NULL, NULL);
        if (window == NULL)
            PANIC("ERROR: Failed to create window!\n");

        glfwSetFramebufferSizeCallback(window, (GLFWframebuffersizefun)glfw_framebuffer_size_callback);
        glfwMakeContextCurrent(window);
    }
    // Load OpenGL functions
    {
        int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
        if (version == GL_FALSE)
            PANIC("ERROR: Failed to load OpenGL functions!\n");

        LOG_MESSAGE("INFO: Using OpenGL Version %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    }

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

void glfw_process_keyboard_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
