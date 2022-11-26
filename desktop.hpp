#ifndef DESKTOP_HPP
#define DESKTOP_HPP


#include <cassert>
#include "button.hpp"
#include "containerwindow.hpp"
#include "menubar.hpp"
#include "raytracer.hpp"
#include "scene.hpp"
#include "window.hpp"


const int RAYTRACER_WINDOW_WIDTH  = 600;
const int RAYTRACER_WINDOW_HEIGHT = 400;


class Desktop final : public ContainerWindow
{
public:
//--------------------------------------------------------------------------------

    Desktop(int width   = STANDARD_WINDOW_WIDTH, 
            int height  = STANDARD_WINDOW_HEIGHT, 
            const char *name = STANDARD_WINDOW_NAME)
      : ContainerWindow(width, height, name),
        main_surface(new Surface{width, height})
    {
        std::cout << "init desktop" << std::endl;
        menubar = new Menubar{Rectangle{0, 0, width, 80}, LIGHT_GREY, this};

        exit_button = new ActionButton{Rectangle{900, 30, 60, 30}, "Exit", *DEFAULT_BUTTON_COLOR,
                                                                           *DEFAULT_BUTTON_HOVERING_COLOR,
                                                                           *DEFAULT_TEXT_COLOR,
                                                                            menubar};
        connect<Button, ContainerWindow>(exit_button, &Button::clicked, this, &ContainerWindow::exit);   

        no_text_button = new ActionButton{Rectangle{50, 30, 80, 30}, "Menu", *DEFAULT_BUTTON_COLOR, 
                                                                             *DEFAULT_BUTTON_HOVERING_COLOR,
                                                                             *DEFAULT_TEXT_COLOR,
                                                                              menubar};

        raytracer = new Raytracer{Rectangle{30, 150, RAYTRACER_WINDOW_WIDTH, RAYTRACER_WINDOW_HEIGHT}, this};

        //bebra = new Text{"bebraaaa", Rectangle{500, 500, 60, 30}, this};
    }

    ~Desktop()
    {
        delete main_surface;
        delete menubar;
        delete exit_button;
        delete raytracer;
        delete no_text_button;
        //delete bebra;
    }
//--------------------------------------------------------------------------------
    void redraw()
    {
        //clear();

        //std::cout << "NEW REDRAW " << std::endl;
        main_surface->clear();
        render(main_surface);
        main_surface->update();
        draw_surface(main_surface);

        update();
    }
//--------------------------------------------------------------------------------
private:
public:
    Surface *main_surface        = nullptr;

    Menubar *menubar             = nullptr;
    ActionButton *exit_button    = nullptr;
    ActionButton *no_text_button = nullptr; 
    Raytracer *raytracer         = nullptr;

    Text *bebra = nullptr;

    //implementation order
    //
    //3) menubar 
    //4) scrollbar
    //5) toolbar, canvas
    //6) plugin



//  no chance
    //) serialization
    //) advertisement // no chance, optional
    //) skins         // no chance? 
};

#endif
