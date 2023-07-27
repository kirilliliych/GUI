#include "colorpalette.hpp"
#include "painteditor.hpp"


ColorPalette::ColorPalette(const Rectangle &rectangle, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{
    connect(this, &ColorPalette::foreground_color_changed, PaintEditor::application_, &PaintEditor::set_foreground_color);
    connect(this, &ColorPalette::background_color_changed, PaintEditor::application_, &PaintEditor::set_background_color);

    for (int color_index = 0; color_index < ALL_AVAILABLE_COLORS.size(); ++color_index)
    {
        add_color_cell_(ALL_AVAILABLE_COLORS[color_index]);
    }

    // do fore and background colorcells visible
}

ColorPalette::~ColorPalette()
{
    for (int cell_index = 0; cell_index < cells_.size(); ++cell_index)
    {
        delete cells_[cell_index];
    }
    delete foreground_color_cell_;
    delete background_color_cell_;
}


EventHandlerState ColorPalette::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    surface_->clear(0);
    surface_->draw_rectangle(area_, *DEFAULT_WIDGET_COLOR);

    return ContainerWidget::on_paint_event(event);
}

void ColorPalette::add_color_cell_(const Color &color)
{
    int cur_cells_quantity = cells_.size();

    ColorCell *new_color_cell = new ColorCell(Rectangle{area_.get_x() + (DEFAULT_COLOR_CELL_WIDTH + 1) * (cur_cells_quantity % COLOR_CELLS_IN_A_ROW),
                                                        area_.get_y() +  DEFAULT_COLOR_CELL_HEIGHT     * (cur_cells_quantity / COLOR_CELLS_IN_A_ROW),
                                                        DEFAULT_COLOR_CELL_WIDTH,
                                                        DEFAULT_COLOR_CELL_HEIGHT},
                                              color, this); 
    cells_.push_back(new_color_cell);
    new_color_cell->selected.connect(this, &ColorPalette::select_color_cell_);
}

void ColorPalette::select_color_cell_(ColorCell *color_cell, const bool fore_or_back)
{
    assert(color_cell != nullptr);

    if (!fore_or_back)
    {
        foreground_color_changed.emit(color_cell->get_color());
    }
    else
    {
        background_color_changed.emit(color_cell->get_color());
    }
}
//---------------------------------------------------------------------------------------
ColorCell::ColorCell(const Rectangle &rectangle, const Color &color, Widget *parent)
  : Widget(rectangle, parent),
    color_(color)
{}

ColorCell::~ColorCell()
{}


Color ColorCell::get_color() const
{
    return color_;
}

EventHandlerState ColorCell::on_mouse_button_pressed_event(const Event *event)
{
    assert(event != nullptr);

    if (event->mbedata_.button == MouseButton::Left)
    {
        is_pressed_ = true;

        return EventHandlerState::Accepted;
    }

    return EventHandlerState::Skipped;
}

EventHandlerState ColorCell::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

    if (is_pressed_ && (event->mbedata_.button == MouseButton::Left) && (area_.contains(event->mbedata_.position)))
    {
        // bool fore_or_back = shortcut_keys.ctrl ? true : false;
        std::cout << "EMIT" << std::endl;
        bool fore_or_back = with_ctrl_ ? true : false;
        selected.emit(this, fore_or_back);

        is_pressed_ = false;
    }

    return Widget::on_mouse_button_released_event(event);
}

EventHandlerState ColorCell::on_key_pressed_event(const Event *event)
{
            std::cout << "registered ctrl" << std::endl;
    switch (event->kedata_.key_code)
    {
        case KeyboardKey::LControl:
        {
            with_ctrl_ = true;

            return EventHandlerState::Accepted;
        }

        default:
        {
            return Widget::on_key_pressed_event(event);
        }
    }
}

EventHandlerState ColorCell::on_key_released_event(const Event *event)
{
    switch (event->kedata_.key_code)
    {
        case KeyboardKey::LControl:
        {
            with_ctrl_ = false;

            return EventHandlerState::Accepted;
        }

        default:
        {
            return Widget::on_key_pressed_event(event);
        }
    }
}

EventHandlerState ColorCell::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    surface_->clear(0);
    surface_->draw_rectangle(area_, color_);

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}
