#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include "desktop.hpp"
#include "scene.hpp"
#include "color.hpp"
#include "colorlibrary.hpp"


class Application : public SignalObject
{
public:
//----------------------------------------------------
    Application()
    {
        desktop_ = new Desktop{};
        desktop_->set_event_manager(&main_event_manager_);
        connect<ContainerWindow>(desktop_, &ContainerWindow::exited, this, &Application::exit);
        std::cout << "successfully set main_event_manager: id 228" << std::endl;
        main_event_manager_.identif_ = 228;
    }

    ~Application()
    {
        delete desktop_;
    }

//----------------------------------------------------

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

            // std::cout << "before main handler" << std::endl;
            main_event_manager_.handle_event(&event);
            // std::cout << "after main handler" << std::endl;
        }
        // std::cout << "correct exit from cycle\n";

        return 0;
    }


    void exit()
    {
        desktop_->close();
    }

//----------------------------------------------------
private:

    Desktop *desktop_ = nullptr;
    EventManager main_event_manager_{};
};


#endif