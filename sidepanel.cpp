#include "sidepanel.hpp"


SidePanel::SidePanel(const Rectangle &rectangle, const std::vector<Tool *> &tools, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{
    tool_panel_ = new ToolPanel(Rectangle{rectangle.get_x() + 5,
                                          rectangle.get_y() + 5,
                                          rectangle.get_width() - 5,
                                          140},
                                tools, this);

    palette_ = new ColorPalette(Rectangle{rectangle.get_x() + 5,
                                          rectangle.get_y() + tool_panel_->get_area().get_height() + 5,
                                          rectangle.get_width() - 5,
                                          300},
                                this);
}


SidePanel::~SidePanel()
{
    delete tool_panel_;
    delete palette_;
}


EventHandlerState SidePanel::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    surface_->clear(0);
    surface_->draw_rectangle(area_, *DEFAULT_WIDGET_COLOR);

    return ContainerWidget::on_paint_event(event);
}
