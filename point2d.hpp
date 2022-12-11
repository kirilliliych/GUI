#ifndef POINT2D_HPP
#define POINT2D_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>


class Point2d
{
public:
    
    float x = 0;
    float y = 0;

    Point2d(){}

    Point2d(float x, float y)
      : x(x),
        y(y)
    {
        assert(std::isfinite(x));
        assert(std::isfinite(y));
    }

    Point2d(int x, int y)
      : Point2d((float) x, (float) y)
    {}


    bool operator ==(const Point2d &another_point) const
    {
        return ((x == another_point.x) && (y == another_point.y));
    }

    bool operator !=(const Point2d &another_point) const
    {
        return !(*this == another_point);
    }

    bool operator <=(const Point2d &another_point) const
    {
        return x == another_point.x ? y <= another_point.y : x <= another_point.x;
    }

    bool operator <(const Point2d &another_point) const
    {
        return x == another_point.x ? y <  another_point.y : x  < another_point.x;
    }

    bool operator >=(const Point2d &another_point) const
    {
        return x == another_point.x ? y >= another_point.y : x >= another_point.x;
    }

    bool operator >(const Point2d &another_point) const
    {
        return x == another_point.x ? y >  another_point.y : x >  another_point.x; 
    }

    void operator +=(const Point2d &another_point)
    {
        x += another_point.x;
        y += another_point.y;
    }

    void operator -=(const Point2d &another_point)
    {
        x -= another_point.x;
        y -= another_point.y;
    }

    Point2d operator +(const Point2d &another_point) const
    {
        Point2d result = *this;
        result += another_point;        

        return result;
    }
    
    Point2d operator -(const Point2d &another_point) const
    {
        Point2d result = *this;
        result -= another_point;

        return result;
    }

    Point2d operator -() const
    {
        return Point2d{-x, -y};
    }

};

#endif