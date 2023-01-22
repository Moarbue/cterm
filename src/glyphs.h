#ifndef _CTERM_GLYPHS_H_
#define _CTERM_GLYPHS_H_

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glad/glad.h"

#include "lineal.h"
#include "renderer.h"

#define GLYPH_FONT_SIZE 64
#define GLYPHS_CAPACITY 128 // only support ASCII characters

typedef struct {
    float ax; // advance.x
    float ay; // advance.y

    float bw; // bitmap.width;
    float bh; // bitmap.rows;

    float bl; // bitmap_left;
    float bt; // bitmap_top;

    float tx; // x offset of glyph in texture coordinates
} Glyph_Metric;


typedef struct {
    FT_UInt atlas_width;
    FT_UInt atlas_height;
    GLuint glyphs_texture;
    Glyph_Metric metrics[GLYPHS_CAPACITY];
} Glyph_Atlas;

void glyphs_atlas_init(Glyph_Atlas *atlas, FT_Library ftl, const char *font_path);
void glyphs_render_text(Glyph_Atlas *atlas, Renderer *r, const char *text, Vec2f *pos, Vec4f color);

#endif // _CTERM_GLYPHS_H_