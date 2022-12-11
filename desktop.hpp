#ifndef DESKTOP_HPP
#define DESKTOP_HPP


#include <cassert>
#include <stdlib.h>
#include <time.h>
#include "button.hpp"
#include "colorlibrary.hpp"
#include "containerwindow.hpp"
#include "menubar.hpp"
#include "painteditor.hpp"
#include "raytracer.hpp"
#include "scene.hpp"
#include "window.hpp"


const int RAYTRACER_WINDOW_WIDTH  = 400;
const int RAYTRACER_WINDOW_HEIGHT = 300;


class Desktop final : public ContainerWindow
{
public:
//--------------------------------------------------------------------------------
    Desktop(int width  = STANDARD_WINDOW_WIDTH, 
            int height = STANDARD_WINDOW_HEIGHT, 
            const char *name = STANDARD_WINDOW_NAME)
      : ContainerWindow(width, height, name),
        main_surface_(new Surface{width, height})
    {
        //event_manager_.identif = 150;
    }

    ~Desktop()
    {
        delete main_surface_;
        delete menubar_;
        delete cringe_button_;
        delete raytracer_;
        delete pe_;
    }
//--------------------------------------------------------------------------------
    void fill_desktop()
    {
        raytracer_ = new Raytracer(Rectangle{0, 51, RAYTRACER_WINDOW_WIDTH, RAYTRACER_WINDOW_HEIGHT}, this);
        menubar_ = new Menubar(Rectangle{0, 0, area_.get_width(), 50}, *DEFAULT_MENUBAR_COLOR, this);
        pe_ = new PaintEditor(Rectangle{0, 
                                        raytracer_->get_area().get_height() + raytracer_->get_area().get_y() + menubar_->get_area().get_height(),
                                        area_.get_width(),
                                        area_.get_height() - menubar_->get_area().get_height() - raytracer_->get_area().get_height() - 2},
                                        this);
        
        cringe_button_ = new ActionButton{Rectangle{900, 200, 60, 30}, "Ur gay", GREEN,
                                                                                *DEFAULT_BUTTON_HOVERING_COLOR,
                                                                                *DEFAULT_TEXT_COLOR,
                                                                                 this};
        cringe_button_->mouse_entered.connect<Desktop>(this, &Desktop::meme);
        cringe_button_->clicked.connect<Desktop>(this, &Desktop::meme_change_text);

        init_menubar();
    }

    void init_menubar()
    {
        menubar_->add_new_menu("Lorem");
        menubar_->get_menus()[0]->add_new_button("ipsum");
        menubar_->get_menus()[0]->add_new_button("dolor");
        menubar_->get_menus()[0]->add_new_button("exit");
        menubar_->get_menus()[0]->get_buttons()[2]->clicked.connect<ContainerWindow>(this, &ContainerWindow::exit);

        menubar_->add_new_menu("Raytracer");
        menubar_->get_menus()[1]->add_new_button("show");
        menubar_->get_menus()[1]->add_new_button("hide");
        menubar_->get_menus()[1]->get_buttons()[0]->clicked.connect<Raytracer>(raytracer_, &Widget::show);
        menubar_->get_menus()[1]->get_buttons()[1]->clicked.connect<Raytracer>(raytracer_, &Widget::hide);

        menubar_->add_new_menu("Canvas");
        menubar_->get_menus()[2]->add_new_button("show");
        menubar_->get_menus()[2]->add_new_button("hide");
        menubar_->get_menus()[2]->get_buttons()[0]->clicked.connect<Canvas>(pe_->canvas_, &Widget::show);
        menubar_->get_menus()[2]->get_buttons()[1]->clicked.connect<Canvas>(pe_->canvas_, &Widget::hide);
        
        menubar_->add_new_menu("Aboba");
        menubar_->get_menus()[3]->get_head()->clicked.connect<Desktop>(this, &Desktop::aboba); 

        menubar_->set_text_color(*DEFAULT_MENUBAR_TEXT_COLOR);
        size_t unified_character_size = menubar_->get_unified_character_size();
        menubar_->set_unified_character_size(unified_character_size);
        menubar_->center_text();
    }

    void redraw()
    {
        main_surface_->clear();
        render(main_surface_);
        main_surface_->update();
        draw_surface(main_surface_);

        update();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void aboba()                                                                                // also joke
    {
        system("./Hack/sfml-app");
    }

    void meme()                                                                                 // just a joke
    {
        srand(time(NULL));
        Rectangle area = cringe_button_->get_area();
        int x = 0;
        int y = 0;
        do 
        {
            x = rand() % ((int) (STANDARD_WINDOW_WIDTH  * 0.9) + 10) - STANDARD_WINDOW_WIDTH  * 0.45;
            y = rand() % ((int) (STANDARD_WINDOW_HEIGHT * 0.9) + 10) - STANDARD_WINDOW_HEIGHT * 0.45;
            std::cout << "x " << x + area.get_x() << " y " << area.get_y() << std::endl;
        } while ((!get_area().contains(Point2d{x + area.get_x() + area.get_width(), y + area.get_y()}))  ||
                 (!get_area().contains(Point2d{x + area.get_x() - area.get_width(), y + area.get_y()}))  ||
                 (!get_area().contains(Point2d{x + area.get_x(), y + area.get_y() + area.get_height()})) ||
                 (!get_area().contains(Point2d{x + area.get_x(), y + area.get_y() - area.get_height()})));
    
        cringe_button_->move(Point2d{x, y});
    }
    void meme_change_text()
    {
        cringe_button_->set_text("Fuck u");
    }
//--------------------------------------------------------------------------------
private:
//public:
    Surface *main_surface_        = nullptr;

    Menubar *menubar_             = nullptr;
    ActionButton *cringe_button_  = nullptr;
    Raytracer *raytracer_         = nullptr;
    PaintEditor *pe_              = nullptr;
};

#endif
