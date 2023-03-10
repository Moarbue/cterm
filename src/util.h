#ifndef _CTERM_UTIL_H_
#define _CTERM_UTIL_H_

#include "GLFW/glfw3.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#define PANIC(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
#define LOG_MESSAGE(...) ( printf(__VA_ARGS__) )

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

void set_up_glfw(void);
GLFWwindow * create_window(GLFWframebuffersizefun framebuffer_size_callback, GLFWmousebuttonfun mouse_button_callback);
void load_opengl_functions(void);

void freetype_init(FT_Library *ftl);

#endif // _CTERM_UTIL_H_