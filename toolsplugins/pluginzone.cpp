#include "pluginzone.hpp"


PluginZone::PluginZone(const Rectangle &rectangle, const char *title, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent),
    title_(title)
{}

PluginZone::~PluginZone()
{}

EventHandlerState PluginZone::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    surface_->clear(0);
    surface_->draw_rectangle(area_, *DEFAULT_PLUGIN_ZONE_COLOR);

    return ContainerWidget::on_paint_event(event);
}
