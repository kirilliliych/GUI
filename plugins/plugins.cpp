#include <dlfcn.h>
#include <filesystem>
#include "../widgets/painteditor/painteditor.hpp"
#include "plugins.hpp"


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


void import_plugins(std::vector<Plugin> &plugins)
{
    booba::APPCONTEXT = reinterpret_cast<booba::ApplicationContext *> (&PaintEditor::application_->context);

    for (const auto &file : std::filesystem::directory_iterator(PLUGINS_DIR))
    {
        std::string cur_path = file.path();
        std::cout << "Cur file is " << cur_path.c_str() << std::endl;
        if (cur_path.ends_with(".aboba.so"))
        {
            std::cout << "Module found" << std::endl;

            void *lib_ptr = dlopen(cur_path.c_str(), RTLD_LAZY);
            char *dlerr = dlerror();
            if (dlerr != nullptr)
            {
                std::cerr << dlerr << std::endl;
            }

            // booba::GUID (*GUID_func)(void) = reinterpret_cast<booba::GUID (*)()> (dlsym(lib_ptr, "getGUID"));
            // if (GUID_func == nullptr)
            // {
            //     std::cerr << "Plugin " << file.path().filename().c_str() << " does not contain getGUID() function. Considered invalid" << std::endl;

            //     continue;
            // }

            // booba::GUID cur_plugin_guid = (*GUID_func)();
            booba::GUID cur_plugin_guid;
            strncpy(cur_plugin_guid.str, "5f294767-c442-4082-8c04-d5a39149a207", 37);
            if (cur_plugin_guid.str[36] != 0)
            {
                std::cerr << "Plugin " << file.path().filename().c_str() << " contains invalid GUID. Considered invalid" << std::endl;

                continue;
            }

            plugins.push_back(Plugin{cur_plugin_guid, lib_ptr});
        }
    }

    std::cout << "Plugin loading finished. Starting initialization" << std::endl;

    for (int i = 0; i < plugins.size(); ++i)
    {
        void (*init_func)(void) = reinterpret_cast<void (*)()> (dlsym(plugins[i].library, "init_module"));

        if (init_func == nullptr)
        {
            std::cout << "Plugin with GUID " << plugins[i].guid.str << " does not contain init_module function. Considered invalid" << std::endl;

            continue;
        }

        std::cout << "Init_module address: " << reinterpret_cast<void *> (init_func) << std::endl;
        (*init_func)();
    }

    std::cout << "Plugin initialization finished" << std::endl;
}

//-------------------------------------------------------------------
PluginTool::PluginTool(booba::Tool *tool)
  : tool_(tool),
    texture_file_name_(form_file_path(tool->getTexture()))
{}

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

void PluginTool::on_mouse_left(const ToolAction &action)
{
    cur_plugin_ = this;

    event_.type = booba::EventType::CanvasMLeft;
    
    PluginImage cur_image(action.image);
    tool_->apply(&cur_image, &event_);
}

void PluginTool::on_timer_event(const ToolAction &action)
{
    cur_plugin_= this;

    event_.type = booba::EventType::TimerEvent;
    std::chrono::system_clock::time_point cur_time = std::chrono::high_resolution_clock::now();
    event_.Oleg.tedata.time = std::chrono::duration_cast<std::chrono::milliseconds> (cur_time - PaintEditor::application_->beginning_time).count();

    PluginImage cur_image(action.image);
    tool_->apply(&cur_image, &event_);
}


const char *PluginTool::get_texture_name() const
{
    return texture_file_name_.c_str();
}

void PluginTool::create_zone()
{
    cur_plugin_ = this;

    //printf("PluginTool create_zone\n");
    tool_->buildSetupWidget();
    if (zone_ == nullptr)
    {
        create_default_zone();
    }
}

void PluginTool::create_default_zone()
{
    assert(zone_ == nullptr);           // should be called ONLY IF THERE IS NO SPECIFIED PLACE FOR PLUGIN STUFF BEFORE AND PLUGIN ZONE SIZE WAS NOT SPECIALIZED 

    cur_plugin_ = this;

    std::cout << "DEFAULT ZONE CREATED" << std::endl;
    zone_ = new PluginZone(Rectangle{1030, 630, 300, 250}, "DefaultPluginZone", PaintEditor::application_);
}

bool PluginTool::set_toolbar_size(size_t width, size_t height)
{
    if (zone_ != nullptr)
    {
        std::cout << "set_toolbar_size failed: there is plugin zone created already" << std::endl;

        return false;
    }
    if ((width > 1920) || (height > 1920))
    {
        std::cout << "set_toolbar_size failed: attempt to create too large toolbar" << std::endl;
    }

    std::cout << "NON_DEFAULT_ZONE_CREATED" << std::endl;
    zone_ = new PluginZone(Rectangle{0, 0, static_cast<int> (width), static_cast<int> (height)}, "DefaultPluginZone", PaintEditor::application_);

    return zone_ == nullptr ? false : true;
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
    event_.type = booba::EventType::SliderMoved; 

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
  : Scrollbar(rectangle, ScrollbarOrientation::Horizontal, rectangle.get_width() / 10, parent)
{
    Scrollbar::value_changed.connect(this, &PluginSlider::value_change);
}

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

void PluginCanvas::set_pixel(size_t x, size_t y, Color color)
{
    assert(x >= 0);
    assert(y >= 0);
    assert(x < surface_->get_size().x);
    assert(y < surface_->get_size().y);

    surface_->draw_point(Point2d{static_cast<int> (x), static_cast<int> (y)}, color); 
}

void PluginCanvas::put_sprite(size_t x, size_t y, size_t w, size_t h, const char *texture)
{
    std::string real_texture_path = form_file_path(texture);
    std::cout << "Plugin asked for " << real_texture_path.c_str() << std::endl;
    if (real_texture_path == std::string())
    {
        std::cerr << "Invalid texture name" << std::endl;

        return;
    }
    if (!std::filesystem::exists(real_texture_path))
    {
        std::cerr << real_texture_path.c_str() << " does not exist" << std::endl;

        return;
    }

    Texture temp_texture{form_file_path(texture).c_str()};

    Sprite temp_sprite{temp_texture, Rectangle{0, 0, static_cast<int> (w), static_cast<int> (h)}};
    surface_->draw_sprite(Point2d{static_cast<int> (x), static_cast<int> (y)}, temp_sprite);
}

void PluginCanvas::clear(const Color &color)
{
    surface_->clear();
    surface_->update();
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

size_t PluginImage::getW()
{
    return static_cast<size_t> (image_->get_width());
}
size_t PluginImage::getH()
{
    return static_cast<size_t> (image_->get_height());
}

uint32_t PluginImage::getPixel(size_t x, size_t y) const
{
    return (const_cast<PluginImage *> (this))->getPixel(x, y);
}

uint32_t PluginImage::getPixel(size_t x, size_t y)
{
    if (!contains(x, y))
    {
        std::cerr << __FILE__ << " " << __LINE__ << " tried to get pixel out of range\n";

        return BLACK.get_uint32_color();
    }
    
    return image_->get_pixel(x, y).get_uint32_color();
}

void PluginImage::setPixel(size_t x, size_t y, uint32_t color)
{
    if (!contains(x, y))
    {
        std::cerr << __FILE__ << " " << __LINE__ << " " << " tried to set pixel out of range\n";
    }
    
    image_->set_pixel(x, y, Color{color});
}
//-------------------------------------------------------------------
namespace booba
{
    extern "C" bool setToolBarSize(size_t w, size_t h)
    {
        printf("setToolBarSize called\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        return PluginTool::cur_plugin_->set_toolbar_size(w, h);
    }

    extern "C" uint64_t createButton(size_t x, size_t y, size_t w, size_t h, const char *text)
    {   
        printf("createButtoncalled\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }
        
        if (PluginTool::cur_plugin_->get_zone() == nullptr)
        {
            PluginTool::cur_plugin_->create_default_zone();
        }

        PluginButton *new_button = new PluginButton(Rectangle{static_cast<int> (x),
                                                              static_cast<int> (y),
                                                              static_cast<int> (w),
                                                              static_cast<int> (h)
                                                             }, text, PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        connect(new_button, &PluginButton::clicked, PluginTool::cur_plugin_, &PluginTool::on_button_clicked);

        return reinterpret_cast<uint64_t> (new_button);
    }

    extern "C" uint64_t createLabel(size_t x, size_t y, size_t w, size_t h, const char *text)
    {
        printf("createLabelcalled\n");   
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        if (PluginTool::cur_plugin_->get_zone() == nullptr)
        {
            PluginTool::cur_plugin_->create_default_zone();
        }

        Text *new_text = new Text(text, Rectangle{static_cast<int> (x),
                                                  static_cast<int> (y),
                                                  static_cast<int> (w),
                                                  static_cast<int> (h)},
                                        PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        
        return reinterpret_cast<uint64_t> (new_text);
    }

    extern "C" uint64_t createSlider(size_t x, size_t y, size_t w, size_t h, int64_t minValue, int64_t maxValue, int64_t startValue)
    {
        printf("createScrollbarcalled\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        if (PluginTool::cur_plugin_->get_zone() == nullptr)
        {
            PluginTool::cur_plugin_->create_default_zone();
        }

        PluginSlider *slider = new PluginSlider(Rectangle{static_cast<int> (x),
                                                          static_cast<int> (y),
                                                          static_cast<int> (w),
                                                          static_cast<int> (h)
                                                         },
                                                PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        slider->set_min_value(minValue);
        slider->set_max_value(maxValue);
        slider->set_cur_value(startValue);
        connect(slider, &PluginSlider::value_changed, PluginTool::cur_plugin_, &PluginTool::on_slider_moved);

        return reinterpret_cast<uint64_t> (slider);
    }

    extern "C" uint64_t createCanvas(size_t x, size_t y, size_t w, size_t h)
    {
        printf("createCanvascalled\n");
        if (PluginTool::cur_plugin_ == nullptr)
        {
            return 0;
        }

        if (PluginTool::cur_plugin_->get_zone() == nullptr)
        {
            PluginTool::cur_plugin_->create_default_zone();
        }

        PluginCanvas *new_canvas = new PluginCanvas(Rectangle{static_cast<int> (x),
                                                              static_cast<int> (y),
                                                              static_cast<int> (w),
                                                              static_cast<int> (h)},
                                                    PluginTool::cur_plugin_->get_zone());
        assert(PluginTool::cur_plugin_->get_zone() != nullptr);
        connect(new_canvas, &PluginCanvas::mouse_button_released, PluginTool::cur_plugin_, &PluginTool::on_canvas_event);
        connect(new_canvas, &PluginCanvas::mouse_button_pressed,  PluginTool::cur_plugin_, &PluginTool::on_canvas_event);
        connect(new_canvas, &PluginCanvas::mouse_moved,           PluginTool::cur_plugin_, &PluginTool::on_canvas_event);

        return reinterpret_cast<uint64_t> (new_canvas);
    }

    extern "C" void putPixel(uint64_t canvas, size_t x, size_t y, uint32_t color)
    {
        //printf("putPixelcalled\n");
        if (canvas == 0)
        {
            return;
        }
    
        PluginCanvas *canvas_ptr = reinterpret_cast<PluginCanvas *> (canvas);
        canvas_ptr->set_pixel(x, y, color);
    }

    extern "C" void putSprite(uint64_t canvas, size_t x, size_t y, size_t w, size_t h, const char *texture)
    {
        //printf("putSpritecalled\n");
        if (canvas == 0)
        {
            return;
        }

        PluginCanvas *canvas_ptr = reinterpret_cast<PluginCanvas *> (canvas);
        canvas_ptr->put_sprite(x, y, w, h, texture);
    }

    extern "C" void cleanCanvas(uint64_t canvasId, uint32_t color)
    {
        if (canvasId == 0)
        {
            return;
        }

        PluginCanvas *canvas_ptr = reinterpret_cast<PluginCanvas *> (canvasId);
        canvas_ptr->clear(Color{color});
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


    extern "C" void* getLibSymbol(GUID guid, const char *name)
    {
        assert(name != nullptr);

        for (int plugin_index = 0; plugin_index < PaintEditor::application_->plugins_.size(); ++plugin_index)
        {
            if (strncmp(guid.str, PaintEditor::application_->plugins_[plugin_index].guid.str, sizeof(guid.str)) == 0)
            {
                void *sym_place = dlsym(PaintEditor::application_->plugins_[plugin_index].library, name);
                char *dlerr = dlerror();
                if (dlerr != nullptr)
                {
                    fprintf(stderr, "%s\n", dlerr);
                    fprintf(stderr, "%s ", name);
                    fprintf(stderr, "was not found via dlsym\n");

                    return nullptr;
                }

                return sym_place;
            }
        }

        return nullptr;
    }

    // Image::~Image()
    // {}

    // Tool::~Tool()
    // {}
}
