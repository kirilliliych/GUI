#ifndef PAINT_EDITOR_HPP
#define PAINT_EDITOR_HPP


#include "canvas.hpp"
#include "../containerwidget.hpp"
#include "../../inbuilttools/defaulttools.hpp"
#include "../../plugins/plugins.hpp"
#include "../scrollbar.hpp"
#include "sidepanel.hpp"


static const Color *DEFAULT_FOREGROUND_COLOR = &BLACK;
static const Color *DEFAULT_BACKGROUND_COLOR = &WHITE;


class Canvas;
class SidePanel;


struct ApplicationContext
{
    Color foreground_color = *DEFAULT_FOREGROUND_COLOR;
    Color background_color = *DEFAULT_BACKGROUND_COLOR;
};

struct PureColors
{
    uint32_t fg_color = DEFAULT_FOREGROUND_COLOR->get_uint32_color();
    uint32_t bg_color = DEFAULT_BACKGROUND_COLOR->get_uint32_color();
};


class PaintEditor : public ContainerWidget
{
public:

    static PaintEditor *application_;
//----------------------------------------------------------
    PaintEditor(const Rectangle &Rectangle, ContainerWidget *parent = nullptr);

    ~PaintEditor();
//----------------------------------------------------------
    void set_current_tool(Tool *another_tool);
    Tool *get_current_tool() const;

    void set_foreground_color(const Color &color);
    void set_background_color(const Color &color);

    const std::vector<Tool *> &get_tools() const;

    void add_tool(Tool *tool);

private:

    void create_default_tools_();
//------------------------Variables-------------------------
public:

    std::vector<Plugin> plugins_{};
    
    const std::chrono::system_clock::time_point beginning_time{};

    Tool *cur_tool_ = nullptr;
    std::vector<Tool *> tools_{};

    Canvas *canvas_                = nullptr;
    Scrollbar *canvas_x_scrollbar_ = nullptr;
    Scrollbar *canvas_y_scrollbar_ = nullptr;
    SidePanel *side_panel_         = nullptr;

    ApplicationContext basic_colors;
    PureColors context;
//----------------------------------------------------------
};

#endif