#ifndef _CTERM_BUTTON_H
#define _CTERM_BUTTON_H

#include "lineal.h"
#include "renderer.h"

typedef void (*btn_click_callback)(int btn);

typedef struct {
    Vec2f base;     // bottom left corner
    float w, h;     // width and height

    Vec2f uv_base;
    float uv_w, uv_h;

    btn_click_callback on_click;
} Button;

void button(Button *btn, float x, float y, float w, float h, float uv_x, float uv_y, float uv_w, float uv_h, btn_click_callback onc_click);
void button_update(Button *btn, float mouse_x, float mouse_y, int mouse_btn);
void button_render(Button *btn, Renderer *r);

#endif // _CTERM_BUTTON_H