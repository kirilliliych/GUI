#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP


#include "widget.hpp"


enum ScrollbarOrientation
{
    Vertical,
    Horizontal
};


class Scrollbar : public Widget
{
public:
//--------------------------------------------------------------------------------------------------------------------------
    Scrollbar(const Rectangle &rectangle, ScrollbarOrientation orientation, int carriage_size = 33, Widget *parent = nullptr);
    Scrollbar(int x, int y, int w = 200, int h = 20, ScrollbarOrientation orientation = ScrollbarOrientation::Horizontal,
              int carriage_size = 33, Widget *parent = nullptr);
    
    ~Scrollbar();
//--------------------------------------------------------------------------------------------------------------------------
    void set_min_value(const long long value);
    void set_max_value(const long long value);
    void set_cur_value(const long long value);

    EventHandlerState on_mouse_button_pressed_event (const Event *event) override;
    EventHandlerState on_mouse_button_released_event(const Event *event) override;
    EventHandlerState on_mouse_moved_event          (const Event *event) override;
    EventHandlerState on_paint_event                (const Event *event) override;

private:

    void update_carriage_();
//-----------------------------------------------------Variables------------------------------------------------------------
    bool requires_repaint_ = true;
    bool captured_         = false;

    long long min_value_ = 0;
    long long max_value_ = 100;
    long long cur_value_ = 0;

    ScrollbarOrientation orientation_ = Vertical;

    Rectangle carriage_ = {0, 0, 10, 10};

public:
    bool ab = false;
    Signal<int> value_changed;
};



#endif
