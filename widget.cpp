#include "widget.hpp"


Widget::Widget(Widget *parent)
  : Widget(Rectangle{100, 100, 200, 200}, parent)
{
    std::cout << "CALLED FICTIVE WIDGET CONSTRUCTOR" << std::endl;
}

Widget::Widget(const Rectangle &rectangle, Widget *parent)
  : area_(rectangle),
    surface_(new Surface(rectangle.get_width(), rectangle.get_height())),
    skin_(new Texture{}),
    parent_(parent)        
{
    if (parent != nullptr)
    {
        parent->add_child(this);
    }
    else
    {
        std::cout << "widget parent nullptr" << std::endl;
    }
}

Widget::~Widget()
{
    delete surface_;
    delete skin_;
}


void Widget::set_event_manager(EventManager *event_manager)
{
    if ((event_manager == nullptr) || (event_manager_ptr_ != nullptr))
    {
        return;
    }

    //std::cout << "Widget event manager set, id " << event_manager->identif_ << std::endl;

    event_manager_ptr_ = event_manager;
    event_manager_ptr_->subscribe(this);
}

void Widget::add_child(Widget *child)
{
    assert(child != nullptr);
    assert(child != this);

    children_.push_back(child);
    child->set_event_manager(event_manager_ptr_);
    //std::cout << "add child to event manager with identif " << event_manager_ptr_->identif_ << std::endl;
}


void Widget::render(Surface *surface)
{
    if ((surface_ != nullptr) && (!hidden_))
    {
        //std::cout << "render: rendering " << id_ << std::endl;

        Sprite temp{};
        temp.load_from_surface(surface_);
        temp.set_position(area_.get_x(), area_.get_y());
        surface->draw_sprite(temp);
    
        for (int child_index = 0; child_index < children_.size(); ++child_index)
        {
            children_[child_index]->render(surface);
        }
    }
    else
    {
        std::cout << "Given surface is unitialized " << __LINE__ << " " << __FILE__ << std::endl;
    }
}


const Rectangle &Widget::get_area() const
{
    return area_;
}

void Widget::move(const Point2d &offset)
{
    area_.set_x(area_.get_x() + offset.x);
    area_.set_y(area_.get_y() + offset.y);
    for (int child_index = 0; child_index < children_.size(); ++child_index)
    {
        children_[child_index]->move(offset);
    }
}

const Point2d Widget::get_reference_point_for_event() const
{
    return area_.get_top_left_corner_coords();
}


bool Widget::contains(const Point2d &position)
{
    return hidden_ ? false : ignored_by_events_ ? false : area_.contains(position);
}


void Widget::show()
{
    hidden_ = false;
}

void Widget::hide()
{
    hidden_ = true;
}

bool Widget::is_hidden()
{
    return hidden_;
}

void Widget::make_ignored_by_events()
{
    ignored_by_events_ = true;
}

bool Widget::is_ignored_by_events() const
{
    return ignored_by_events_;
}


EventHandlerState Widget::handle_event                  (const Event *event)
{
    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_mouse_button_pressed_event (const Event *event)
{
    return event->mbedata_.button == MouseButton::Left ? 
                                     EventHandlerState::Accepted :
                                     EventHandlerState::Skipped;
}
EventHandlerState Widget::on_mouse_button_released_event(const Event *event)
{
    return event->mbedata_.button == MouseButton::Left ?
                                     EventHandlerState::Accepted :
                                     EventHandlerState::Skipped;
}
EventHandlerState Widget::on_mouse_moved_event          (const Event *event)
{
    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_mouse_scrolled_event       (const Event *event)
{
    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_mouse_entered_event        (const Event *event)
{
    return EventHandlerState::Accepted;
}
EventHandlerState Widget::on_mouse_left_event           (const Event *event)
{
    return EventHandlerState::Accepted;
}
EventHandlerState Widget::on_key_pressed_event          (const Event *event)
{
    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_key_released_event         (const Event *event)
{
    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_gained_focus_event         (const Event *event)
{
    in_focus_ = true;

    return EventHandlerState::Accepted;
}
EventHandlerState Widget::on_lost_focus_event           (const Event *event)
{
    in_focus_ = false;

    return EventHandlerState::Accepted;
}
EventHandlerState Widget::on_time_event                 (const Event *event)
{
    timer_ = event->timer_;

    return EventHandlerState::Skipped;
}
EventHandlerState Widget::on_paint_event                (const Event *event)
{
    return EventHandlerState::Accepted;
}


void Widget::draw_frame_(const Color &focused_color, const Color &unfocused_color)
{
    surface_->draw_line({0,                      0}, {area_.get_width(),                          0}, in_focus_ ? focused_color : unfocused_color);
    surface_->draw_line({area_.get_width(),      0}, {area_.get_width(),     area_.get_height() - 1}, in_focus_ ? focused_color : unfocused_color);
    surface_->draw_line({1,                      0}, {1,                     area_.get_height() - 1}, in_focus_ ? focused_color : unfocused_color);
    surface_->draw_line({0, area_.get_height() - 1}, {area_.get_width() - 1, area_.get_height() - 1}, in_focus_ ? focused_color : unfocused_color);
}
