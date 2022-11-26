#include "eventmanager.hpp"
#include "widget.hpp"


EventManager::EventManager(const Point2d &reference_point){}

EventManager::~EventManager(){}


EventHandlerState EventManager::handle_event(Event *event)
{
    assert(event != nullptr);

    switch (event->type_)
    {
        case EventType::Error:
        {
            std::cerr << "ERROR event was tried to handle" << std::endl;

            return EventHandlerState::Skipped;
        }

        case EventType::MouseButtonPressed:
        {
            std::cout << "HANDLING MOUSE_BUTTON_PRESSED" << std::endl;
            std::cout << "click_position: " << event->mbedata_.position.get_x() << " " << event->mbedata_.position.get_y() << std::endl;
            std::cout << "widgets_pool_size " << widgets_pool_.size() << std::endl;
            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                std::cout << "cur_widget_area: " << cur_widget->area_.get_x() << " " << cur_widget->area_.get_y() << " " << cur_widget->area_.get_width() << " " << cur_widget->area_.get_height() << std::endl;

                if (cur_widget->contains(event->mbedata_.position))
                {
                    std::cout << "clicked on" << cur_widget->id_ << std::endl;
                    EventHandlerState event_result = process_event_(event, cur_widget, &Widget::on_mouse_button_pressed_event);
                    if (event_result == EventHandlerState::Accepted)
                    {
                        if (focused_ != cur_widget)
                        {
                            if (focused_ != nullptr)
                            {
                                focused_->on_lost_focus_event(event);
                            }

                            cur_widget->on_gained_focus_event(event);
                            focused_ = cur_widget;
                        }

                        return event_result;
                    }
                }
            }

            break;
        }

        case EventType::MouseButtonReleased:
        {
            std::cout << "HANDLING MOUSE_BUTTON_RELEASED" << std::endl;

            if (focused_ != nullptr)
            {
                EventHandlerState event_result = process_event_(event, focused_, &Widget::on_mouse_button_released_event);
                if (event_result == EventHandlerState::Accepted)
                {
                    return event_result;
                }
            }

            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (cur_widget == focused_)
                {
                    continue;
                }

                if (cur_widget->contains(event->mbedata_.position))
                {
                    EventHandlerState event_result = process_event_(event, cur_widget, &Widget::on_mouse_button_released_event);
                    if (event_result == EventHandlerState::Accepted)
                    {
                        return event_result;
                    }
                }
            }

            break;
        }

        case EventType::MouseMoved:
        {
            std::cout << "HANDLING MOUSE_MOVED" << std::endl;
            std::cout << "event_manager identificator " << identif_ << std::endl;
            std::cout << "widget_pool_size: " << widgets_pool_.size() << std::endl;
            Widget *cur_top = nullptr;
            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (cur_widget->contains(event->mmedata_.position))
                {
                    cur_top = cur_widget;
                    
                    break;
                }
            }

            if (top_ != cur_top)
            {
                if (top_ != nullptr)
                {
                    top_->on_mouse_left_event(event);
                }
                if (cur_top != nullptr)
                {
                    cur_top->on_mouse_entered_event(event);
                }

                top_ = cur_top;
            }

            prev_mouse_position_ = event->mmedata_.position;

            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (cur_widget->contains(event->mmedata_.position))
                {
                    EventHandlerState event_result = process_event_(event, cur_widget, &Widget::on_mouse_moved_event);
                    if (event_result == EventHandlerState::Accepted)
                    {
                        return event_result;
                    }
                }
            }

            break;
        }

        case EventType::MouseWheelScrolled:
        {
            std::cout << "MOUSE_WHEEL_SCROLLED" << std::endl;

            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (!cur_widget->is_hidden())
                {
                    EventHandlerState event_result = process_event_(event, cur_widget, &Widget::on_mouse_scrolled_event);
                    // Point2d cur_widget_ref_point = cur_widget->get_reference_point_for_event();
                    // reinterpret_event_positions_(event, cur_widget_ref_point);
                    // EventHandlerState event_result = cur_widget->on_mouse_moved_event(event); //mistake?
                    // restore_event_positions_(event, cur_widget_ref_point);

                    if (event_result == EventHandlerState::Accepted)
                    {
                        return event_result;
                    }
                }
            }

            break;
        }

        case EventType::KeyPressed:
        {
            std::cout << "KEY_PRESSED" << std::endl;

            if ((focused_ != nullptr) && (focused_->on_key_pressed_event(event) == EventHandlerState::Accepted))
            {
                return EventHandlerState::Accepted;
            }

            return spread_event_(event, &Widget::on_key_pressed_event);
        }

        case EventType::KeyReleased:
        {
            std::cout << "KEY_RELEASED" << std::endl;
            if ((focused_ != nullptr) && (focused_->on_key_released_event(event) == EventHandlerState::Accepted))
            {
                return EventHandlerState::Accepted;
            }

            return spread_event_(event, &Widget::on_key_released_event);
        }

        case EventType::TimerTicked:
        {
            //std::cout << "event_manager idenf: " << identif_ << " timer_ticked" << std::endl;
            return spread_event_(event, &Widget::on_time_event, true);
        }

        case EventType::Paint:
        {
            //std::cout << "case eventtype::paint" << std::endl;
            return spread_event_(event, &Widget::on_paint_event, true);
        }

        default:
        {
            //std::cout << "did not recognise event_type " << std::endl;
            spread_event_(event, &Widget::handle_event);

            break;
        }
    }

    return EventHandlerState::Skipped;
}

bool EventManager::subscribe(Widget *subscriber)
{
    assert(subscriber != nullptr);

    widgets_pool_.push_back(subscriber);

    return true;
}

// int EventManager::register_new_event()
// {
//     if (new_events_quantity_ > MAX_EVENTS)
//     {
//         return -1;
//     }

//     ++new_events_quantity_;

//     return new_events_quantity_;
// }


void EventManager::set_focused(Widget *widget)
{
    assert(widget != nullptr);

    focused_ = widget;
}



void EventManager::reinterpret_event_positions_(Event *event, const Point2d &point)                     // copypaste?
{
    switch (event->type_)
    {
        case EventType::MouseButtonPressed:
        case EventType::MouseButtonReleased:
        {
            event->mbedata_.position -= point - reference_point_;

            break;
        }
        case EventType::MouseMoved:
        {   
            event->mmedata_.position -= point - reference_point_;

            break;
        }
        case EventType::MouseWheelScrolled:
        {
            event->mwedata_.position -= point - reference_point_;
            
            break;
        }

        default:
        {
            break;
        }
    }
}

void EventManager::restore_event_positions_(Event *event, const Point2d &point)                         // copypaste?
{
    switch (event->type_)
    {
        case EventType::MouseButtonPressed:
        case EventType::MouseButtonReleased:
        {
            event->mbedata_.position += point - reference_point_;

            break;
        }
        case EventType::MouseMoved:
        {   
            event->mmedata_.position += point - reference_point_;

            break;
        }
        case EventType::MouseWheelScrolled:
        {
            event->mwedata_.position += point - reference_point_;
            
            break;
        }

        default:
        {
            break;
        }
    }
}

EventHandlerState EventManager::spread_event_(const Event *event, 
                                              EventHandlerState (Widget::*function)(const Event *), 
                                              bool unconditional) const
{
    assert(event    != nullptr);
    assert(function != nullptr);

    //std::cout << "called spread_event_" << std::endl;

    for (size_t i = 0; i < widgets_pool_.size(); ++i)
    {
        Widget* cur_widget = widgets_pool_[i];
        //std::cout << "be4 call " << __LINE__  << " " << __FILE__ << std::endl;
        //printf("pointer to spread_event_function: %\p\n", function);
        EventHandlerState event_result = (cur_widget->*function)(event);
        //std::cout << "a4ter call " << __LINE__  << " " << __FILE__ << std::endl;
        if (!unconditional && event_result == EventHandlerState::Accepted)
        {
            return event_result;
        }
    }

    return EventHandlerState::Accepted;
}


EventHandlerState EventManager::process_event_(Event  *event,
                                               Widget *cur_widget,
                                               EventHandlerState (Widget::*function)(const Event *))
{
    assert(event      != nullptr);
    assert(cur_widget != nullptr);
    assert(function   != nullptr);

    Point2d cur_widget_ref_point = cur_widget->get_reference_point_for_event();
    reinterpret_event_positions_(event, cur_widget_ref_point);
    EventHandlerState event_result = (cur_widget->*function)(event);
    restore_event_positions_(event, cur_widget_ref_point);

    return event_result;
}
