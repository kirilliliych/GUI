#ifndef COLORPALETTE_HPP
#define COLORPALETTE_HPP


#include "../containerwidget.hpp"


const int DEFAULT_COLOR_CELL_WIDTH  = 40;
const int DEFAULT_COLOR_CELL_HEIGHT = 40;
const int COLOR_CELLS_IN_A_ROW      = 6;


class ColorCell;

class ColorPalette : public ContainerWidget
{
public:
//-------------------------------------------------------------------------------
    ColorPalette(const Rectangle &rectangle, ContainerWidget *parent = nullptr);

    ~ColorPalette();
//-------------------------------------------------------------------------------
    EventHandlerState on_paint_event(const Event *event) override;

private:

    void add_color_cell_(const Color &color);

    void select_color_cell_(ColorCell *color_cell, const bool fore_or_back);
//----------------------Variables------------------------------------------------
    ColorCell *foreground_color_cell_ = nullptr;
    ColorCell *background_color_cell_ = nullptr;

    std::vector<ColorCell *> cells_{};

public:

    Signal<const Color &> foreground_color_changed;
    Signal<const Color &> background_color_changed;
};


class ColorCell : public Widget
{
public:
//-------------------------------------------------------------------------------
    ColorCell(const Rectangle &rectangle, const Color &color, Widget *parent = nullptr);

    ~ColorCell();
//-------------------------------------------------------------------------------
    Color get_color() const;

    EventHandlerState on_mouse_button_pressed_event (const Event *event) override;
    EventHandlerState on_mouse_button_released_event(const Event *event) override;
    EventHandlerState on_key_pressed_event (const Event *event) override;
    EventHandlerState on_key_released_event(const Event *event) override;
    EventHandlerState on_paint_event (const Event *event) override;
//---------------------------Variables-------------------------------------------
private:

    bool is_pressed_       = false;
    bool with_ctrl_        = false;
    bool requires_repaint_ = true;

    Color color_{};

public:

    Signal<ColorCell *, bool> selected;
};


#endif
