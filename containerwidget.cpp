#include "containerwidget.hpp"


ContainerWidget::ContainerWidget(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent),
    event_manager_(rectangle.get_top_left_corner_coords())
{
    std::cout << "init container_widget" << std::endl;
    parent_manager_    = event_manager_ptr_;
    event_manager_ptr_ = &event_manager_;
    //Widget::set_event_manager(&event_manager_);
    //event_manager_.subscribe(this);
}

ContainerWidget::~ContainerWidget()
{}


void ContainerWidget::set_event_manager(EventManager *event_manager)
{
    if (parent_manager_ != nullptr)
    {
        return;
    }

    parent_manager_ = event_manager;
    if (event_manager != nullptr)
    {
        event_manager->subscribe(this);
    }
}

EventHandlerState ContainerWidget::handle_event                  (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}  
EventHandlerState ContainerWidget::on_mouse_button_pressed_event (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_mouse_moved_event          (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_mouse_scrolled_event       (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_mouse_entered_event        (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_mouse_left_event           (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_key_pressed_event          (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_key_released_event         (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_gained_focus_event         (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_lost_focus_event           (const Event *event)
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_time_event                 (const Event *event)
{
    assert(event != nullptr);
    //std::cout << "containerwidget on_time_event called " << std::endl;

    update_on_time();
    //std::cout << "after update_on_time" << std::endl;

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_paint_event                (const Event *event)           // begin called 100500 times because widget::on_paint_event is overwritten
{                                                                                               // otherwise gets overwritten by window::on_paint_event
    assert(event != nullptr);

    // if (id_ != 1)
    // std::cout << "containerwidget: on_paint_event" << std::endl;

    return event_manager_.handle_event(const_cast<Event *> (event));
}

