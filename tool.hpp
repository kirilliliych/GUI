#ifndef TOOL_HPP
#define TOOL_HPP


#include "image.hpp"
#include "pluginzone.hpp"
#include "widget.hpp"


struct ToolAction
{
    ImageSf *image;
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
    {
        delete zone_;
    }
//-----------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) = 0;
    virtual void on_mouse_released(const ToolAction &action) = 0;
    virtual void on_mouse_moved   (const ToolAction &action) = 0;
    virtual void on_mouse_left    (const ToolAction &action) = 0;
    virtual void on_timer_event   (const ToolAction &action) = 0;
    virtual const char *get_texture_name() const             = 0;
    virtual void create_zone()
    {
        std::cout << "called Tool::create_zone function, fictive" << std::endl;
    }

    ContainerWidget *get_zone()
    {
        return zone_;
    }

    bool is_to_change() const
    {
        return is_to_change_;
    }
//---------------------Variables-----------------------
protected:

    PluginZone *zone_ = nullptr;

    bool is_to_change_ = true;
//-----------------------------------------------------
};

#endif
