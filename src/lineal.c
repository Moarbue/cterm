#include "lineal.h"
#include <math.h>

Vec2f vec2f(float x, float y)
{
    return (Vec2f) {.x = x, .y = y};
}

Vec2f vec2fs(float x)
{
    return (Vec2f) {.x = x, .y = x};
}

Vec2f vec2f_add(Vec2f a, Vec2f b)
{
    return (Vec2f) {.x = a.x + b.x, .y = a.y + b.y};
}

Vec2f vec2f_sub(Vec2f a, Vec2f b)
{
    return (Vec2f) {.x = a.x - b.x, .y = a.y - b.y};
}

Vec2f vec2f_mul(Vec2f a, Vec2f b)
{
    return (Vec2f) {.x = a.x * b.x, .y = a.y * b.y};
}

Vec2f vec2f_div(Vec2f a, Vec2f b)
{
    return (Vec2f) {.x = a.x / b.x, .y = a.y / b.y};
}

float vec2f_dot(Vec2f a, Vec2f b)
{
    return a.x * b.x + a.y * b.y;
}

float vec2f_mag(Vec2f a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}
float vec2f_dst(Vec2f a, Vec2f b)
{
    Vec2f v0 = vec2f_sub(a, b);
    return vec2f_mag(v0);
}

float vec2f_phi(Vec2f a)
{
    if (a.x == 0) return M_PI_2 * (a.y < 0 ? -1 : 1);
    return atan2f(a.y, a.x);
}

Vec2f vec2f_coord(float mag, float phi)
{
    return vec2f(mag * cosf(phi), mag * sinf(phi));
}

//////////////////////////

Vec2i vec2i(int x, int y)
{
    return (Vec2i) {.x = x, .y = y};
}

Vec2i vec2is(int x)
{
    return (Vec2i) {.x = x, .y = x};
}

Vec2i vec2i_add(Vec2i a, Vec2i b)
{
    return (Vec2i) {.x = a.x + b.x, .y = a.y + b.y};
}

Vec2i vec2i_sub(Vec2i a, Vec2i b)
{
    return (Vec2i) {.x = a.x - b.x, .y = a.y - b.y};
}

Vec2i vec2i_mul(Vec2i a, Vec2i b)
{
    return (Vec2i) {.x = a.x * b.x, .y = a.y * b.y};
}

Vec2i vec2i_div(Vec2i a, Vec2i b)
{
    return (Vec2i) {.x = a.x / b.x, .y = a.y / b.y};
}

int vec2i_dot(Vec2i a, Vec2i b)
{
    return a.x * b.x + a.y * b.y;
}

//////////////////////////

Vec4f vec4f(float x, float y, float z, float w)
{
    return (Vec4f) {.x = x, .y = y, .z = z, .w = w};
}

Vec4f vec4fs(float x)
{
    return (Vec4f) {.x = x, .y = x, .z = x, .w = x};
}

Vec4f vec4f_add(Vec4f a, Vec4f b)
{
    return (Vec4f) {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
}

Vec4f vec4f_sub(Vec4f a, Vec4f b)
{
    return (Vec4f) {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
}

Vec4f vec4f_mul(Vec4f a, Vec4f b)
{
    return (Vec4f) {.x = a.x * b.x, .y = a.y * b.y, .z = a.z * b.z, .w = a.w * b.w};
}

Vec4f vec4f_div(Vec4f a, Vec4f b)
{
    return (Vec4f) {.x = a.x / b.x, .y = a.y / b.y, .z = a.z / b.z, .w = a.w / b.w};
}

float vec4f_dot(Vec4f a, Vec4f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec2f vec2f_lerp(Vec2f a, Vec2f b, float t)
{
    return vec2f(lerp(a.x, b.x, t),
                 lerp(a.y, b.y, t));
}

//////////////////////////

Vec4i vec4i(int x, int y, int z, int w)
{
    return (Vec4i) {.x = x, .y = y, .z = z, .w = w};
}

Vec4i vec4is(int x)
{
    return (Vec4i) {.x = x, .y = x, .z = x, .w = x};
}

Vec4i vec4i_add(Vec4i a, Vec4i b)
{
    return (Vec4i) {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
}

Vec4i vec4i_sub(Vec4i a, Vec4i b)
{
    return (Vec4i) {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
}

Vec4i vec4i_mul(Vec4i a, Vec4i b)
{
    return (Vec4i) {.x = a.x * b.x, .y = a.y * b.y, .z = a.z * b.z, .w = a.w * b.w};
}

Vec4i vec4i_div(Vec4i a, Vec4i b)
{
    return (Vec4i) {.x = a.x / b.x, .y = a.y / b.y, .z = a.z / b.z, .w = a.w / b.w};
}

int vec4i_dot(Vec4i a, Vec4i b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//////////////////////////

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float clampf(float v, float low, float high)
{
    return v < low ? low : (v > high ? high : v);
}