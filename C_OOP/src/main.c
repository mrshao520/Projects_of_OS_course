#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Shape.h"
#include "Rectangle.h"

int main()
{
    // 普通实现
    Shape shape;
    Shape_constructor(&shape, 2, 4);
    printf("Shape shape(x = %ld, y = %ld)\n", Shape_getX(&shape), Shape_getY(&shape));

    // 使用函数指针实现封装
    Shape other_shape;
    Shape_constructor(&other_shape, 3, 6);
    printf("Shape other_shape(x = %ld, y = %ld)\n", other_shape.getX(&other_shape), other_shape.getY(&other_shape));

    // 继承
    Rectangle rectangle;
    Rectangle_constructor(&rectangle, 3, 6, 4, 8);
    printf("Shape rectangle(x = %ld, y = %ld, w = %ld, h = %ld)\n", 
            rectangle.super.getX(&rectangle.super), rectangle.super.getY(&rectangle.super), 
            rectangle.getW(&rectangle), rectangle.getH(&rectangle));

    // 多态
    Rectangle other_retangle;
    Rectangle_constructor(&other_retangle, 1, 2, 3, 4);
    other_retangle.draw(&other_retangle);
    other_shape.draw(&other_shape);

    return EXIT_SUCCESS;
}