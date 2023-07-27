#ifndef SIDEPANEL_HPP
#define SIDEPANEL_HPP


#include "colorpalette.hpp"
#include "../containerwidget.hpp"
#include "toolpanel.hpp"
#include "../scrollbar.hpp"



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
    ColorPalette *palette_   = nullptr;
};


#endif
