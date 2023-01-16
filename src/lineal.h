#ifndef _CTERM_LINEAL_H_
#define _CTERM_LINEAL_H_

typedef struct {
    float x;
    float y;
} Vec2f;

Vec2f vec2f(float x, float y);
Vec2f vec2fs(float x);
Vec2f vec2f_add(Vec2f a, Vec2f b);
Vec2f vec2f_sub(Vec2f a, Vec2f b);
Vec2f vec2f_mul(Vec2f a, Vec2f b);
Vec2f vec2f_div(Vec2f a, Vec2f b);
float vec2f_mag(Vec2f a);
float vec2f_dst(Vec2f a, Vec2f b);
float vec2f_phi(Vec2f a);
Vec2f vec2f_coord(float mag, float phi);
float vec2f_dot(Vec2f a, Vec2f b);

typedef struct {
    int x;
    int y;
} Vec2i;

Vec2i vec2i(int x, int y);
Vec2i vec2is(int x);
Vec2i vec2i_add(Vec2i a, Vec2i b);
Vec2i vec2i_sub(Vec2i a, Vec2i b);
Vec2i vec2i_mul(Vec2i a, Vec2i b);
Vec2i vec2i_div(Vec2i a, Vec2i b);
int   vec2i_dot(Vec2i a, Vec2i b);

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec4f;

Vec4f vec4f(float x, float y, float z, float w);
Vec4f vec4fs(float x);
Vec4f vec4f_add(Vec4f a, Vec4f b);
Vec4f vec4f_sub(Vec4f a, Vec4f b);
Vec4f vec4f_mul(Vec4f a, Vec4f b);
Vec4f vec4f_div(Vec4f a, Vec4f b);
float vec4f_dot(Vec4f a, Vec4f b);

typedef struct {
    int x;
    int y;
    int z;
    int w;
} Vec4i;

Vec4i vec4i(int x, int y, int z, int w);
Vec4i vec4is(int x);
Vec4i vec4i_add(Vec4i a, Vec4i b);
Vec4i vec4i_sub(Vec4i a, Vec4i b);
Vec4i vec4i_mul(Vec4i a, Vec4i b);
Vec4i vec4i_div(Vec4i a, Vec4i b);
int   vec4i_dot(Vec4i a, Vec4i b);


float lerp(float a, float b, float t);
float clampf(float v, float low, float high);

#endif // _CTERM_LINEAL_H_