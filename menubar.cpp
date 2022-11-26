#include "menubar.hpp"


Menubar::Menubar(const Rectangle &rectangle, const Color &color, Widget *parent)
  : Widget(rectangle, parent),
    color_(color)
{
    id_ = 5;
    std::cout << "id: " << id_ << std::endl;
    //init_surface();
}


EventHandlerState Menubar::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    // output_certain_widget_form_();
    surface_->draw_rectangle(area_, color_);

    draw_frame_();
    // sprite_.load_from_surface(surface_);
    // sprite_.set_position(area_.get_x(), area_.get_y());

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}


// void Menubar::output_certain_widget_form_(Color color)
// {
//     surface_->clear(0);

//     color_ = color;
//     surface_->draw_rectangle(area_, color_);

//     surface_->update();
// }
