#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include "desktop.hpp"
#include "../raytracer/scene.hpp"
#include "../utilities/color.hpp"
#include "../utilities/colorlibrary.hpp"
#include <chrono>


class Application : public SignalObject
{
public:
//-----------------------------------------------------------------------------
    Application()
    {
        desktop_ = new Desktop{};
        desktop_->set_event_manager(&main_event_manager_);
        desktop_->fill_desktop();
        connect<ContainerWindow>(desktop_, &ContainerWindow::exited, this, &Application::exit);

        std::cout << "Application: end of initialization" << std::endl;
    }

    ~Application()
    {
        delete desktop_;
    }

//---------------------------------------------------------------------------------
    int execute()
    {
        std::chrono::high_resolution_clock clock{};

        desktop_->open();
        while (desktop_->is_open())
        {
            Event event{};

            event.type_  = EventType::Paint;
            main_event_manager_.handle_event(&event);

            desktop_->redraw();

            event.timer_ = clock.now();
            event.type_  = EventType::TimerTicked;
            main_event_manager_.handle_event(&event);
        }

        return 0;
    }

    void exit()
    {
        desktop_->close();
    }

//---------------------------------------------------------------------------------
private:

    Desktop *desktop_ = nullptr;
    EventManager main_event_manager_{};
};


#endif