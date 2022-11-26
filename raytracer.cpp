#include "raytracer.hpp"


Raytracer::Raytracer(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent),
    scene_(new Scene{}),
    image_(new Image{static_cast<unsigned int> (rectangle.get_width()),
                     static_cast<unsigned int> (rectangle.get_height())}),
    last_rendered_pixel_(Point2d{0, 0})
{
    scene_->fill_scene(rectangle.get_width(), rectangle.get_height());

    id_ = 4;
    std::cout << "id: " << id_ << std::endl;
    //init_surface();
}


Raytracer::~Raytracer()
{
    delete scene_;
    delete image_;
}


// void Raytracer::output_certain_widget_form_(Color color)
// {
//     unsigned x = static_cast<unsigned> (last_rendered_pixel_.get_x());
//     unsigned y = static_cast<unsigned> (last_rendered_pixel_.get_y());
//     //std::cout << "raytracer: x is " << x << " y is " << y << std::endl;

//     if (y >= image_->get_height())
//     {
//         //std::cout << "update_surface for raytracer return" << std::endl;
//         return;
//     }
//     //std::cout << "rendering pixel " << x << " " << y << std::endl;
//     image_->render_pixel(scene_->camera_, scene_->spheres_, x, (image_->get_height() - 1) - y);

//     ++x;
//     bool carry = x == image_->get_width() ? true : false;                                                   // was -1
//     if (carry)
//     {
//         ++y;
//         x = 0;
//     }

//     last_rendered_pixel_.set_x(x);
//     last_rendered_pixel_.set_y(y);

//     surface_->clear(0);
//     surface_->draw_image(*image_);
//     surface_->update();
// }

RaytracerRepaint Raytracer::raytracer_put_pixel_()
{
    unsigned x = static_cast<unsigned> (last_rendered_pixel_.get_x());
    unsigned y = static_cast<unsigned> (last_rendered_pixel_.get_y());
    //std::cout << "raytracer: x is " << x << " y is " << y << std::endl;

    if (y >= image_->get_height())
    {
        return RaytracerRepaint::Skipped;
    }
    //std::cout << "rendering pixel " << x << " " << y << std::endl;
    image_->render_pixel(scene_->camera_, scene_->spheres_, x, (image_->get_height() - 1) - y);

    ++x;
    bool carry = x == image_->get_width() ? true : false;                                                   // was -1
    if (carry)
    {
        ++y;
        x = 0;
    }

    last_rendered_pixel_.set_x(x);
    last_rendered_pixel_.set_y(y);

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

    raytracer_put_pixel_();

    draw_frame_();
    // sprite_.load_from_surface(surface_);
    // sprite_.set_position(area_.get_x(), area_.get_y());

    return EventHandlerState::Accepted;
}

// void Raytracer::init_surface(const char *texture_file_name)
// {
//     assert(texture_file_name != nullptr);

//     if (texture_file_name == NO_TEXTURE_REQUIRED)
//     {
//         update_surface();
//     }
//     else
//     {
//         // load_texture
//     }
// }

// void Raytracer::update_surface()
// {
//     //std::cout << "update_surface for raytracer called" << std::endl;

//     unsigned x = static_cast<unsigned> (last_rendered_pixel_.get_x());
//     unsigned y = static_cast<unsigned> (last_rendered_pixel_.get_y());
//     //std::cout << "raytracer: x is " << x << " y is " << y << std::endl;

//     if (y >= image_->get_height())
//     {
//         //std::cout << "update_surface for raytracer return" << std::endl;
//         return;
//     }
//     //std::cout << "rendering pixel " << x << " " << y << std::endl;
//     image_->render_pixel(scene_->camera_, scene_->spheres_, x, (image_->get_height() - 1) - y);

//     ++x;
//     bool carry = x == image_->get_width() - 1 ? true : false;
//     if (carry)
//     {
//         ++y;
//         x = 0;
//     }

//     last_rendered_pixel_.set_x(x);
//     last_rendered_pixel_.set_y(y);

//     surface_->draw_image(*image_);
//     sprite_.load_from_surface(surface_);
//     sprite_.set_position(area_.get_x(), area_.get_y());

//     //std::cout << "raytracer surface updated" << std::endl;
// }
