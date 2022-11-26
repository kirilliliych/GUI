#ifndef MENUBAR_HPP
#define MENUBAR_HPP


#include "widget.hpp"


const Color DEFAULT_MENUBAR_COLOR = Color{LIGHT_GREY};


class Menubar : public Widget
{
public:
//------------------------------------------------------------''

    Menubar(const Rectangle &rectangle, const Color &color, Widget *parent = nullptr);
//------------------------------------------------------------

    EventHandlerState on_paint_event(const Event *event) override;
    
//------------------------------------------------------------
private:

    //void output_certain_widget_form_(Color color = DEFAULT_MENUBAR_COLOR) override;

//-----------------------Variables-----------------------

    Color color_{};
//-------------------------------------------------------
};

#endif