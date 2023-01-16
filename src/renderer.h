#ifndef _CTERM_RENDERER_H_
#define _CTERM_RENDERER_H_

#include "glad/glad.h"
#include "lineal.h"

enum Uniform {
    UNIFORM_TIME,
    UNIFORM_COUNT,
};

enum Vertex_Attributes {
    VERTEX_ATTR_POS,
    VERTEX_ATTR_COL,
    VERTEX_ATTR_UV,
    VERTEX_ATTR_COUNT,
};

enum Shader {
    SHADER_COLOR,
    SHADER_TEXTURE,
    SHADER_COUNT,
};

struct Vertex {
    Vec2f pos;
    Vec4f col;
    Vec2f uv;   
};

#define VERTICES_CAPACITY (64 * 1024)

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint program[SHADER_COUNT];
    GLint  uniforms[UNIFORM_COUNT];
    struct Vertex vertices[VERTICES_CAPACITY];
    size_t vertices_count;

    float time;
} Renderer;

void renderer_init(Renderer *r, const char *vertex_path, const char *color_path, const char *texture_path);
void renderer_vertex(Renderer *r, Vec2f pos, Vec4f col, Vec2f uv);
void renderer_triangle(Renderer *r, Vec2f pos0, Vec2f pos1, Vec2f pos2, Vec4f col0, Vec4f col1, Vec4f col2, Vec2f uv0, Vec2f uv1, Vec2f uv2);
void renderer_solid_rect(Renderer *r, float x, float y, float w, float h, Vec4f col);
void renderer_texture_rect(Renderer *r, float x, float y, float w, float h, float uv_x, float uv_y, float uv_w, float uv_h);
void renderer_set_shader(Renderer *r, enum Shader shader);
void renderer_flush(Renderer *r);

void texture_new(const char *texture_image_path);

#endif // _CTERM_RENDERER_H_