#include "glyphs.h"

#include "util.h"

#include <string.h>

void glyphs_atlas_init(Glyph_Atlas *atlas, FT_Library ftl, const char *font_path)
{
    // initialize FT_Face
    FT_Face face;
    FT_Error error;

    error = FT_New_Face(ftl, font_path, 0, &face);
    if (error == FT_Err_Unknown_File_Format)
        PANIC("ERROR: \"%s\" has an unsupported file format!\n", font_path);
    else if (error)
        PANIC("ERROR: Failed to load font file \"%s\"!\n", font_path);

    // set font size
    FT_Set_Pixel_Sizes(face, 0, GLYPH_FONT_SIZE);
    
    // load characters
    FT_GlyphSlot g = face->glyph;
    for (size_t i = 32; i < GLYPHS_CAPACITY; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            LOG_MESSAGE("INFO: Failed to load character \"%c\" during font initialization.\n", (char)i);
            continue;
        }

        atlas->atlas_width += g->bitmap.width;
        if (atlas->atlas_height < g->bitmap.rows) {
            atlas->atlas_height = g->bitmap.rows;
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        (GLsizei) atlas->atlas_width,
        (GLsizei) atlas->atlas_height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL);

    int x = 0;
    for (int i = 32; i < GLYPHS_CAPACITY; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            LOG_MESSAGE("INFO: Failed to load character \"%c\" during font initialization.\n", (char)i);
            continue;
        }

        if (FT_Render_Glyph(g, FT_RENDER_MODE_NORMAL)) {
            LOG_MESSAGE("INFO: Failed to render character \"%c\" during font initialization.\n", (char)i);
            continue;
        }

        atlas->metrics[i].ax = g->advance.x >> 6;
        atlas->metrics[i].ay = g->advance.y >> 6;
        atlas->metrics[i].bw = g->bitmap.width;
        atlas->metrics[i].bh = g->bitmap.rows;
        atlas->metrics[i].bl = g->bitmap_left;
        atlas->metrics[i].bt = g->bitmap_top;
        atlas->metrics[i].tx = (float) x / (float) atlas->atlas_width;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            0,
            g->bitmap.width,
            g->bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            g->bitmap.buffer);
        x += g->bitmap.width;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void glyphs_render_text(Glyph_Atlas *atlas, Renderer *r, const char *text, Vec2f *pos, Vec4f color)
{
    float text_len = strnlen(text, 1024);
    for (size_t i = 0; i < text_len; ++i) {
        size_t glyph_index = text[i];
        if (glyph_index >= GLYPHS_CAPACITY) {
            glyph_index = '?';
        }
        Glyph_Metric metric = atlas->metrics[glyph_index];
        float x2 = pos->x + metric.bl;
        float y2 = -pos->y - metric.bt;
        float w  = metric.bw;
        float h  = metric.bh;

        pos->x += metric.ax;
        pos->y += metric.ay;

        renderer_texture_rect(r, x2, -y2, w, -h, metric.tx, 0.0f, metric.bw / (float) atlas->atlas_width, metric.bh / (float) atlas->atlas_height, color);
    }
}
