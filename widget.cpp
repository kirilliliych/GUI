#include "widget.hpp"


Widget::Widget(Widget *parent)
  : Widget(Rectangle{100, 100, 200, 200}, parent)
{}

Widget::Widget(const Rectangle &rectangle, Widget *parent)
  : area_(rectangle),
    surface_(new Surface(rectangle.get_width(), rectangle.get_height())),
    parent_(parent)        
{
    std::cout << "init widget" << std::endl;
    if (parent != nullptr)
    {
        parent->add_child(this);
    }
    else
    {
        std::cout << "widget parent nullptr" << std::endl;
    }
}


Widget::Widget(const Rectangle &rectangle, Surface *surface, Widget *parent)          // copypaste?
  : area_(rectangle),
    surface_(surface),
    parent_(parent)
{
    if (parent != nullptr)
    {
        parent->add_child(this);
    }
}

Widget::~Widget()
{
    delete surface_;
}



void Widget::set_event_manager(EventManager *event_manager)
{
    if ((event_manager == nullptr) || (event_manager_ptr_ != nullptr))
    {
        std::cout << id_ << " was not connected to event_manager because ";
        if (event_manager == nullptr)
        {
            std::cout << "given is nullptr" << std::endl;
        }
        else if (event_manager_ptr_ != nullptr)
        {
            std::cout << "already connected" << std::endl;
        }

        return;
    }

    event_manager_ptr_ = event_manager;
    event_manager_ptr_->subscribe(this);
}

void Widget::add_child(Widget *child)
{
    assert(child != nullptr);
    assert(child != this);

    children_.push_back(child);
    child->set_event_manager(event_manager_ptr_);
    std::cout << "add child to event manager with identif " << event_manager_ptr_->identif_ << std::endl;
}

void Widget::resize(int width, int height)                                    // not tested
{
    area_.set_width(width);
    area_.set_height(height);

    sprite_.set_texture_rect(Rectangle{0, 0, width, height});
}


void Widget::render(Surface *surface)
{
    if (surface_ != nullptr)
    {
        //update_surface();
        //std::cout << "drawing sprite of id " << id_ << std::endl; 
        //if (id_ != 1)

        sprite_.load_from_surface(surface_);
        sprite_.set_position(area_.get_x(), area_.get_y());
        surface->draw_sprite(sprite_);
        
        // if (id_ == 1)
        // {
        //     sf::FloatRect fr = sprite_.sprite_.getLocalBounds();
        //     std::cout << "left: " << fr.left << " top: " << fr.top << " width: " << fr.width << " height: " << fr.height << std::endl;
        //     sf::Vector2f v = sprite_.sprite_.getPosition();
        //     sf::Vector2f v1= sprite_.sprite_.getOrigin();
        //     //std::cout << "position: " << v.x << " " << v.y << std::endl << "origin: " << v1.x << " " << v1.y << std::endl;
        // }
        // if (id_ == 1)
        // {
        //     std::cout << "children size: " << children_.size() << std::endl;
        // }
        //std::cout << "id: " << id_ << " children size: " << children_.size() << std::endl; 
        for (int child_index = 0; child_index < children_.size(); ++child_index)
        {
            children_.at(child_index)->render(surface);
        }
    }
    else
    {
        std::cout << "Given surface is unitialized " << __LINE__ << " " << __FILE__ << std::endl;
    }
}


// void Widget::init_surface(const char *texture_file_name)
// {
//     assert(texture_file_name != nullptr);

//     if (texture_file_name == NO_TEXTURE_REQUIRED)
//     {
//         update_surface();
//     }
//     else
//     {
//         // load texture...
//     }
// }

// void Widget::update_surface() //deprecated
// {
//     surface_->surface_.clear(sf::Color::Transparent);
//     output_certain_widget_form_();
//     surface_->update();
    
//     sprite_.load_from_surface(surface_);
//     sprite_.set_position(static_cast<float> (area_.get_x()), static_cast<float> (area_.get_y()));
// }


const Rectangle &Widget::get_area() const
{
    return area_;
}
const Point2d Widget::get_reference_point_for_event() const
{
    return area_.get_top_left_corner_coords();
}


bool Widget::contains(const Point2d &point)
{
    return hidden_ ? false : ignored_by_events_ ? false : area_.contains(point);
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


void Widget::draw_frame_(const Color focused_color, const Color unfocused_color)
{
    surface_->draw_line({0,                      0}, {area_.get_width() - 1,                      0}, in_focus_ ? *FOCUSED_WIDGET_FRAME_COLOR : *UNFOCUSED_WIDGET_FRAME_COLOR);
    surface_->draw_line({area_.get_width() - 1,  0}, {area_.get_width() - 1, area_.get_height() - 1}, in_focus_ ? *FOCUSED_WIDGET_FRAME_COLOR : *UNFOCUSED_WIDGET_FRAME_COLOR);
    surface_->draw_line({1,                      0}, {0,                     area_.get_height() - 1}, in_focus_ ? *FOCUSED_WIDGET_FRAME_COLOR : *UNFOCUSED_WIDGET_FRAME_COLOR);
    surface_->draw_line({0, area_.get_height() - 1}, {area_.get_width() - 1, area_.get_height() - 1}, in_focus_ ? *FOCUSED_WIDGET_FRAME_COLOR : *UNFOCUSED_WIDGET_FRAME_COLOR);
}
