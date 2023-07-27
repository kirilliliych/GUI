#ifndef POINT3D_HPP
#define POINT3D_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>


class Point3d
{
private:
    
    float x_ = 0;
    float y_ = 0;
    float z_ = 0;

public:

    Point3d(){}

    Point3d(float x, float y, float z = 0)
      : x_(x),
        y_(y),
        z_(z)
    {
        assert(std::isfinite(x_));
        assert(std::isfinite(y_));
        assert(std::isfinite(z_));
    }

    float get_x() const
    {
        return x_;
    }
    float get_y() const
    {
        return y_;
    }
    float get_z() const
    {
        return z_;
    }

    void set_x(float x)
    {
        x_ = x;
    }
    void set_y(float y)
    {
        y_ = y;
    }
    void set_z(float z)
    {
        z_ = z;
    }

    void set_point3d(float x, float y, float z = 0)
    {
        assert(std::isfinite(x));
        assert(std::isfinite(y));
        assert(std::isfinite(z));

        x_ = x;
        y_ = y;
        z_ = z;
    }
};

#endif