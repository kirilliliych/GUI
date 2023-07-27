#ifndef TOOL_HPP
#define TOOL_HPP


#include "../sfmlwrap/image.hpp"
#include "pluginzone.hpp"
#include "../widgets/widget.hpp"


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
      : requires_panel_(true)
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

    bool requires_panel() const
    {
        return requires_panel_;
    }

    ContainerWidget *get_zone()
    {
        return zone_;
    }

    bool changed_canvas() const
    {
        return changed_canvas_;
    }
//---------------------Variables-----------------------
protected:

    PluginZone *zone_ = nullptr;

    bool changed_canvas_ = true;
    bool requires_panel_ = true;
//-----------------------------------------------------
};

#endif
