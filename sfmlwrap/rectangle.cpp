#include "rectangle.hpp"


sf::IntRect to_SFML_IntRect(const Rectangle &rectangle)
{
    return sf::IntRect{rectangle.get_x(),
                       rectangle.get_y(),
                       rectangle.get_width(),
                       rectangle.get_height()
                      };
}
