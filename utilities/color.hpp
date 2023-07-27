#ifndef COLOR_HPP
#define COLOR_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../utilities/utilities.hpp"
#include "../geometry/vector3d.hpp"


const uint8_t COLOR_MAX_VALUE     = 0xFF;
const float GAMMA_CORRECTION_COEF = 0.5;


class Color
{
public:

    Color();

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

    Color(float r, float g, float b, float a = 1);

    Color(unsigned rgba);

    uint8_t get_r() const
    {
        return color_.r;
    }
    uint8_t get_g() const
    {
        return color_.g;
    }
    uint8_t get_b() const
    {
        return color_.b;
    }
    uint8_t get_a() const
    {
        return color_.a;
    }

    float get_red_f() const
    {
        return red_f_;
    }
    float get_green_f() const
    {
        return green_f_;
    }
    float get_blue_f() const
    {
        return blue_f_;
    }
    float get_alpha_f() const
    {
        return alpha_f_;
    }
    
    uint32_t get_uint32_color() const
    {
        return color_.toInteger();
    }

    const sf::Color &get_color_ref() const
    {
        return color_;
    }

    void set_r(uint8_t r)
    {
        color_.r = r;

        red_f_   = (float) r / COLOR_MAX_VALUE;
    }
    void set_g(uint8_t g)
    {
        color_.g = g;

        green_f_ = (float) g / COLOR_MAX_VALUE;
    }
    void set_b(uint8_t b)
    {
        color_.b = b;

        blue_f_  = (float) b / COLOR_MAX_VALUE;
    }
    void set_a(uint8_t a)
    {
        color_.a = a;

        alpha_f_ = (float) a / COLOR_MAX_VALUE;
    }
    
    void set_red_d(float red_d)
    {
        red_f_ = red_d;

        limit_floats();
        update_ints();
    }
    void set_green_d(float green_d)
    {
        green_f_ = green_d;

        limit_floats();
        update_ints();
    }
    void set_blue_d(float blue_d)
    {
        blue_f_ = blue_d;

        limit_floats();
        update_ints();
    }
    void set_alpha_d(float alpha_d)
    {
        alpha_f_ = alpha_d;

        limit_floats();
        update_ints();
    }

    void make_transparent()
    {
        set_a(0x00);
    }

    void set_color(uint32_t rgba)
    {
        color_.r = (uint8_t) ((rgba & 0xFF000000) >> 24);
        color_.g = (uint8_t) ((rgba & 0x00FF0000) >> 16);
        color_.b = (uint8_t) ((rgba & 0x0000FF00) >> 8);
        color_.a = (uint8_t)  (rgba & 0x000000FF);

        update_floats();
    }

    Color gamma_correction(float exponent)
    {
        red_f_   = powf(red_f_,   exponent);
        green_f_ = powf(green_f_, exponent);
        blue_f_  = powf(blue_f_,  exponent);

        update_ints();

        return *this;
    }


    void operator +=(const Color &another_color)
    {
        red_f_   += another_color.red_f_;
        green_f_ += another_color.green_f_;
        blue_f_  += another_color.blue_f_;
        
        limit_floats();
        update_ints();
    }

    void operator *=(const Color &another_color)
    {
        red_f_   *= another_color.red_f_;
        green_f_ *= another_color.green_f_;
        blue_f_  *= another_color.blue_f_;

        limit_floats();
        update_ints();
    }

    void operator *=(float scalar)
    {
        red_f_   *= scalar;
        green_f_ *= scalar;
        blue_f_  *= scalar;
        
        limit_floats();
        update_ints();
    }

    void operator /=(float scalar)
    {
        red_f_   /= scalar;
        green_f_ /= scalar;
        blue_f_  /= scalar;

        limit_floats();
        update_ints();
    }

    Color operator +(const Color &another_color) const
    {
        Color result = *this;

        result += another_color;

        result.limit_floats();
        result.update_ints();

        return result;
    }

    Color operator *(const Color &another_color) const
    {
        Color result = *this;

        result *= another_color;

        result.limit_floats();
        result.update_ints();

        return result;
    }

    Color operator *(float scalar) const
    {
        Color result = *this;

        result *= scalar;

        result.limit_floats();
        result.update_ints();

        return result;
    }

    Color operator /(float scalar) const
    {
        Color result = *this;

        result /= scalar;

        result.limit_floats();
        result.update_ints();

        return result;
    }


private:
    
    void limit_floats()
    {
        if (red_f_ > 1)
        {
            red_f_ = 1;
        }

        if (green_f_ > 1)
        {
            green_f_ = 1;
        }

        if (blue_f_ > 1)
        {
            blue_f_ = 1;
        }

        if (alpha_f_ > 1)
        {
            alpha_f_ = 1;
        }

        if (red_f_ < 0)
        {
            red_f_ = 0;
        }

        if (green_f_ < 0)
        {
            green_f_ = 0;
        }

        if (blue_f_ < 0)
        {
            blue_f_ = 0;
        }

        if (alpha_f_ < 0)
        {
            alpha_f_ = 0;
        }

        update_ints();
    }

    void update_ints()
    {
        color_.r = (float) (red_f_   * COLOR_MAX_VALUE);
        color_.g = (float) (green_f_ * COLOR_MAX_VALUE);
        color_.b = (float) (blue_f_  * COLOR_MAX_VALUE);
        color_.a = (float) (alpha_f_ * COLOR_MAX_VALUE);
    }

    void update_floats()
    {
        red_f_   = (float) color_.r / COLOR_MAX_VALUE;
        green_f_ = (float) color_.g / COLOR_MAX_VALUE;
        blue_f_  = (float) color_.b / COLOR_MAX_VALUE;
        alpha_f_ = (float) color_.a / COLOR_MAX_VALUE;
    }


    float red_f_   = 0;
    float green_f_ = 0;
    float blue_f_  = 0; 
    float alpha_f_ = 1;

    sf::Color color_{};
};

Vector3d color_to_vector(const Color &color);

Vector3d color255_to_vector(const Color &color);

Color vector_to_color(const Vector3d &vector);


#endif