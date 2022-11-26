#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "image.hpp"
#include "point2d.hpp"
#include "scene.hpp"
#include "widget.hpp"


enum class RaytracerRepaint
{
    Accepted = 0,
    Skipped
};


class Raytracer : public Widget
{
public:
//------------------------------------------------------------
    Raytracer(const Rectangle &rectangle, Widget *parent = nullptr);

    ~Raytracer();
//------------------------------------------------------------

    //void init_surface(const char *texture_file_name = NO_TEXTURE_REQUIRED);

    //void update_surface();

    EventHandlerState on_paint_event(const Event *event) override;
    
//------------------------------------------------------------

private:

    RaytracerRepaint raytracer_put_pixel_();
    //void output_certain_widget_form_(Color color = {0x00000000}) override;

//----------------------------Variables-----------------------
    Scene *scene_ = nullptr;
    Image *image_ = nullptr;

    Point2d last_rendered_pixel_{0, 0};
//------------------------------------------------------------
};

#endif