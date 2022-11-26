#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "color.hpp"
#include "colorlibrary.hpp"
#include "widget.hpp"
#include "containerwidget.hpp"


static const unsigned STANDARD_WINDOW_WIDTH   = 1024;
static const unsigned STANDARD_WINDOW_HEIGHT  = 768;


static const char *STANDARD_WINDOW_NAME = "Gui";


class Window
{   
public:
//-----------------------------------------------------------------
    Window(unsigned width   = STANDARD_WINDOW_WIDTH, 
           unsigned height  = STANDARD_WINDOW_HEIGHT, 
           const char *name = STANDARD_WINDOW_NAME);

    virtual ~Window(){}


    Window(const Window &window)             = delete;
    Window &operator =(const Window &window) = delete;
//-----------------------------------------------------------------
    
    void create(unsigned width   = STANDARD_WINDOW_WIDTH,
                unsigned height  = STANDARD_WINDOW_HEIGHT,
                const char *name = STANDARD_WINDOW_NAME);

    void open();

    bool is_open();

    void close();

    void clear(const Color &color);

    void update();
   
    int get_width() const
    {
        return width_;
    }
    int get_height() const
    {
        return height_;  
    }

    void draw_surface(const Surface *surface);


    bool poll_event(Event &event);

    //void update_on_time() override;


    //EventHandlerState on_paint_event(const Event *event) override;

//-------------------------------------------------------------------------------------
protected:
public:

    //void output_certain_widget_form_(Color color = Color{WHITE}) override;

//-------------------------Variables---------------------------------------------------
    unsigned width_  = 0;
    unsigned height_ = 0;

    char *name_ = const_cast<char *> (STANDARD_WINDOW_NAME);

    sf::RenderWindow window_{};
};


#endif