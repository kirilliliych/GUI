#include "raytracer.hpp"


Raytracer::Raytracer(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent),
    scene_(new Scene{}),
    image_(new ImageSf{(rectangle.get_width()),
                       (rectangle.get_height())
                      }),
    last_rendered_pixel_(Point2d{0, 0})
{
    scene_->fill_scene(rectangle.get_width(), rectangle.get_height());

    //hide();
}

Raytracer::~Raytracer()
{
    delete scene_;
    delete image_;
}


RaytracerRepaint Raytracer::raytracer_put_pixel_()
{
    unsigned x = static_cast<unsigned> (last_rendered_pixel_.x);
    unsigned y = static_cast<unsigned> (last_rendered_pixel_.y);

    if (y >= image_->get_height())
    {
        return RaytracerRepaint::Skipped;
    }
    image_->render_pixel(scene_->camera_, scene_->spheres_, x, (image_->get_height() - 1) - y);

    ++x;
    bool carry = x == image_->get_width() ? true : false; 
    if (carry)
    {
        ++y;
        x = 0;
    }

    last_rendered_pixel_.x = x;
    last_rendered_pixel_.y = y;

    surface_->clear(0);
    surface_->draw_image(*image_);

    return RaytracerRepaint::Accepted;
}

EventHandlerState Raytracer::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    if (!hidden_)
    {
        raytracer_put_pixel_();
    }

    draw_frame_();

    return EventHandlerState::Accepted;
}

