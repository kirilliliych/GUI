#ifndef BUTTON_HPP
#define BUTTON_HPP


#include "color.hpp"
#include "colorlibrary.hpp"
#include "event.hpp"
#include "text.hpp"
#include "widget.hpp"


static const Color *DEFAULT_BUTTON_COLOR          = &LIGHT_GREY;
static const Color *DEFAULT_BUTTON_HOVERING_COLOR = &GREY;


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
        named_(true)
    {
        text_.make_ignored_by_events();

        id_ = 2;
        std::cout << "id: " << id_ << std::endl;
        //init_surface();
    }

    Button(const Rectangle &rectangle, Widget *parent = nullptr)
      : Button(rectangle, "", parent)
    {
        text_.make_ignored_by_events();

        id_ = 2;
        std::cout << "id: " << id_ << std::endl;
        //init_surface();
    }

    virtual ~Button(){}


    Button(const Button &button)             = delete;
    Button &operator =(const Button &button) = delete;

//-------------------------Functions--------------------------------

    void set_text_color(const Color &color)
    {
        text_.set_color(color);

        requires_repaint_ = true;
    }

    void set_text(const char *text)
    {
        text_.set_string(text);

        requires_repaint_ = true;
    }

    void set_character_size(unsigned int size)
    {
        text_.set_character_size(size);

        requires_repaint_ = true;
    }

    
    EventHandlerState on_mouse_button_pressed_event(const Event *event) override
    {
        assert(event != nullptr);

        if (event->mbedata_.button == MouseButton::Left)
        {
            pressed_          = true;
            pressed.emit();
            requires_repaint_ = true;

            return EventHandlerState::Accepted;
        }

        return EventHandlerState::Skipped;
    }

    EventHandlerState on_mouse_button_released_event(const Event *event) override
    {
        assert(event != nullptr);

        if (pressed_ && event->mbedata_.button == MouseButton::Left)
        {
            released.emit();
            if (Rectangle{0, 0, area_.get_width(), area_.get_height()}.contains(event->mbedata_.position))
            {
                clicked.emit();
                //std::cout << "after clickedemit" << std::endl;
            }

            pressed_          = false;
            requires_repaint_ = true;

            return EventHandlerState::Accepted;
        }

        return EventHandlerState::Skipped;
    }

    EventHandlerState on_mouse_entered_event(const Event *event) override
    {
        assert(event != nullptr);

        hovered_          = true;
        requires_repaint_ = true;

        return EventHandlerState::Accepted;
    }

    EventHandlerState on_mouse_left_event(const Event *event) override
    {
        assert(event != nullptr);

        hovered_          = false;
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

        hovered_          = false;
        if (pressed_)
        {
            released.emit();
        }
        pressed_          = false;
        requires_repaint_ = true;
        
        Widget::on_lost_focus_event(event);

        return EventHandlerState::Accepted;
    }

//-----------------------------------------------------------------
protected:

    // void output_certain_widget_form_(Color color = DEFAULT_BUTTON_COLOR) override
    // {   
    //     std::cout << "called button output_certain_widget_form" << std::endl; 
    //     surface_->clear(0);
    //     surface_->draw_rectangle(area_, color);
    //     surface_->update();
    // }
//-----------------------Variables------------------------------

    int text_vertical_offset_   = 3;

    Text text_;
    bool pressed_  = false;
    bool hovered_  = false;
    bool named_    = false;

public:

    Signal<> clicked;
    Signal<> pressed;
    Signal<> released;
    
//--------------------------------------------------------------
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
    void set_color(Color color)
    {
        color_ = color;
        //update_surface();
        requires_repaint_ = true;
    }

    EventHandlerState on_paint_event(const Event *event) override
    {
        //std::cout << "button: called on_paint_event" << std::endl;
        assert(event != nullptr);

        if (!requires_repaint_)
        {
            return EventHandlerState::Accepted;
        }

        Color *to_paint = hovered_ ? &hovering_color_ : &color_;
        surface_->draw_rectangle(area_, *to_paint);
        
        draw_frame_();
    
        // sprite_.load_from_surface(surface_);
        // sprite_.set_position(area_.get_x(), area_.get_y());

        requires_repaint_ = false;

        return EventHandlerState::Accepted;
    }
//---------------------------------------------------------------------------------------
private:
public:
//------------------------Variables------------------------------------------------------

    Color color_{*DEFAULT_BUTTON_COLOR};
    Color hovering_color_{*DEFAULT_BUTTON_HOVERING_COLOR};
};
//---------------------------------------------------------------------------------------

#endif
