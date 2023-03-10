#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "util.h"
#include "lineal.h"
#include "renderer.h"
#include "button.h"
#include "glyphs.h"

GLFWwindow *window;
int ww, wh;

Renderer r = {0};
Glyph_Atlas atlas = {0}; 
Button btn;

void cleanup(void);
void glfw_process_keyboard_input(GLFWwindow *window);
void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void btn0_on_click(int btn);

int main()
{
    atexit(cleanup);
    // Initialize and configure GLFW
    set_up_glfw();
    // Create and configure window
    window = create_window(glfw_framebuffer_size_callback, glfw_mouse_button_callback);
    // Load OpenGL functions
    load_opengl_functions();

    glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    renderer_init(&r, "shaders/cterm.vert", "shaders/color.frag", "shaders/texture.frag", "shaders/text.frag");
    texture_new("resources/red_button.jpg");

    FT_Library ft_library = {0};
    freetype_init(&ft_library);
    glyphs_atlas_init(&atlas, ft_library, "resources/VictorMono-Regular.ttf");

    // main loop
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glfwPollEvents();
        glfw_process_keyboard_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Vec2f pos = vec2f(-200.0f, 0.0f);
        renderer_set_shader(&r, SHADER_TEXT);
        glyphs_render_text(&atlas, &r, "Hello OpenGL!", &pos, vec4f(0.f, 1.f, 0.f, 1.f));

        r.time = glfwGetTime();
        r.ww = ww;
        r.wh = wh;
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

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    ww = width;
    wh = height;
    glViewport(0, 0, ww, wh);
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void)mods;
    double cx, cy;
    glfwGetCursorPos(window, &cx, &cy);
    glfwGetWindowSize(window, &ww, &wh);

    cx = cx / (double)ww * 2 - 1;
    cy = cy / (double)wh * 2 - 1;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        button_update(&btn, (float)cx, (float)cy, button);
    }

}

void btn0_on_click(int btn)
{
    if (btn == GLFW_MOUSE_BUTTON_LEFT)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
