#ifndef WIDGET_HPP
#define WIDGET_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "eventmanager.hpp"
#include "rectangle.hpp"
#include "signal.hpp"
#include "sprite.hpp"
#include "surface.hpp"


static const char *NO_TEXTURE_REQUIRED  = reinterpret_cast<const char *> (0xDEADDEAD);
 

static const Color *FOCUSED_WIDGET_FRAME_COLOR   = &ORANGE; 
static const Color *UNFOCUSED_WIDGET_FRAME_COLOR = &LIGHT_GREY;


class Widget : public SignalObject
{

public:
//-------------------Ctors, dtors...--------------------

    Widget(Widget *parent = nullptr);

    Widget(const Rectangle &rectangle, Widget *parent = nullptr);

    // Widget(const Rectangle &rectangle, Surface *surface, Widget *parent = nullptr)
    //   : Widget(rectangle, parent)
    // {
    //     delete surface_;

    //     assert(surface != nullptr);
    //     surface_ = surface;
    // }

    Widget(const Rectangle &rectangle, Surface *surface, Widget *parent = nullptr);

    Widget(const Widget &widget)             = delete;
    Widget &operator =(const Widget &widget) = delete;

    virtual ~Widget();

//----------------Functions-----------------------------

    virtual void set_event_manager(EventManager *event_manager);

    void add_child(Widget *child);

    void resize(int width, int height);

    void render(Surface *surface);

    // void init_surface  (const char *texture_file_name = NO_TEXTURE_REQUIRED);
    // void update_surface();

    const Rectangle &get_area() const;


    const Point2d get_reference_point_for_event() const;
    virtual bool contains(const Point2d &point);

    void show();
    void hide();
    bool is_hidden();
    void make_ignored_by_events();
    bool is_ignored_by_events() const;


    virtual void update_on_time(){std::cout << "called empty update_on_time" << std::endl;}

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


    bool requires_repaint_  = true;

protected:
public:

    virtual void draw_frame_(const Color focused_color = MAGENTA, const Color unfocused_color = LIGHT_GREY);
    // virtual void output_certain_widget_form_(Color color = Color{0x000000FF});

//-------------------Variables----------------------------------------
    bool in_focus_          = false;
    bool hidden_            = false;
    bool ignored_by_events_ = false;

    EventManager *event_manager_ptr_ = nullptr;
    Rectangle area_{};
    Surface *surface_ = nullptr;
    Sprite sprite_{};

    std::chrono::system_clock::time_point timer_{};

    Widget *parent_   = nullptr;
    std::vector<Widget *> children_{};

public:
    int id_ = 0;                            // convenient for debug
};

#endif
