#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP


#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "../geometry/point2d.hpp"


class Rectangle
{
public:
//------------------------------------------------------------------
    Rectangle()
      : Rectangle(0, 0, 0, 0)
    {}

    Rectangle(int x, int y, int width, int height)
      : x_(x),
        y_(y),
        width_(width),
        height_(height)
    {}

    Rectangle(const Rectangle &other)
      : Rectangle(other.x_, other.y_, other.width_, other.height_)
    {}
//------------------------------------------------------------------
    Point2d get_top_left_corner_coords() const
    {
        return Point2d{static_cast<float> (x_), static_cast<float> (y_)};
    }
    Point2d get_top_right_corner_coords() const
    {
        return Point2d{static_cast<float> (x_ + width_ - 1), static_cast<float> (y_)};
    }
    Point2d get_bottom_left_corner_coords() const
    {
        return Point2d{static_cast<float> (x_), static_cast<float> (y_ + height_ - 1)};
    }
    Point2d get_bottom_right_corner_coords() const
    {
        return Point2d{static_cast<float> (x_ + width_ - 1), static_cast<float> (y_ + height_ - 1)};
    }


    int get_x() const
    {
        return x_;
    }
    int get_y() const
    {
        return y_;
    }
    int get_width() const
    {
        return width_;
    }
    int get_height() const
    {
        return height_;
    }


    void set_x(int x)
    {
        x_ = x;
    }
    void set_y(int y)
    {
        y_ = y;
    }
    void set_width(int width)
    {
        width_  = width;
    }
    void set_height(int height)
    {
        height_ = height;
    }


    bool contains(const Point2d &position) const
    {
        return ((position.x >= x_)               &&
                (position.x <= x_ + width_  - 1) &&
                (position.y >= y_)               &&
                (position.y <= y_ + height_ - 1));
    }


//--------------------------Variables--------------------------------
private:

    int x_ = 0;
    int y_ = 0;
    int width_  = 0;
    int height_ = 0;
};
//-------------------------------------------------------------------

sf::IntRect to_SFML_IntRect(const Rectangle &rectangle);


#endif
