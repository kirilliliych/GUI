#include "sidepanel.hpp"


SidePanel::SidePanel(const Rectangle &rectangle, const std::vector<Tool *> &tools, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{
    tool_panel_ = new ToolPanel(Rectangle{rectangle.get_x() + 5,
                                          rectangle.get_y() + 5,
                                          rectangle.get_width() - 5,
                                          100}, tools, this);
    event_manager_.identif = 2000;
}


SidePanel::~SidePanel()
{}



EventHandlerState SidePanel::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    surface_->clear(0);
    surface_->draw_rectangle(area_, *DEFAULT_SIDE_PANEL_COLOR);

    return ContainerWidget::on_paint_event(event);
}
