#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
//#include <SFML/Graphics.hpp>
#include "../sfmlwrap/image.hpp"
#include "../geometry/point2d.hpp"
#include "scene.hpp"
#include "../widgets/widget.hpp"


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

    EventHandlerState on_paint_event(const Event *event) override;
    
//------------------------------------------------------------
private:

    RaytracerRepaint raytracer_put_pixel_();

//----------------------------Variables-----------------------
    bool requires_repaint_ = true;

    Scene *scene_   = nullptr;
    ImageSf *image_ = nullptr;

    Point2d last_rendered_pixel_{0, 0};
//------------------------------------------------------------
};

#endif