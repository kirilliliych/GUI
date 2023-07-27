#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../utilities/color.hpp"
#include "../utilities/colorlibrary.hpp"
#include "../widgets/widget.hpp"
#include "../widgets/containerwidget.hpp"


static const int STANDARD_WINDOW_WIDTH   = 1600;/*1024;*/
static const int STANDARD_WINDOW_HEIGHT  = 900;/*768;*/


static const char *STANDARD_WINDOW_NAME = "Gui";

class Window
{   
public:
//-----------------------------------------------------------------
    Window(int width   = STANDARD_WINDOW_WIDTH, 
           int height  = STANDARD_WINDOW_HEIGHT, 
           const char *name = STANDARD_WINDOW_NAME);

    virtual ~Window(){}


    Window(const Window &window)             = delete;
    Window &operator =(const Window &window) = delete;
//-----------------------------------------------------------------
    
    void create(int width   = STANDARD_WINDOW_WIDTH,
                int height  = STANDARD_WINDOW_HEIGHT,
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

//-------------------------------------------------------------------------------------
protected:
//-------------------------Variables---------------------------------------------------
    int width_  = 0;
    int height_ = 0;

    char *name_ = const_cast<char *> (STANDARD_WINDOW_NAME);

    sf::RenderWindow window_{};
};


#endif