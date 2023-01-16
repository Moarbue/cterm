#include "button.h"

#include <assert.h>

#define IS_IN_RANGE(base, w, h, point) ((point).x > (base).x && (point).x < ((base).x + w) && (point).y > (base).y && (point).y < ((base).y + h))

void button(Button *btn, float x, float y, float w, float h, float uv_x, float uv_y, float uv_w, float uv_h, btn_click_callback onc_click)
{
    assert(btn != NULL);

    btn->base     = vec2f(x, y);
    btn->w        = w;
    btn->h        = h;
    btn->uv_base  = vec2f(uv_x, uv_y);
    btn->uv_w     = uv_w;
    btn->uv_h     = uv_h;
    btn->on_click = onc_click;
}

void button_update(Button *btn, float mouse_x, float mouse_y, int mouse_btn)
{
    if (IS_IN_RANGE(btn->base, btn->w, btn->h, vec2f(mouse_x, mouse_y))) {
        btn->on_click(mouse_btn);
    }
}

void button_render(Button *btn, Renderer *r)
{
    renderer_texture_rect(r, btn->base.x, btn->base.y,    btn->w,    btn->h,
                          btn->uv_base.x, btn->uv_base.y, btn->uv_w, btn->uv_h);
}
