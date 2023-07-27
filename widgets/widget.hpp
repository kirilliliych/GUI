#ifndef WIDGET_HPP
#define WIDGET_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../events/eventmanager.hpp"
#include "../sfmlwrap/rectangle.hpp"
#include "../events/signals/signal.hpp"
#include "../sfmlwrap/sprite.hpp"
#include "../sfmlwrap/surface.hpp"


static const Color *DEFAULT_WIDGET_COLOR                 = &LIGHT_GREY;
static const Color *FOCUSED_WIDGET_FRAME_COLOR           = &ORANGE; 
static const Color *UNFOCUSED_WIDGET_FRAME_COLOR         = &LIGHT_GREY;
static const Color FOCUSED_WIDGET_TEXTURE_HOVERED_COLOR  = Color{0x00000060};


class Widget : public SignalObject
{

public:
//-------------------Ctors, dtors...--------------------

    Widget(Widget *parent = nullptr);
    Widget(const Rectangle &rectangle, Widget *parent = nullptr);

    Widget(const Widget &widget)             = delete;
    Widget &operator =(const Widget &widget) = delete;

    virtual ~Widget();

//----------------Functions-----------------------------

    virtual void set_event_manager(EventManager *event_manager);

    void add_child(Widget *child);

    virtual void render(Surface *surface);

    const Rectangle &get_area() const;
    void move(const Point2d &offset);

    const Point2d get_reference_point_for_event() const;
    virtual bool contains(const Point2d &position);

    void show();
    void hide();
    bool is_hidden();
    void make_ignored_by_events();
    bool is_ignored_by_events() const;

    virtual const char *get_texture_name() const;

    virtual void update_on_time(){}

    virtual EventHandlerState handle_event                  (const Event *event);
    virtual EventHandlerState on_mouse_button_pressed_event (const Event *event);
    virtual EventHandlerState on_mouse_button_released_event(const Event *event);
    virtual EventHandlerState on_mouse_moved_event          (const Event *event);
    virtual EventHandlerState on_mouse_scrolled_event       (const Event *event);
    virtual EventHandlerState on_mouse_entered_event        (const Event *event);
    virtual EventHandlerState on_mouse_left_event           (const Event *event);
    virtual EventHandlerState on_key_pressed_event          (const Event *event);
    virtual EventHandlerState on_key_released_event         (const Event *event);
    virtual EventHandlerState on_gained_focus_event         (const Event *event);
    virtual EventHandlerState on_lost_focus_event           (const Event *event);
    virtual EventHandlerState on_time_event                 (const Event *event);
    virtual EventHandlerState on_paint_event                (const Event *event);


protected:
//public:

    virtual void draw_frame_(const Color &focused_color = *FOCUSED_WIDGET_FRAME_COLOR, const Color &unfocused_color = *UNFOCUSED_WIDGET_FRAME_COLOR);

//-----------------------------------Variables-------------------------------------
    bool in_focus_          = false;
    bool hidden_            = false;
    bool ignored_by_events_ = false;
    bool has_texture_       = false;

    EventManager *event_manager_ptr_ = nullptr;
    Rectangle area_{};
    Surface *surface_ = nullptr;
    Texture *skin_    = nullptr;

    std::chrono::system_clock::time_point timer_{};

    Widget *parent_   = nullptr;
    std::vector<Widget *> children_{};

public:
    int id_ = 0;                            // for debug
};

#endif
