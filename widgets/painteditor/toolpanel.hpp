#ifndef TOOL_PANEL_HPP
#define TOOL_PANEL_HPP


#include "../button.hpp"
#include "../../utilities/color.hpp"
#include "../../utilities/colorlibrary.hpp"
#include "../containerwidget.hpp"
#include "painteditor.hpp"
#include "../../events/signals/signal.hpp"
#include "../../toolsplugins/tool.hpp"


const int DEFAULT_TOOL_BUTTON_WIDTH  = 40;
const int DEFAULT_TOOL_BUTTON_HEIGHT = 40;

static const Color *DEFAULT_TOOL_BUTTON_COLOR                  = &YELLOW;
static const Color *DEFAULT_TOOL_BUTTON_HOVERING_COLOR         = &GREEN;
static const Color *DEFAULT_TOOL_BUTTON_SELECTED_FRAME_COLOR   = &RED;
static const Color *DEFAULT_TOOL_BUTTON_UNSELECTED_FRAME_COLOR = &GREY; 


class ToolButton;

class ToolPanel : public ContainerWidget
{

public:
//--------------------------------------------------------------------
    ToolPanel(const Rectangle &rectangle, const std::vector<Tool *> &tools, ContainerWidget *parent = nullptr);

    ~ToolPanel();
//--------------------------------------------------------------------
    Tool *get_selected_tool() const;

    EventHandlerState on_paint_event(const Event *event) override;

private:

    void add_tool_button_(Tool *tool);
    
    void select_tool_button_(ToolButton *button);
    
//---------------------------Variables--------------------------------
    const std::vector<Tool *> &tools_;

    std::vector<ToolButton *> buttons_{};
    ToolButton *selected_tool_button_ = nullptr;

public:

    Signal<Tool *> selected_tool_changed;
};


class ToolButton : public Button
{
public:
//--------------------------------------------------------------------
    ToolButton(const Rectangle &rectangle, Tool *tool, Widget *parent = nullptr);

    ~ToolButton();
//--------------------------------------------------------------------
    Tool *get_tool() const;

    void on_select();
    void on_deselect();

    EventHandlerState on_mouse_button_released_event(const Event *event) override;

    EventHandlerState on_paint_event(const Event *event) override;

    void draw_frame_(const Color &selected_tool_color   = *DEFAULT_TOOL_BUTTON_SELECTED_FRAME_COLOR,
                     const Color &unselected_tool_color = *DEFAULT_TOOL_BUTTON_UNSELECTED_FRAME_COLOR) override;
//---------------------------Variables--------------------------------
private:

    bool is_selected_ = false;

    Tool *tool_ = nullptr;

public:

    Signal<ToolButton *> chosen;
};


#endif
