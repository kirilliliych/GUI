#ifndef PLUGINS_HPP
#define PLUGINS_HPP


#include "../widgets/button.hpp"
#include "../sfmlwrap/image.hpp"
#include "../widgets/painteditor/canvas.hpp"
#include "../widgets/scrollbar.hpp"
#include "../sfmlwrap/text.hpp"
#include "../toolsplugins/tool.hpp"
#include "../standard/tools.hpp"


class PluginButton;
class PluginSlider;


struct Plugin
{
    booba::GUID guid;
    void *library;
};


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
    void on_mouse_left(const ToolAction &action) override;
    void on_timer_event(const ToolAction &action) override;
    const char *get_texture_name() const override;

    void create_zone() override;
    void create_default_zone();

    bool set_toolbar_size(size_t width, size_t height);

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


class PluginSlider : public Scrollbar
{
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
    void set_pixel (size_t x, size_t y, Color color);
    void put_sprite(size_t x, size_t y, size_t w, size_t h, const char *texture);
    void clear(const Color &color);

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

    virtual ~PluginImage();
//-------------------------------------------------------------------
    bool contains(int x, int y) const;

    virtual size_t getW() override;
    virtual size_t getH() override;
    
    uint32_t getPixel(size_t x, size_t y) const;
    virtual uint32_t getPixel(size_t x, size_t y) override;
    virtual void setPixel(size_t x, size_t y, uint32_t color) override;
//---------------------------Variables-------------------------------
private:

    ImageSf *image_ = nullptr;
    uint32_t pixel_to_return_ = 0;
};


std::string form_file_path(const char *file_name);

void set_basic_event_mbedata(const ToolAction &action, booba::Event *event);

void import_plugins(std::vector<Plugin> &plugins);


#endif
