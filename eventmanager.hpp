#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP


#include <vector>
#include "event.hpp"


const int MIN_EVENTS = 500;
const int MAX_EVENTS = 1000;


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

    // int register_new_event();

    void set_focused(Widget *widget);

    int identif_ = 1488;
//-----------------------------------------------------------------------
private:

    EventHandlerState spread_event_(const Event *event, 
                                    EventHandlerState (Widget::*function)(const Event *), 
                                    bool unconditional = false) const;

    EventHandlerState process_event_(Event  *event,
                                     Widget *cur_widget,
                                     EventHandlerState (Widget::*function)(const Event *));

    void reinterpret_event_positions_(Event *event, const Point2d &point);
    void restore_event_positions_    (Event *event, const Point2d &point);

//-------------------------------Variables--------------------------------
    std::vector<Widget *> widgets_pool_{};
    Widget *focused_ = nullptr;
    Widget *top_     = nullptr;

    Point2d prev_mouse_position_ = {-1, -1};
    Point2d reference_point_{0, 0};
//------------------------------------------------------------------------
    //int new_events_quantity_ = MIN_EVENTS;
};


#endif