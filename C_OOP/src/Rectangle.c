#include "Rectangle.h"

void Rectangle_constructor(Rectangle *const self, int64_t x, int64_t y,
                           int64_t w, int64_t h)
{
    Shape_constructor(&self->super, x, y);

    self->width_ = w;
    self->height_ = h;
    self->constructor = Rectangle_constructor;
    self->getW = Rectangle_getW;
    self->getH = Rectangle_getH;

    // 多态
    static struct ShapeVtbl vtbl =
    {
        /* data */
        &Rectangle_draw
    };
    self->super.vptr = &vtbl;
    self->draw = Rectangle_draw_;
}

int64_t Rectangle_getW(Rectangle const *const self)
{
    return self->width_;
}

int64_t Rectangle_getH(Rectangle const *const self)
{
    return self->height_;
}

void Rectangle_draw(Shape const *const self)
{
    Rectangle const * const self_ = (Rectangle const *)self; // 显示转换
    printf("Rectangle_draw : (x = %ld, y = %ld, width = %ld, height = %ld)\n",
          self_->super.getX(&self_->super), self_->super.getY(&self_->super),
          self_->getW(self_), self_->getH(self_));
}

void Rectangle_draw_(Rectangle const *const self)
{
    // 调用虚函数表里的函数
    self->super.vptr->draw(self);
}