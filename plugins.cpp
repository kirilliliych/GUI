#include <dlfcn.h>
#include <filesystem>
#include "painteditor.hpp"
#include "plugins.hpp"
#include "tools.hpp"


static const char *PLUGINS_DIR = "./plugins/";


booba::ApplicationContext *booba::APPCONTEXT = nullptr;
PluginTool *PluginTool::cur_plugin_ = nullptr;

std::string form_file_path(const char *file_name)
{
    if (file_name == nullptr)
    {
        std::cerr << "file name is nullptr " << __FILE__ << " " << __LINE__ << std::endl;
        
        return std::string();
    }

    if (file_name[0] == '/')
    {
        return file_name;
    }

    std::string result = PLUGINS_DIR;

    return result + file_name;
}


void set_basic_event_mbedata(const ToolAction &action, booba::Event *event)         // looked like copypaste, decided to do a wrap
{
    assert(event != nullptr);

    event->Oleg.mbedata.x = action.point.x;
    event->Oleg.mbedata.y = action.point.y;

    event->Oleg.mbedata.shift = action.shift; 
    event->Oleg.mbedata.alt   = action.alt;
    event->Oleg.mbedata.ctrl  = action.ctrl;
}


void import_plugins()
{
    booba::APPCONTEXT = reinterpret_cast<booba::ApplicationContext *> (&PaintEditor::application_->context);

    for (const auto &file : std::filesystem::directory_iterator(PLUGINS_DIR))
    {
        std::string cur_path = file.path();
        std::cout << "Cur file is " << cur_path.c_str() << std::endl;
        if (cur_path.ends_with(".aboba.so"))
        {
            std::cout << "Module found" << std::endl;

            void *lib = dlopen(cur_path.c_str(), RTLD_LAZY);
            char *errr = dlerror();
            std::cout << errr << std::endl;

            void (*init_func)(void) = reinterpret_cast<void (*)()> (dlsym(lib, "init_module"));

            std::cout << "Init_module address: " << reinterpret_cast<void *> (init_func) << std::endl;

            (*init_func)();
        }
    }
}

//-------------------------------------------------------------------
PluginTool::PluginTool(booba::Tool *tool)
  : tool_(tool),
    texture_file_name_(form_file_path(tool->getTexture()))
{

}

PluginTool::~PluginTool()
{
    delete tool_;
    tool_ = nullptr;
}

void PluginTool::on_mouse_pressed(const ToolAction &action)
{
    cur_plugin_ = this;

    //printf("PluginTool on_mouse_pressed\n");
    set_basic_event_mbedata(action, &event_);
    event_.Oleg.mbedata.button = booba::MouseButton::Left;
    event_.type                = booba::EventType::MousePressed;

    PluginImage cur_image(action.image);
    assert(tool_ != nullptr);
    tool_->apply(&cur_image, &event_);
}

void PluginTool::on_mouse_released(const ToolAction &action)
{
    cur_plugin_ = this;

    //printf("PluginTool on_mouse_released\n");
    set_basic_event_mbedata(action, &event_);
    event_.Oleg.mbedata.button = booba::MouseButton::Left;
    event_.type                = booba::EventType::MouseReleased;

    PluginImage cur_image(action.image);
    assert(tool_ != nullptr);
    tool_->apply(&cur_image, &event_);
    
}

void PluginTool::on_mouse_moved(const ToolAction &action)
{
    cur_plugin_ = this;

    //printf("PluginTool on_mouse_moved\n");
    event_.Oleg.motion.x     = action.point.x;
    event_.Oleg.motion.y     = action.point.y;
    event_.Oleg.motion.rel_x = action.point.x - prev_position_.x;
    event_.Oleg.motion.rel_y = action.point.y - prev_position_.y;
    prev_position_ = action.point;
    event_.type = booba::EventType::MouseMoved;

    PluginImage cur_image(action.image);
    assert(tool_ != nullptr);
    tool_->apply(&cur_image, &event_);
}

const char *PluginTool::get_texture_name() const
{
    return texture_file_name_.c_str();
}

void PluginTool::create_zone(const Rectangle &rectangle, ContainerWidget *parent)
{
    cur_plugin_ = this;

    //printf("PluginTool create_zone\n");
    Tool::create_zone(rectangle, parent);
    tool_->buildSetupWidget();
}

void PluginTool::on_button_clicked(const PluginButton *button)
{
    cur_plugin_ = this;

    //printf("PluginTool on_button_clicked\n");
    event_.Oleg.bcedata.id = reinterpret_cast<uint64_t> (button);
    event_.type            = booba::EventType::ButtonClicked;

    tool_->apply(nullptr, &event_); // better not nullptr but still pluginimage?
}

void PluginTool::on_slider_moved(int32_t value, const PluginSlider *slider)
{
    cur_plugin_ = this;

    event_.Oleg.smedata.id    = reinterpret_cast<uint64_t> (slider);
    event_.Oleg.smedata.value = value;
    event_.type = booba::EventType::ScrollbarMoved; 

    tool_->apply(nullptr, &event_);
}

void PluginTool::on_canvas_event(const booba::Event event)
{
    cur_plugin_ = this;

   // printf("PluginTool on_canvas_event\n");
    tool_->apply(nullptr, &event_);
}
//-------------------------------------------------------------------
PluginButton::PluginButton(const Rectangle &rectangle, const char *text, Widget *parent)
  : Button(rectangle, text, parent),
    color_(*DEFAULT_WIDGET_COLOR)
{
    Button::clicked.connect(this, &PluginButton::click_this);
    
    text_.set_color(*DEFAULT_TEXT_COLOR);
}

PluginButton::~PluginButton()
{}

void PluginButton::click_this()
{
    clicked.emit(this);
}

EventHandlerState PluginButton::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    const Color *to_paint = is_pressed_ ? DEFAULT_BUTTON_PRESSED_COLOR : is_hovered_ ? DEFAULT_BUTTON_HOVERING_COLOR : &color_;
    surface_->clear(0);
    surface_->draw_rectangle(area_, *to_paint);
    
    draw_frame_();

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}
//-------------------------------------------------------------------
PluginSlider::PluginSlider(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent)
{}

PluginSlider::~PluginSlider()
{}

void PluginSlider::value_change(int32_t value)
{
    value_changed.emit(value, this);
}
//-------------------------------------------------------------------
PluginCanvas::PluginCanvas(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent)
{}

PluginCanvas::~PluginCanvas()
{}

void PluginCanvas::putPixel(int32_t x, int32_t y, Color color)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < surface_->get_size().x);
    assert(y < surface_->get_size().y);

    surface_->draw_point(Point2d{x, y}, color); 
}

void PluginCanvas::putSprite(int32_t x, int32_t y, uint32_t w, uint32_t h, const char *texture)
{
    Texture temp_texture{form_file_path(texture).c_str()};
    assert(form_file_path(texture) != std::string());

    Sprite temp_sprite{temp_texture, Rectangle{0, 0, static_cast<int> (w), static_cast<int> (h)}};
    surface_->draw_sprite(Point2d{x, y}, temp_sprite);
}

EventHandlerState PluginCanvas::on_mouse_button_pressed_event(const Event *event)
{
    assert(event != nullptr);
    
    if (event->mbedata_.button != MouseButton::Left)
    {
        return EventHandlerState::Skipped;
    }

    booba::Event standard_event;
    standard_event.Oleg.cedata.id = reinterpret_cast<uint64_t> (this);
    standard_event.Oleg.cedata.x  = event->mbedata_.position.x;
    standard_event.Oleg.cedata.y  = event->mbedata_.position.y;
    standard_event.type = booba::EventType::CanvasMPressed;

    mouse_button_pressed.emit(standard_event);

    return EventHandlerState::Accepted;
}

EventHandlerState PluginCanvas::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

    if (event->mbedata_.button != MouseButton::Left)
    {
        return EventHandlerState::Skipped;
    }

    booba::Event standard_event;
    standard_event.Oleg.cedata.id = reinterpret_cast<uint64_t> (this);
    standard_event.Oleg.cedata.x  = event->mbedata_.position.x;
    standard_event.Oleg.cedata.y  = event->mbedata_.position.y;
    standard_event.type = booba::EventType::CanvasMReleased;

    mouse_button_released.emit(standard_event);

    return EventHandlerState::Accepted;
}

EventHandlerState PluginCanvas::on_mouse_moved_event(const Event *event)
{
    assert(event != nullptr);

    booba::Event standard_event;
    standard_event.Oleg.cedata.id = reinterpret_cast<uint64_t> (this);
    standard_event.Oleg.cedata.x  = event->mmedata_.position.x;
    standard_event.Oleg.cedata.y  = event->mmedata_.position.y;
    standard_event.type = booba::EventType::CanvasMMoved;

    mouse_moved.emit(standard_event);

    return EventHandlerState::Accepted;
}
//-------------------------------------------------------------------
PluginImage::PluginImage(ImageSf *image)
  : image_(image)
{}

PluginImage::~PluginImage()
{}

bool PluginImage::contains(int x, int y) const
{
    return (x >= 0)                  &&
           (y >= 0)                  &&
           (x < image_->get_width()) &&
           (y < image_->get_height());
}

uint32_t PluginImage::getW()
{
    return static_cast<uint32_t> (image_->get_width());
}
uint32_t PluginImage::getH()
{
    return static_cast<uint32_t> (image_->get_height());
}

uint32_t PluginImage::getPixel(int32_t x, int32_t y) const
{
    return (const_cast<PluginImage *> (this))->getPixel(x, y);
}

uint32_t PluginImage::getPixel(int32_t x, int32_t y)
{
    if (!contains(x, y))
    {
        std::cerr << __FILE__ << " " << __LINE__ << " tried to get pixel out of range\n";

        return BLACK.get_uint32_color();
    }
    
    return image_->get_pixel(x, y).get_uint32_color();
}

void PluginImage::putPixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (!contains(x, y))
    {
        std::cerr << __FILE__ << " " << __LINE__ << " " << " tried to set pixel out of range\n";
    }
    
    image_->set_pixel(x, y, Color{color});
}  

uint32_t &PluginImage::operator ()(uint32_t x, uint32_t y)
{
    pixel_to_return_ = getPixel(static_cast<int32_t> (x), static_cast<int32_t> (y));

    return pixel_to_return_;
}
const uint32_t &PluginImage::operator ()(uint32_t x, uint32_t y) const
{
    return const_cast<const uint32_t &> ((const_cast<PluginImage *> (this))->operator()(x, y));
}
//-------------------------------------------------------------------
namespace booba
{
    extern "C" uint64_t createButton(int32_t x, int32_t y, uint32_t w, uint32_t h, const char *text)
    {   
        //printf("createButtoncalled\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        PluginButton *new_button = new PluginButton(Rectangle{x, y, static_cast<int> (w), static_cast<int> (h)}, text, PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        connect(new_button, &PluginButton::clicked, PluginTool::cur_plugin_, &PluginTool::on_button_clicked);

        return reinterpret_cast<uint64_t> (new_button);
    }

    extern "C" uint64_t createLabel(int32_t x, int32_t y, uint32_t w, uint32_t h, const char* text)
    {
        //printf("createLabelcall\n");   
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        Text *new_text = new Text(text, Rectangle{x, y, static_cast<int> (w), static_cast<int> (h)}, PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        
        return reinterpret_cast<uint64_t> (new_text);
    }

    extern "C" uint64_t createScrollbar(int32_t x, int32_t y, uint32_t w, uint32_t h, int32_t maxValue, int32_t startValue)
    {
        // NOT FULLY IMPLEMENTED
        //printf("createScrollbarcalled\n");
        
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        Widget *stub = new Widget(Rectangle{x, y, static_cast<int> (w), static_cast<int> (h)}, PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);

        return reinterpret_cast<uint64_t> (stub);
    }

    extern "C" uint64_t createCanvas(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        //printf("createCanvascalled\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        PluginCanvas *new_canvas = new PluginCanvas(Rectangle{x, y, static_cast<int> (w), static_cast<int> (h)}, PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        connect(new_canvas, &PluginCanvas::mouse_button_released, PluginTool::cur_plugin_, &PluginTool::on_canvas_event);
        connect(new_canvas, &PluginCanvas::mouse_button_pressed,  PluginTool::cur_plugin_, &PluginTool::on_canvas_event);
        connect(new_canvas, &PluginCanvas::mouse_moved,           PluginTool::cur_plugin_, &PluginTool::on_canvas_event);

        return reinterpret_cast<uint64_t> (new_canvas);
    }

    extern "C" void putPixel(uint64_t canvas, int32_t x, int32_t y, uint32_t color)
    {
        printf("putPixelcalled\n");
        if (canvas == 0)
        {
            return;
        }

        PluginCanvas *canvas_ptr = reinterpret_cast<PluginCanvas *> (canvas);
        canvas_ptr->putPixel(x, y, color);
    }

    extern "C" void putSprite(uint64_t canvas, int32_t x, int32_t y, uint32_t w, uint32_t h, const char* texture)
    {
        //printf("putSpritecalled\n");
        if (canvas == 0)
        {
            return;
        }

        PluginCanvas *canvas_ptr = reinterpret_cast<PluginCanvas *> (canvas);
        canvas_ptr->putSprite(x, y, w, h, texture);
    }

    extern "C" void addTool(booba::Tool *tool)
    {
        //printf("addToolcalled\n");
        PaintEditor::application_->add_tool(new PluginTool(tool));
    }

    extern "C" void addFilter(booba::Tool *tool)
    {
        //printf("addFiltercalled\n");
        PaintEditor::application_->add_tool(new PluginTool(tool));
    }

    Image::~Image()
    {}

    Tool::~Tool()
    {}
}
