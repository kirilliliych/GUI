#ifndef TEXT_HPP
#define TEXT_HPP


#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../utilities/color.hpp"
#include "../geometry/point2d.hpp"
#include "../sfmlwrap/rectangle.hpp"
#include "../widgets/widget.hpp"


static const char *DEFAULT_FONT_NAME = "fonts/CamingoMono-Regular.ttf";

static const Color *DEFAULT_TEXT_COLOR          = &BLACK;
static const Color *DEFAULT_TEXT_HOVERING_COLOR = &WHITE;
static const Color *DEFAULT_OUTLINE_COLOR       = &BLACK;

static const float DEFAULT_OUTLINE_THICKNESS = 0.0;


class Surface;


class Text : public Widget
{
    friend Surface;

public:
//---------------------------------------------------------------------
    Text();

    Text(const char *text, const Rectangle &area, Widget *parent = nullptr,
         const char *font_name = DEFAULT_FONT_NAME);

    ~Text();
//---------------------------------------------------------------------

    void set_position(int x, int y);

    void set_position(const Point2d &position);

    void set_character_size(size_t size);

    void set_string(const char *string);

    void set_color(const Color &color);

    void set_outline_color(const Color &color);

    void set_outline_thickness(float thickness = DEFAULT_OUTLINE_THICKNESS);

    void set_font(const char *font_name = DEFAULT_FONT_NAME);

    void center_text();

    Color get_color() const;
    
    Rectangle get_area() const;

    size_t get_character_size() const;

    size_t get_length() const;

    void set_hovered(bool is_hovered);

    EventHandlerState on_paint_event(const Event *event) override;
//---------------------------------------------------------------------
private:
public:

    Point2d find_character_pos_(size_t index) const;

//---------------------------Variables---------------------------------
    bool is_hovered_ = false;

    sf::Font font_{};
    sf::Text text_{};
    Color    color_{};
    Color    hovered_color_{};

public:

    bool requires_repaint_ = true;
//---------------------------------------------------------------------
};


#endif