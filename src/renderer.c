#include "renderer.h"

#include <assert.h>
#include <stdlib.h>

struct Uniform_Definition {
    enum Uniform uniform;
    const char *name;
};

static const struct Uniform_Definition uniform_definitions[UNIFORM_COUNT] = {
    [UNIFORM_TIME] = {
        .uniform = UNIFORM_TIME,
        .name    = "time",
    },
};

void shader_new(unsigned int *shader, const char *vertex_path, const char *fragment_path, const char *geometry_path);
static void get_uniform_location(GLuint program, GLint locations[UNIFORM_COUNT]);

void renderer_init(Renderer *r, const char *vertex_path, const char *fragment_path)
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

    // shader compilation
    shader_new(&r->program, vertex_path, fragment_path, NULL);
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

void renderer_set_shader(Renderer *r, enum Shader)
{
    glUseProgram(r->program);
    get_uniform_location(r->program, r->uniforms);
    glUniform1f(r->uniforms[UNIFORM_TIME], r->time);
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

static void get_uniform_location(GLuint program, GLint locations[UNIFORM_COUNT])
{
    for (enum Uniform uniform = 0; uniform < UNIFORM_COUNT; ++uniform) {
        locations[uniform] = glGetUniformLocation(program, uniform_definitions[uniform].name);
    }
}