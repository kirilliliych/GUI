#ifndef BUTTON_HPP
#define BUTTON_HPP


#include "../utilities/color.hpp"
#include "../utilities/colorlibrary.hpp"
#include "../events/event.hpp"
#include "../sfmlwrap/text.hpp"
#include "widget.hpp"


static const Color *DEFAULT_BUTTON_HOVERING_COLOR = &GREY;
static const Color *DEFAULT_BUTTON_PRESSED_COLOR  = &ORANGE;


class Button : public Widget
{
public:
//-------------------Ctor, dtor...----------------------------------

    Button(const Rectangle &rectangle, const char *text, Widget *parent = nullptr)
      : Widget(rectangle, parent),
        text_(Text(text, Rectangle{rectangle.get_x(),
                                   rectangle.get_y() + text_vertical_offset_,
                                   rectangle.get_width(),
                                   rectangle.get_height() - 2 * text_vertical_offset_
                                  }, this)),
        is_named_(true)
    {
        text_.make_ignored_by_events();

        id_ = 2;
    }

    Button(const Rectangle &rectangle, Widget *parent = nullptr)
      : Button(rectangle, "", parent)
    {
        text_.make_ignored_by_events();

        id_ = 2;
    }

    virtual ~Button() override
    {}


    Button(const Button &button)             = delete;
    Button &operator =(const Button &button) = delete;

//-------------------------Functions--------------------------------

    void set_text_color(const Color &color)
    {
        text_.set_color(color);

        text_.requires_repaint_ = true;
    }

    void set_text(const char *text)
    {
        text_.set_string(text);

        text_.requires_repaint_ = true;
    }

    void set_character_size(unsigned int size)
    {
        text_.set_character_size(size);

        text_.requires_repaint_ = true;
    }

    void set_font(const char *font_name = DEFAULT_FONT_NAME)
    {
        assert(font_name != nullptr);

        text_.set_font(font_name);

        text_.requires_repaint_ = true;
    }

    void center_text()
    {
        if (is_named_)
        {
            text_.center_text();
        }
    }
    
    EventHandlerState on_mouse_button_pressed_event(const Event *event) override
    {
        assert(event != nullptr);
        
        if (event->mbedata_.button == MouseButton::Left)
        {
            is_pressed_       = true;
            pressed.emit();
            requires_repaint_ = true;

            return EventHandlerState::Accepted;
        }

        return EventHandlerState::Skipped;
    }

    EventHandlerState on_mouse_button_released_event(const Event *event) override
    {
        assert(event != nullptr);

        if (is_pressed_ && event->mbedata_.button == MouseButton::Left)
        {
            released.emit();
            if (area_.contains(event->mbedata_.position))
            {
                clicked.emit();
            }

            is_pressed_       = false;
            requires_repaint_ = true;

            return EventHandlerState::Accepted;
        }

        return EventHandlerState::Skipped;
    }

    EventHandlerState on_mouse_entered_event(const Event *event) override
    {
        assert(event != nullptr);
        is_hovered_       = true;
        if (is_named_)
        {
            text_.set_hovered(true);
        }
        requires_repaint_ = true;

        mouse_entered.emit();   // meme

        return EventHandlerState::Accepted;
    }

    EventHandlerState on_mouse_left_event(const Event *event) override
    {
        assert(event != nullptr);

        is_hovered_       = false;
        if (is_named_)
        {
            text_.set_hovered(false);
        }
        requires_repaint_ = true;

        return EventHandlerState::Accepted;
    }

    EventHandlerState on_gained_focus_event(const Event *event) override
    {
        assert(event != nullptr);

        Widget::on_gained_focus_event(event);

        requires_repaint_ = true;

        return EventHandlerState::Accepted;
    }

    EventHandlerState on_lost_focus_event(const Event *event) override
    {
        assert(event != nullptr);

        is_hovered_       = false;
        if (is_named_)
        {
            text_.set_hovered(false);
        }
        if (is_pressed_)
        {
            released.emit();
        }
        is_pressed_       = false;
        requires_repaint_ = true;
        
        Widget::on_lost_focus_event(event);

        return EventHandlerState::Accepted;
    }

//---------------------------------------------------------------------------------------
protected:
//public:
//-----------------------Variables-------------------------------------------------------

    int text_vertical_offset_   = 3;

    Text text_;
    bool is_pressed_  = false;
    bool is_hovered_  = false;
    bool is_named_    = false;
    bool requires_repaint_ = true;

public:

    Signal<> clicked;
    Signal<> pressed;
    Signal<> released;

    Signal<> mouse_entered;     // just for meme
//---------------------------------------------------------------------------------------
};


class ActionButton : public Button
{
public:
//---------------------------------------------------------------------------------------
    ActionButton(const Rectangle &rectangle, const char *text, 
                 const Color &button_color, const Color &hovering_color, const Color &text_color,
                 Widget *parent = nullptr)
      : Button(rectangle, text, parent),
        color_         (button_color),
        hovering_color_(hovering_color)
    {
        set_color(button_color);
        set_text_color(text_color);
    }

    ActionButton(const Rectangle &rectangle, 
                 const Color &button_color, const Color &hovering_color,
                 Widget *parent = nullptr)
      : Button(rectangle, parent),
        color_         (button_color),
        hovering_color_(hovering_color)
    {
        set_color(button_color);
    }


    virtual ~ActionButton(){}


    ActionButton(const Button &button)             = delete;
    ActionButton &operator =(const Button &button) = delete;
//---------------------------------------------------------------------------------------
    void set_color(const Color &color)
    {
        color_ = color;
        requires_repaint_ = true;
    }

    EventHandlerState on_paint_event(const Event *event) override
    {
        assert(event != nullptr);

        if (!requires_repaint_)
        {
            return EventHandlerState::Accepted;
        }

        const Color *to_paint = is_pressed_ ? DEFAULT_BUTTON_PRESSED_COLOR : is_hovered_ ? DEFAULT_BUTTON_HOVERING_COLOR : &color_;
        surface_->clear(0);
        surface_->draw_rectangle(area_, *to_paint);
        
        draw_frame_();

        requires_repaint_ = false;

        return EventHandlerState::Accepted;
    }
//---------------------------------------------------------------------------------------
private:
//public:
//-----------------------------------Variables-------------------------------------------
    Color color_{*DEFAULT_WIDGET_COLOR};
    Color hovering_color_{*DEFAULT_BUTTON_HOVERING_COLOR};
//---------------------------------------------------------------------------------------
};

#endif
