#include "eventmanager.hpp"
#include "widget.hpp"


EventManager::EventManager(const Point2d &relative_point)
  : relative_point_(relative_point)
{}

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
            //std::cout << "HANDLING MOUSE_BUTTON_PRESSED" << std::endl;
            //std::cout << "identif: " << identif_ << std::endl;
            //std::cout << "click_position: " << event->mbedata_.position.x << " " << event->mbedata_.position.y << std::endl;
            //std::cout << "widgets_pool_size " << widgets_pool_.size() << std::endl;
            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (cur_widget->contains(event->mbedata_.position))
                {
                    // std::cout << "clicked on" << cur_widget->id_ << std::endl;
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
            //std::cout << "HANDLING MOUSE_BUTTON_RELEASED" << std::endl;

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

                //Rectangle cur_widget_area = cur_widget->get_area();
                // cur_widget_area.set_x(0);
                // cur_widget_area.set_y(0);
                // Rectangle cur_widget_area = cur_widget->get_area();
                // cur_widget_area.set_x(cur_widget_area.get_x() - reference_point_.x);
                // cur_widget_area.set_y(cur_widget_area.get_y() - reference_point_.y);
                // if (cur_widget_area.contains(event->mbedata_.position) && (!cur_widget->is_hidden()) && (!cur_widget->is_ignored_by_events()))
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
            // std::cout << "HANDLING MOUSE_MOVED" << std::endl;
            //std::cout << "event_manager identificator " << identif_ << std::endl;
            //std::cout << "widget_pool_size: " << widgets_pool_.size() << std::endl;
            Widget *cur_top = nullptr;
            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                // Rectangle cur_widget_area = cur_widget->get_area();
                // cur_widget_area.set_x(0);
                // cur_widget_area.set_y(0);
                
                // std::cout << "cur_widget_area: x" << cur_widget_area.get_x() << " y " << cur_widget_area.get_y() << " w " << cur_widget_area.get_width() << " h " << cur_widget_area.get_height() << std::endl;
                // std::cout << "mmedatapos: x " << event->mmedata_.position.x << " y " << event->mmedata_.position.y << std::endl;
                // Rectangle cur_widget_area = cur_widget->get_area();
                // cur_widget_area.set_x(cur_widget_area.get_x() - reference_point_.x);
                // cur_widget_area.set_y(cur_widget_area.get_y() - reference_point_.y);
                // if (cur_widget_area.contains(event->mbedata_.position) && (!cur_widget->is_hidden()) && (!cur_widget->is_ignored_by_events()))
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
            for (int i = widgets_pool_.size() - 1; i >= 0; --i)
            {
                Widget *cur_widget = widgets_pool_[i];
                if (!cur_widget->is_hidden())
                {
                    EventHandlerState event_result = process_event_(event, cur_widget, &Widget::on_mouse_scrolled_event);

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
            if ((focused_ != nullptr) && (focused_->on_key_pressed_event(event) == EventHandlerState::Accepted))
            {
                return EventHandlerState::Accepted;
            }

            return spread_event_(event, &Widget::on_key_pressed_event);
        }

        case EventType::KeyReleased:
        {
            if ((focused_ != nullptr) && (focused_->on_key_released_event(event) == EventHandlerState::Accepted))
            {
                // if (focused_->id_ == 25)
                // {
                //     std::cout << "ALARM KEY-RELEASED ID 25\n";
                // }
                return EventHandlerState::Accepted;
            }

            return spread_event_(event, &Widget::on_key_released_event);
        }

        case EventType::TimerTicked:
        {
            return spread_event_(event, &Widget::on_time_event, true);
        }

        case EventType::Paint:
        {
            return spread_event_(event, &Widget::on_paint_event, true);
        }

        default:
        {
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


void EventManager::set_focused(Widget *widget)
{
    assert(widget != nullptr);

    focused_ = widget;
}


EventHandlerState EventManager::spread_event_(const Event *event, 
                                              EventHandlerState (Widget::*function)(const Event *), 
                                              bool unconditional) const
{
    assert(event    != nullptr);
    assert(function != nullptr);

    for (size_t i = 0; i < widgets_pool_.size(); ++i)
    {
        Widget* cur_widget = widgets_pool_[i];
        // if ((cur_widget->id_ == 25) && (event->type_ == EventType::KeyReleased))
        // {
        //     std::cout << "ALARM SPREAD EVENT ID IS 25 AND KEY_RELEASED\n";
        // }
       // std::cout << "spread_event_: cur_widget id: " << cur_widget->id_ << std::endl;
        //std::cout << "spread_event_:" << __LINE__  << " " << __FILE__ << std::endl;
        //printf("pointer to spread_event_function: %\p\n", function);
        EventHandlerState event_result = (cur_widget->*function)(event);
        //std::cout << "spread_event_: a4ter call " << __LINE__  << " " << __FILE__ << std::endl;
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

    EventHandlerState event_result = (cur_widget->*function)(event);

    return event_result;
}
