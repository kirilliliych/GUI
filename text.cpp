#include "text.hpp"


Text::Text(){}

Text::Text(const char *text, const Rectangle &area, Widget *parent,
           const char *font_file_name)
  : Widget(area, parent)
{
    std::cout << "text constructor" << std::endl;
    assert(font_.loadFromFile(font_file_name) && "Font was not loaded\n");
    
    text_.setFont(font_);
    text_.setString(text);
    text_.setStyle(sf::Text::Regular);
    text_.setCharacterSize(area.get_height());
    size_t text_length = strlen(text);
    centering_text_(text_length);

    id_ = 3;
    std::cout << "id: " << id_ << std::endl;
    // std::cout << "class Text init_surface" << std::endl;

    //init_surface();
}

Text::~Text(){}


void Text::set_position(int x, int y)
{
    text_.setPosition(x, y);
}

void Text::set_position(const Point2d &position)
{
    set_position(position.get_x(),
                 position.get_y());
}

void Text::set_character_size(unsigned int size)
{
    text_.setCharacterSize(size);
}

void Text::set_string(const char *string)
{
    text_.setString(string);
}

void Text::set_color(const Color &color)
{
    color_ = color;
    text_.setFillColor(sf::Color(color.get_uint32_color()));
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

Point2d Text::find_character_pos(size_t index) const
{
    sf::Vector2f result = text_.findCharacterPos(index);

    return Point2d{result.x, result.y};
}

unsigned Text::get_character_size() const
{
    return text_.getCharacterSize();
}


EventHandlerState Text::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    // output_certain_widget_form_();
    surface_->draw_text(*this);
    // sprite_.load_from_surface(surface_);
    // sprite_.set_position(area_.get_x(), area_.get_y());

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}


// void Text::output_certain_widget_form_(Color color)
// {
//     surface_->clear(0);
//     surface_->draw_text(*this);
//     surface_->update();
// }

void Text::centering_text_(size_t text_size)
{
    if (parent_ != nullptr)
    {
        Point2d last_character_pos = find_character_pos(text_size);
        double half_of_real_text_len = last_character_pos.get_x() / 2;
        
        double text_beginning_pos = (area_.get_x() + area_.get_width() / 2) - half_of_real_text_len;
        area_.set_x(text_beginning_pos);
    }
}



