#ifndef PLUGINS_HPP
#define PLUGINS_HPP


#include "button.hpp"
#include "canvas.hpp"
#include "image.hpp"
#include "text.hpp"
#include "tools.hpp"
#include <unistd.h>


class PluginButton;
class PluginSlider;


class PluginTool : public Tool, public SignalObject
{
public:

    static PluginTool *cur_plugin_;
//-------------------------------------------------------------------
    PluginTool(booba::Tool *tool);

    ~PluginTool() override;
//-------------------------------------------------------------------
    void on_mouse_pressed(const ToolAction &action) override;
    void on_mouse_released(const ToolAction &action) override;
    void on_mouse_moved(const ToolAction &action) override;
    const char *get_texture_name() const override;
    void create_zone(const Rectangle &rectangle, ContainerWidget *parent) override;

    void on_button_clicked(const PluginButton *button);
    void on_slider_moved(int32_t value, const PluginSlider *slider);
    void on_canvas_event(const booba::Event event);
//---------------------------Variables-------------------------------
private:

    booba::Tool *tool_;
    booba::Event event_;

    Point2d prev_position_{};
    std::string texture_file_name_;
};


class PluginButton : public Button
{
public:
//-------------------------------------------------------------------
    PluginButton(const Rectangle &rectangle, const char *text, Widget *parent);

    ~PluginButton();
//-------------------------------------------------------------------
    void click_this();

    EventHandlerState on_paint_event(const Event *event) override;
//---------------------------Variables-------------------------------
private:

    Color color_ = *DEFAULT_WIDGET_COLOR;

public:

    Signal<const PluginButton *> clicked;
};


class PluginSlider : public Widget
{
// unimplemented, this is stub
public:
//-------------------------------------------------------------------
    PluginSlider(const Rectangle &rectangle, Widget *parent);

    ~PluginSlider();
//-------------------------------------------------------------------
    void value_change(int32_t value);
//---------------------------Variables-------------------------------
public:

    Signal<int32_t, const PluginSlider *> value_changed;
};


class PluginCanvas : public Widget
{
public:
//-------------------------------------------------------------------
    PluginCanvas(const Rectangle &rectangle, Widget *parent);

    ~PluginCanvas();
//-------------------------------------------------------------------
    void putPixel (int32_t x, int32_t y, Color color);
    void putSprite(int32_t x, int32_t y, uint32_t w, uint32_t h, const char *texture);

    EventHandlerState on_mouse_button_pressed_event(const Event *event) override;
    EventHandlerState on_mouse_button_released_event(const Event *event) override;
    EventHandlerState on_mouse_moved_event(const Event *event) override;
    EventHandlerState on_paint_event(const Event *event) override
    {
        return EventHandlerState::Accepted;
    }

//---------------------------Variables-------------------------------
public:

    Signal<booba::Event> mouse_button_pressed;
    Signal<booba::Event> mouse_button_released;
    Signal<booba::Event> mouse_moved;
};


class PluginImage : public booba::Image
{
public:
//-------------------------------------------------------------------
    PluginImage(ImageSf *image);

    virtual ~PluginImage() override;
//-------------------------------------------------------------------
    bool contains(int x, int y) const;

    virtual uint32_t getW() override;
    virtual uint32_t getH() override;
    
    uint32_t getPixel(int32_t x, int32_t y) const;
    virtual uint32_t getPixel(int32_t x, int32_t y) override;
    virtual void putPixel(uint32_t x, uint32_t y, uint32_t color) override;    
    virtual uint32_t &operator ()(uint32_t x, uint32_t y) override;
    virtual const uint32_t &operator ()(uint32_t x, uint32_t y) const override;
//---------------------------Variables-------------------------------
private:

    ImageSf *image_ = nullptr;
    uint32_t pixel_to_return_ = 0;
};


std::string form_file_path(const char *file_name);

void set_basic_event_mbedata(const ToolAction &action, booba::Event *event);

void import_plugins();


#endif
