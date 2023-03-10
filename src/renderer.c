#include "renderer.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assert.h>
#include <stdlib.h>

#define BEZIER_ITERATIONS 256

struct Uniform_Definition {
    enum Uniform uniform;
    const char *name;
};

static const struct Uniform_Definition uniform_definitions[UNIFORM_COUNT] = {
    [UNIFORM_TIME] = {
        .uniform = UNIFORM_TIME,
        .name    = "time",
    },
    [UNIFORM_RESOLUTION] = {
        .uniform = UNIFORM_RESOLUTION,
        .name    = "resolution",
    },
};

void shader_new(unsigned int *shader, const char *vertex_path, const char *fragment_path, const char *geometry_path);
void renderer_flush(Renderer *r);
static void get_uniform_location(GLuint program, GLint locations[UNIFORM_COUNT]);

void renderer_init(Renderer *r, const char *vertex_path, const char *color_path, const char *texture_path, const char *text_path)
{
    // vertex attribute object
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);

    // vertex buffer object
    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (r->vertices), r->vertices, GL_DYNAMIC_DRAW);

    // position
    glVertexAttribPointer(VERTEX_ATTR_POS, 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void *) offsetof (struct Vertex, pos));
    glEnableVertexAttribArray(VERTEX_ATTR_POS);

    // color
    glVertexAttribPointer(VERTEX_ATTR_COL, 4, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void *) offsetof (struct Vertex, col));
    glEnableVertexAttribArray(VERTEX_ATTR_COL);

    // uv
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void *) offsetof (struct Vertex, uv));
    glEnableVertexAttribArray(VERTEX_ATTR_UV);

    // color shader compilation
    shader_new(&r->program[SHADER_COLOR], vertex_path, color_path, NULL);

    // texture shader compilation
    shader_new(&r->program[SHADER_TEXTURE], vertex_path, texture_path, NULL);

    // text shader compilation
    shader_new(&r->program[SHADER_TEXT], vertex_path, text_path, NULL);
}

void renderer_vertex(Renderer *r, Vec2f pos, Vec4f col, Vec2f uv)
{
    assert(r->vertices_count < VERTICES_CAPACITY);

    struct Vertex *v = &r->vertices[r->vertices_count++];
    v->pos = pos;
    v->col = col;
    v->uv  = uv;
}

void renderer_triangle(Renderer *r, Vec2f pos0, Vec2f pos1, Vec2f pos2, Vec4f col0, Vec4f col1, Vec4f col2, Vec2f uv0, Vec2f uv1, Vec2f uv2)
{
    renderer_vertex(r, pos0, col0, uv0);
    renderer_vertex(r, pos1, col1, uv1);
    renderer_vertex(r, pos2, col2, uv2);
}

void renderer_solid_rect(Renderer *r, float x, float y, float w, float h, Vec4f col)
{
    renderer_triangle(r, vec2f(x, y),     vec2f(x, y + h),     vec2f(x + w, y), col, col, col, vec2fs(0), vec2fs(0), vec2fs(0));
    renderer_triangle(r, vec2f(x, y + h), vec2f(x + w, y + h), vec2f(x + w, y), col, col, col, vec2fs(0), vec2fs(0), vec2fs(0));
}

void renderer_texture_rect(Renderer *r, float x, float y, float w, float h, float uv_x, float uv_y, float uv_w, float uv_h, Vec4f c)
{
    renderer_triangle(r, vec2f(x, y),       vec2f(x, y + h),          vec2f(x + w, y), 
                         c,                 c,                        c, 
                         vec2f(uv_x, uv_y), vec2f(uv_x, uv_y + uv_h), vec2f(uv_x + uv_w, uv_y));

    renderer_triangle(r, vec2f(x, y + h),          vec2f(x + w, y + h),             vec2f(x + w, y),
                         c,                 c,                        c, 
                         vec2f(uv_x, uv_y + uv_h), vec2f(uv_x + uv_w, uv_y + uv_h), vec2f(uv_x + uv_w, uv_y));
}

void renderer_line(Renderer *r, Vec2f pos0, Vec2f pos1, Vec4f col, float stroke_width)
{
    float a0, b0, c0, b1, c1, phi;
    c0 = stroke_width / 2.f;
    b1 = pos0.y - pos1.y;
    c1 = vec2f_dst(pos0, pos1);
    phi = asinf(b1 / c1);
    a0 = cosf(phi) * c0;
    b0 = sinf(phi) * c0;

    Vec2f p0, p1, p2, p3;
    p0 = vec2f(pos0.x - b0, pos0.y - a0);
    p1 = vec2f(pos1.x - b0, pos1.y - a0);
    p2 = vec2f(pos0.x + b0, pos0.y + a0);
    p3 = vec2f(pos1.x + b0, pos1.y + a0);

    renderer_triangle(r, p0, p1, p2, col, col, col, vec2fs(0), vec2fs(0), vec2fs(0));
    renderer_triangle(r, p3, p2, p1, col, col, col, vec2fs(0), vec2fs(0), vec2fs(0));
}

void renderer_cubic_bezier(Renderer *r, Vec2f pos0, Vec2f pos1, Vec2f pos2, Vec2f pos3, Vec4f col, float stroke_width)
{
    float step, t;
    Vec2f a, b, c, d, e, p0, p1;

    p1   = pos0;
    step = 1.f / BEZIER_ITERATIONS;
    t    = 0.f;
    for (size_t i = 0; i <= BEZIER_ITERATIONS && t <= 1.f; ++i, t += step) {
        p0 = p1;

        a  = vec2f_lerp(pos0, pos1, t);
        b  = vec2f_lerp(pos1, pos2, t);
        c  = vec2f_lerp(pos2, pos3, t);
        d  = vec2f_lerp(a , b , t);
        e  = vec2f_lerp(b , c , t);
        p1 = vec2f_lerp(d , e , t);

        renderer_line(r, p0, p1, col, stroke_width);
    }
}

void renderer_set_shader(Renderer *r, enum Shader shader)
{
    renderer_flush(r);
    glUseProgram(r->program[shader]);
    get_uniform_location(r->program[shader], r->uniforms);
    glUniform1f(r->uniforms[UNIFORM_TIME], r->time);
    glUniform2f(r->uniforms[UNIFORM_RESOLUTION], r->ww, r->wh);
}

void renderer_flush(Renderer *r)
{
    // sync vertices
    glBufferSubData(GL_ARRAY_BUFFER, 0, r->vertices_count * sizeof (struct Vertex), r->vertices);
    // draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, r->vertices_count);
    // reset the vertices count
    r->vertices_count = 0;
}

void texture_new(const char *texture_image_path)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int w, h, ch;
    unsigned char *data;

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(texture_image_path, &w, &h, &ch, 0);
    if (data == NULL)
        PANIC("ERROR: Failed to load texture \'%s\'\n", texture_image_path);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free((void *) data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

static void get_uniform_location(GLuint program, GLint locations[UNIFORM_COUNT])
{
    for (enum Uniform uniform = 0; uniform < UNIFORM_COUNT; ++uniform) {
        locations[uniform] = glGetUniformLocation(program, uniform_definitions[uniform].name);
    }
}