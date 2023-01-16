#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"

#include "util.h"

extern int ww, wh;

void glfw_error_callback(int error, char *description)
{
    (void)error;
    fprintf(stderr, "ERROR: %s\n", description);
}

void gl_message_callback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void set_up_glfw(void)
{
    if (glfwInit() == GLFW_FALSE)
        PANIC("ERROR: Failed to initialize GLFW!\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwSetErrorCallback((GLFWerrorfun)glfw_error_callback);
}

GLFWwindow * create_window(GLFWframebuffersizefun framebuffer_size_callback, GLFWmousebuttonfun mouse_button_callback)
{
    GLFWwindow *window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "cterm", NULL, NULL);
    if (window == NULL)
        PANIC("ERROR: Failed to create window!\n");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);

    return window;
}

void load_opengl_functions(void)
{
    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version == GL_FALSE)
        PANIC("ERROR: Failed to load OpenGL functions!\n");

    GLint major_version, minor_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);

    LOG_MESSAGE("INFO: Using OpenGL Version %u.%u\n", major_version, minor_version);

    // only enable logging if supported
    if (major_version > 3 && minor_version >= 3) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_message_callback, 0); 
    }   
}