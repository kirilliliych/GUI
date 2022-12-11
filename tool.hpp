#ifndef TOOL_HPP
#define TOOL_HPP


#include "image.hpp"
#include "widget.hpp"


struct ToolAction
{
    Image *image;
    Point2d point;
    bool shift;
    bool alt;
    bool ctrl;
};


class Tool
{
public:
//-----------------------------------------------------
    Tool()
    {}

    virtual ~Tool()
    {}
//-----------------------------------------------------

    virtual void on_mouse_pressed (const ToolAction &action) = 0;
    virtual void on_mouse_released(const ToolAction &action) = 0;
    virtual void on_mouse_moved   (const ToolAction &action) = 0;
    virtual const char *get_texture_name() const             = 0;

    bool is_to_change() const
    {
        return is_to_change_;
    }

//---------------------Variables-----------------------
protected:

    bool is_to_change_ = true;
//-----------------------------------------------------
};

#endif
