#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP


#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "point2d.hpp"


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
        return Point2d{static_cast<float> (x_ + width_), static_cast<float> (y_)};
    }
    Point2d get_bottom_left_corner_coords() const
    {
        return Point2d{static_cast<float> (x_), static_cast<float> (y_ + height_)};
    }
    Point2d get_bottom_right_corner_coords() const
    {
        return Point2d{static_cast<float> (x_ + width_), static_cast<float> (y_ + height_)};
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


    void set_x(unsigned x)
    {
        x_ = x;
    }
    void set_y(unsigned y)
    {
        y_ = y;
    }
    void set_width(unsigned width)
    {
        width_ = width;
    }
    void set_height(unsigned height)
    {
        height_ = height;
    }


    bool contains(const Point2d &point) const
    {
        return ((point.get_x() >= x_)          &&
                (point.get_x() <= x_ + width_) &&
                (point.get_y() >= y_)          &&
                (point.get_y() <= y_ + height_));
    }

private:

    int x_ = 0;
    int y_ = 0;
    int width_  = 0;
    int height_ = 0;
};


sf::IntRect to_SFML_IntRect(const Rectangle &rectangle);


#endif
