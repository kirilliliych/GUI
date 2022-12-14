#include "containerwidget.hpp"


ContainerWidget::ContainerWidget(const Rectangle &rectangle, ContainerWidget *parent)
  : Widget(rectangle, parent),
    event_manager_(rectangle.get_top_left_corner_coords())
{
    //std::cout << "ContainerWidget constructor" << std::endl;

    parent_manager_    = event_manager_ptr_;
    event_manager_ptr_ = &event_manager_;
    // //Widget::set_event_manager(&event_manager_);
    // //event_manager_.subscribe(this);

    // if (parent != nullptr)
    // {
    //     //std::cout << "container widget connnected to parent parent" << std::endl; 
    //     parent_manager_ = parent->parent_manager_;
    //     parent->parent_manager_->subscribe(this);
    // }
    // else
    // {
    //     parent_manager_ = event_manager_ptr_;
    // }
    // event_manager_ptr_ = &event_manager_;

    //std::cout << "End of ContainerWidget constructor" << std::endl;
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

EventManager *ContainerWidget::get_parent_event_manager()
{
    return parent_manager_;
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

    update_on_time();

    return event_manager_.handle_event(const_cast<Event *> (event));
}
EventHandlerState ContainerWidget::on_paint_event                (const Event *event)           
{
    assert(event != nullptr);

    return event_manager_.handle_event(const_cast<Event *> (event));
}

