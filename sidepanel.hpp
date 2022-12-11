#ifndef SIDEPANEL_HPP
#define SIDEPANEL_HPP


#include "containerwidget.hpp"
#include "toolpanel.hpp"


static const Color *DEFAULT_SIDE_PANEL_COLOR = &LIGHT_GREY;


class ToolPanel;

class SidePanel : public ContainerWidget
{
public:
//------------------------------------------------------------
    SidePanel(const Rectangle &rectangle, const std::vector<Tool *> &tools, ContainerWidget *widget = nullptr);

    ~SidePanel();
//------------------------------------------------------------
    EventHandlerState on_paint_event(const Event *event) override;

private:
//------------------Variables---------------------------------
    ToolPanel *tool_panel_   = nullptr;
    // ColorPalette *palette = nullptr;
};


#endif
