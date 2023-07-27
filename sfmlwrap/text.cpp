#include "text.hpp"


Text::Text(){}

Text::Text(const char *text, const Rectangle &area, Widget *parent,
           const char *font_name)
  : Widget(area, parent),
    color_(*DEFAULT_TEXT_COLOR),
    hovered_color_(*DEFAULT_TEXT_HOVERING_COLOR)
{
    //std::cout << "Text constructor" << std::endl;
   
    set_string(text);
    set_font(font_name);
    set_character_size(area.get_height());
    set_outline_color(BLACK);
    set_outline_thickness();
    //text_.setOutlineColor(sf::Color::Red);
    text_.setStyle(sf::Text::Regular);
    center_text();                                      // now all text is centrated!

    id_ = 3;
}

Text::~Text(){}


void Text::set_position(int x, int y)
{
    text_.setPosition(x, y);
}

void Text::set_position(const Point2d &position)
{
    set_position(position.x,
                 position.y);
}

void Text::set_character_size(size_t size)
{
    text_.setCharacterSize(size);
}

void Text::set_string(const char *string)
{
    assert(string != nullptr);

    text_.setString(string);
}

void Text::set_color(const Color &color)
{
    text_.setFillColor(sf::Color(color.get_uint32_color()));
}

void Text::set_outline_color(const Color &color)
{
    text_.setOutlineColor(sf::Color{color.get_uint32_color()});
}

void Text::set_outline_thickness(float thickness)
{
    text_.setOutlineThickness(thickness);
}

void Text::set_font(const char *font_name)
{
    assert(font_name != nullptr);

    assert(font_.loadFromFile(font_name) && "ERROR: font was not found\n");
    text_.setFont(font_);
}

void Text::center_text()
{
    size_t text_len = get_length();
    Point2d last_character_pos = find_character_pos_(text_len);
    float text_pos_x = text_.getPosition().x;
    size_t cur_character_size  = get_character_size();
    while (1)
    {
        if (last_character_pos.x - text_pos_x < area_.get_width())
        {
            break;
        }

        --cur_character_size;
        set_character_size(cur_character_size);
        last_character_pos = find_character_pos_(text_len);
    }
    float half_of_real_text_size = (last_character_pos.x - text_pos_x) / 2;
    
    float text_beginning_pos = (area_.get_x() + area_.get_width() / 2) - half_of_real_text_size;
    text_.setPosition(sf::Vector2f{text_beginning_pos - area_.get_x(), 0});
}


Color Text::get_color() const
{
    return color_;
}

Rectangle Text::get_area() const
{
    sf::FloatRect rectangle = text_.getGlobalBounds();

    return Rectangle{static_cast<int> (rectangle.left),
                     static_cast<int> (rectangle.top),
                     static_cast<int> (rectangle.width),
                     static_cast<int> (rectangle.height)
                    };
}

size_t Text::get_character_size() const
{
    return text_.getCharacterSize();
}

size_t Text::get_length() const
{
    return text_.getString().getSize();
}

void Text::set_hovered(bool is_hovered)
{
    is_hovered_ = is_hovered;
    requires_repaint_ = true;
}


EventHandlerState Text::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }
    
    Color color_to_draw = is_hovered_ ? hovered_color_ : color_;
    
    set_color(color_to_draw);
    surface_->clear(0);
    surface_->draw_text(*this);
    
    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}


Point2d Text::find_character_pos_(size_t index) const
{
    sf::Vector2f result = text_.findCharacterPos(index);

    return Point2d{result.x, result.y};
}
