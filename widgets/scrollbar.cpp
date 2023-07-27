#include "scrollbar.hpp"


Scrollbar::Scrollbar(const Rectangle &rectangle, ScrollbarOrientation orientation, int carriage_size, Widget *parent)
  : Widget(rectangle, parent),
    orientation_(orientation)
{
    if (orientation == ScrollbarOrientation::Vertical)
    {
        carriage_ = {0, 0, rectangle.get_width(), carriage_size};
    }
    else
    {
        carriage_ = {0, 0, carriage_size, rectangle.get_height()};
    }
    cur_value_ = 0;
    update_carriage_();
}

Scrollbar::Scrollbar(int x, int y, int w, int h, ScrollbarOrientation orientation,
                     int scroll_size, Widget *parent)
  : Scrollbar({x, y, w, h}, orientation, scroll_size, parent)
{}
    
Scrollbar::~Scrollbar()
{}

void Scrollbar::set_min_value(const long long value)
{
    min_value_ = value;

    requires_repaint_ = true;
}

void Scrollbar::set_max_value(const long long value)
{
    max_value_ = value;

    requires_repaint_ = true;
}

void Scrollbar::set_cur_value(const long long value)
{
    cur_value_ = std::max(min_value_, std::min(max_value_, value));
    value_changed.emit(value);

    requires_repaint_ = true;
}

EventHandlerState Scrollbar::on_mouse_button_pressed_event (const Event *event)
{
    assert(event != nullptr);

    Point2d point = event->mbedata_.position - area_.get_top_left_corner_coords();
    if ((event->mbedata_.button == MouseButton::Left) && carriage_.contains(point))
    {
        captured_ = true;

        return EventHandlerState::Accepted;
    }

    return EventHandlerState::Skipped;
}

EventHandlerState Scrollbar::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

    if (event->mbedata_.button == MouseButton::Left)
    {
        captured_ = false;

        return EventHandlerState::Accepted; 
    }

    return EventHandlerState::Skipped;
}

EventHandlerState Scrollbar::on_mouse_moved_event(const Event *event)
{
    assert(event != nullptr);

    if (captured_)
    {
        int width    = orientation_ == ScrollbarOrientation::Horizontal ? area_.get_width() : area_.get_height();
        int position = orientation_ == ScrollbarOrientation::Horizontal ? event->mmedata_.position.x - area_.get_x() :
                                                                          event->mmedata_.position.y - area_.get_y();
        int height = 0;
        int length = orientation_ == ScrollbarOrientation::Horizontal ? carriage_.get_width() : carriage_.get_height();
        position = std::max(height + length / 2, std::min(width - height - length / 2, position)) - height - length / 2;

        cur_value_ = position * (max_value_ - min_value_) / (width - 2 * height - length) + min_value_;
        update_carriage_();
        value_changed.emit(cur_value_);

        requires_repaint_ = true;

        return EventHandlerState::Accepted;
    }

    return EventHandlerState::Skipped;
}

EventHandlerState Scrollbar::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    update_carriage_();
    surface_->clear(LIGHT_GREY);
    Point2d start{carriage_.get_x(), carriage_.get_y()};
    surface_->draw_rectangle(carriage_, start, GREY);

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}

void Scrollbar::update_carriage_()
{
    int width  = orientation_ == ScrollbarOrientation::Horizontal ? area_.get_width()     : area_.get_height();
    int length = orientation_ == ScrollbarOrientation::Horizontal ? carriage_.get_width() : carriage_.get_height();
    int height = 0;
    long long result = std::min(static_cast<long long> (width - 2 * height - length),
                       std::max(static_cast<long long> (0), (cur_value_ - min_value_) * (width - 2 * height - length) / (max_value_ - min_value_))) + height;

    if (orientation_ == ScrollbarOrientation::Horizontal)
    {
        carriage_.set_x(result);
    }
    else
    {
        carriage_.set_y(result);
    }
}
