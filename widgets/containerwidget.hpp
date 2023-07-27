#ifndef CONTAINERWIDGET_HPP
#define CONTAINERWIDGET_HPP


#include "widget.hpp"


class ContainerWidget : public Widget
{
public:
//---------------------------------------------------------------------------------------
    ContainerWidget(const Rectangle &rectangle, ContainerWidget *parent = nullptr);

    virtual ~ContainerWidget();

//---------------------------------------------------------------------------------------
    virtual void set_event_manager(EventManager *event_manager) override;

    EventManager *get_parent_event_manager();

    virtual EventHandlerState handle_event                  (const Event *event) override;
    virtual EventHandlerState on_mouse_button_pressed_event (const Event *event) override;
    virtual EventHandlerState on_mouse_button_released_event(const Event *event) override;
    virtual EventHandlerState on_mouse_moved_event          (const Event *event) override;
    virtual EventHandlerState on_mouse_scrolled_event       (const Event *event) override;
    virtual EventHandlerState on_mouse_entered_event        (const Event *event) override;
    virtual EventHandlerState on_mouse_left_event           (const Event *event) override;
    virtual EventHandlerState on_key_pressed_event          (const Event *event) override;
    virtual EventHandlerState on_key_released_event         (const Event *event) override;
    virtual EventHandlerState on_gained_focus_event         (const Event *event) override;
    virtual EventHandlerState on_lost_focus_event           (const Event *event) override;
    virtual EventHandlerState on_time_event                 (const Event *event) override;
    virtual EventHandlerState on_paint_event                (const Event *event) override;

protected:
//-----------------------------------Variables-------------------------------------

    EventManager event_manager_;
    EventManager *parent_manager_ = nullptr;
};


#endif