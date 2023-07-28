#ifndef PLUGIN_ZONE_HPP
#define PLUGIN_ZONE_HPP


#include "../widgets/containerwidget.hpp"


static const Color *DEFAULT_PLUGIN_ZONE_COLOR =&SEA_GREEN;


class PluginZone : public ContainerWidget
{
public:
//-------------------------------------------------------------------
    PluginZone(const Rectangle &rectangle, const char *title, ContainerWidget *parent = nullptr);

    ~PluginZone();
//-------------------------------------------------------------------
    EventHandlerState on_paint_event(const Event *event) override;
//---------------------------Variables-------------------------------
private:

    const char *title_ = nullptr;
};


#endif
