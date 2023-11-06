#pragma once

#include "Shape.h"

typedef struct Rectangle
{
    /* data */
    Shape super; // 继承 Shape

    int64_t width_;
    int64_t height_;

    void (*constructor)(struct Rectangle *const self, int64_t x, int64_t y,
                        int64_t w, int64_t h);
    int64_t (*getW)(struct Rectangle const *const self);
    int64_t (*getH)(struct Rectangle const *const self);
    void (*draw)(struct Rectangle const *const self);
} Rectangle;

void Rectangle_constructor(Rectangle *const self, int64_t x, int64_t y,
                           int64_t w, int64_t h);
int64_t Rectangle_getW(Rectangle const *const self);
int64_t Rectangle_getH(Rectangle const *const self);

void Rectangle_draw(Shape const *const self);
void Rectangle_draw_(Rectangle const *const self);