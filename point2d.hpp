#ifndef POINT2D_HPP
#define POINT2D_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>


class Point2d
{
private:
    
    float x_ = 0;
    float y_ = 0;

public:

    Point2d(){}

    Point2d(float x, float y)
      : x_(x),
        y_(y)
    {
        assert(std::isfinite(x_));
        assert(std::isfinite(y_));
    }

    Point2d(int x, int y)
      : Point2d((float) x, (float) y)
    {}

    float get_x() const
    {
        return x_;
    }
    float get_y() const
    {
        return y_;
    }

    void set_x(float x)
    {
        x_ = x;
    }
    void set_y(float y)
    {
        y_ = y;
    }

    void set_point2d(float x, float y)
    {
        assert(std::isfinite(x));
        assert(std::isfinite(y));

        x_ = x;
        y_ = y;
    }


    void operator +=(const Point2d &another_vector)
    {
        x_ += another_vector.x_;
        y_ += another_vector.y_;
    }

    void operator -=(const Point2d &another_vector)
    {
        x_ -= another_vector.x_;
        y_ -= another_vector.y_;
    }

    Point2d operator +(const Point2d &another_vector) const
    {
        Point2d result = *this;
        result += another_vector;        

        return result;
    }
    
    Point2d operator -(const Point2d &another_vector) const
    {
        Point2d result = *this;
        result -= another_vector;

        return result;
    }

    Point2d operator -() const
    {
        return {-x_, -y_};
    }
};

#endif