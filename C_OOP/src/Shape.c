#include "Shape.h"

// 构造函数
void Shape_constructor(Shape *const self, int64_t x, int64_t y)
{
    self->x_ = x;
    self->y_ = y;
    self->constructor = Shape_constructor;
    self->move = Shape_move;
    self->getX = Shape_getX;
    self->getY = Shape_getY;

    // 虚表
    static struct ShapeVtbl vtbl =
    {
        /* data */
        &Shape_draw
    };
    self->vptr = &vtbl;
    self->draw = Shape_draw_;
}

void Shape_move(Shape *const self, int64_t x, int64_t y)
{
    self->x_ += x;
    self->y_ += y;
}

// 获取属性值
int64_t Shape_getX(Shape const *const self)
{
    return self->x_;
}

int64_t Shape_getY(Shape const *const self)
{
    return self->y_;
}

void Shape_draw(Shape const *const self)
{
    printf("Shape_draw : (x = %ld, y = %ld)\n",
          self->getX(self), self->getY(self));
}

void Shape_draw_(Shape const *const self)
{
    // 调用虚函数表里的函数
    self->vptr->draw(self);
}