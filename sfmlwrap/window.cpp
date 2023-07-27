#include "window.hpp"


Window::Window(int width, 
               int height,
               const char *name)
  : width_ (width),
    height_(height),
    name_  (const_cast<char *> (name))
{   std::cout << "init window" << std::endl;
    assert(name != nullptr);
}

void Window::create(int width,
                    int height,
                    const char *name)
{
    assert(name != nullptr);

    window_.create(sf::VideoMode(width, height), name);
    //init_surface();

    // std::cout << "surface_whited_create\n";
}

void Window::open()
{
    assert(width_  > 0);
    assert(height_ > 0);
    assert(name_ != nullptr);

    create(width_, height_, name_);
    // update_surface();
}

bool Window::is_open()
{
    return window_.isOpen();
}

void Window::close()
{
    window_.close();
}   

void Window::clear(const Color &color)
{
    sf::Color sf_color{color.get_uint32_color()};

    window_.clear(sf_color);
}

void Window::update()
{
    window_.display();
}


void Window::draw_surface(const Surface *surface)
{
    assert(surface != nullptr);

    Sprite spr{surface->get_texture()};
    window_.draw(spr.sprite_);
}


MouseButton recognise_mouse_button(sf::Mouse::Button mouse_button)
{
    switch (mouse_button)
    {
        case sf::Mouse::Left:
        {
            return MouseButton::Left;
        }

        case sf::Mouse::Right:
        {
            return MouseButton::Right;
        }

        case sf::Mouse::Middle:
        {
            return MouseButton::Wheel;
        }

        default:
        {
            return MouseButton::Additional;
        }
    }
}

bool Window::poll_event(Event &event)
{
    sf::Event sfml_event;
    if (window_.pollEvent(sfml_event))
    {  
        switch (sfml_event.type)
        {
            case sf::Event::KeyPressed:
            {
                event.type_   = EventType::KeyPressed;
                event.kedata_ = *(reinterpret_cast<KeyEventData *> (&sfml_event.key));

                break;
            }

            case sf::Event::KeyReleased:
            {
                event.type_   = EventType::KeyReleased;
                event.kedata_ = *(reinterpret_cast<KeyEventData *> (&sfml_event.key)); 

                break;
            }

            case sf::Event::Closed:
            {
                event.type_ = EventType::Closed;

                break;
            }

            case sf::Event::MouseWheelScrolled:
            {
                event.type_             = EventType::MouseWheelScrolled;
                event.mwedata_.delta    = sfml_event.mouseWheel.delta;
                event.mwedata_.position = {sfml_event.mouseWheel.x,
                                           sfml_event.mouseWheel.y
                                          };
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                event.type_             = EventType::MouseButtonPressed;
                event.mbedata_.button   = recognise_mouse_button(sfml_event.mouseButton.button);
                event.mbedata_.position = {sfml_event.mouseButton.x,
                                           sfml_event.mouseButton.y
                                          }; 
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                event.type_ = EventType::MouseButtonReleased;
                event.mbedata_.button   = recognise_mouse_button(sfml_event.mouseButton.button);
                event.mbedata_.position = {sfml_event.mouseButton.x,
                                           sfml_event.mouseButton.y
                                          };
                break;
            }

            case sf::Event::MouseMoved:
            {
                event.type_ = EventType::MouseMoved;
                event.mmedata_.position = {sfml_event.mouseMove.x,
                                           sfml_event.mouseMove.y,
                                          };

                break;
            }

            default:
            {
                std::cout << "Attempt to process unused event" << std::endl;
                return false;
            }
        }

        return true;
    }

    return false;
}

