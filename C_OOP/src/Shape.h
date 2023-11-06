#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct Shape
{
    /* data */
    int64_t x_;
    int64_t y_;
    void (*constructor)(struct Shape *const, int64_t, int64_t);
    void (*move)(struct Shape *const, int64_t x, int64_t y);
    int64_t (*getX)(const struct Shape *const);
    int64_t (*getY)(const struct Shape *cosnt);

    // 多态
    // 虚函数表
    struct ShapeVtbl
    {
        void (*draw)(struct Shape const *const self);
    } *vptr;

    // 虚函数
    void (*draw)(struct Shape const *const self);


} Shape;

// Shape 的操作函数，接口函数
void Shape_constructor(Shape *const self, int64_t x, int64_t y);
void Shape_move(Shape *const self, int64_t x, int64_t y);
int64_t Shape_getX(const Shape *const self);
int64_t Shape_getY(const Shape *const self);

// 多态
void Shape_draw(Shape const *const self);
void Shape_draw_(Shape const *const self);
