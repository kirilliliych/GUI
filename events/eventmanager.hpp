#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP


#include <vector>
#include "event.hpp"



class Widget;

class EventManager
{
public:
//-----------------------------------------------------------------------
    EventManager(const Point2d &reference_point = {0, 0});

    ~EventManager();


    EventManager(const EventManager &event_manager)            = delete;
    EventManager &operator=(const EventManager &event_manager) = delete;
//-----------------------------------------------------------------------

    EventHandlerState handle_event(Event *event);

    bool subscribe(Widget *subscriber);

    void set_focused(Widget *widget);
//-----------------------------------------------------------------------
private:

    EventHandlerState spread_event_(const Event *event, 
                                    EventHandlerState (Widget::*function)(const Event *), 
                                    bool unconditional = false) const;

    EventHandlerState process_event_(Event  *event,
                                     Widget *cur_widget,
                                     EventHandlerState (Widget::*function)(const Event *));

//-------------------------------Variables--------------------------------
    std::vector<Widget *> widgets_pool_{};
    Widget *focused_ = nullptr;
    Widget *top_     = nullptr;

    Point2d prev_mouse_position_{-1, -1};
    Point2d relative_point_{0, 0};
public:
    int identif = 0;
//------------------------------------------------------------------------
};

#endif
