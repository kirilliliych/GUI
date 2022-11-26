#ifndef TEXT_HPP
#define TEXT_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "color.hpp"
#include "point2d.hpp"
#include "rectangle.hpp"
#include "widget.hpp"


static const char *DEFAULT_FONT_NAME = "fonts/CamingoMono-Regular.ttf";

static const Color *DEFAULT_TEXT_COLOR = &YELLOW;


class Surface;


class Text : public Widget
{
    friend Surface;

public:
//---------------------------------------------------------------------
    Text();

    Text(const char *text, const Rectangle &area, Widget *parent = nullptr,
         const char *font_file_name = DEFAULT_FONT_NAME);

    ~Text();
//---------------------------------------------------------------------

    void set_position(int x, int y);

    void set_position(const Point2d &position);

    void set_character_size(unsigned int size);

    void set_string(const char *string);

    void set_color(const Color &color);

    Color get_color() const;
    
    Rectangle get_area() const;

    Point2d find_character_pos(size_t index) const;

    unsigned get_character_size() const;


    EventHandlerState on_paint_event(const Event *event) override;
//---------------------------------------------------------------------
private:
public:

    void centering_text_(size_t text_size);

    //void output_certain_widget_form_(Color color = DEFAULT_TEXT_COLOR);

//---------------------------Variables---------------------------------
    sf::Font font_{};
    sf::Text text_{};
    Color    color_{};
//---------------------------------------------------------------------
};


#endif