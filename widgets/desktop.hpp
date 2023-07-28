#ifndef DESKTOP_HPP
#define DESKTOP_HPP


#include <cassert>
#include <stdlib.h>
#include <time.h>
#include "button.hpp"
#include "../utilities/colorlibrary.hpp"
#include "containerwindow.hpp"
#include "menubar.hpp"
#include "painteditor/painteditor.hpp"
#include "../raytracer/raytracer.hpp"
#include "../raytracer/scene.hpp"
#include "../sfmlwrap/window.hpp"
#include "scrollbar.hpp"


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
        menubar_area_   = {0, 0, area_.get_width(), 25};
        raytracer_area_ = {1050, 50, 520, 400};
        pe_area_        = {0, 
                           menubar_area_.get_y() + menubar_area_.get_height() + 1,
                           width,
                           height - menubar_area_.get_height() - menubar_area_.get_y()
                          };
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
        pe_        = new PaintEditor(pe_area_,      this);
        raytracer_ = new Raytracer(raytracer_area_, this);
        menubar_   = new Menubar(menubar_area_, *DEFAULT_WIDGET_COLOR, this);
        
        // cringe_button_ = new ActionButton{Rectangle{100, 800, 60, 30}, "Ur stupid", GREEN,
        //                                                                           *DEFAULT_BUTTON_HOVERING_COLOR,
        //                                                                           *DEFAULT_TEXT_COLOR,
        //                                                                           this};
        // cringe_button_->mouse_entered.connect<Desktop>(this, &Desktop::meme);
        
        init_menubar();
    }

    void init_menubar()
    {
        menubar_->add_new_menu("File");
        menubar_->get_menus()[0]->add_new_button("Exit");
        menubar_->get_menus()[0]->get_buttons()[0]->clicked.connect<ContainerWindow>(this, &ContainerWindow::exit);

        menubar_->add_new_menu("Edit");

        menubar_->add_new_menu("Tools");

        menubar_->add_new_menu("Raytracer");
        menubar_->get_menus()[3]->add_new_button("show");
        menubar_->get_menus()[3]->add_new_button("hide");
        menubar_->get_menus()[3]->get_buttons()[0]->clicked.connect<Raytracer>(raytracer_, &Widget::show);
        menubar_->get_menus()[3]->get_buttons()[1]->clicked.connect<Raytracer>(raytracer_, &Widget::hide);

        menubar_->add_new_menu("PaintEditor");
        menubar_->get_menus()[4]->add_new_button("show");
        menubar_->get_menus()[4]->add_new_button("hide");
        menubar_->get_menus()[4]->get_buttons()[0]->clicked.connect<PaintEditor>(pe_, &Widget::show);
        menubar_->get_menus()[4]->get_buttons()[1]->clicked.connect<PaintEditor>(pe_, &Widget::hide); 

        menubar_->set_text_color(*DEFAULT_MENUBAR_TEXT_COLOR);
        size_t unified_character_size = menubar_->get_unified_character_size();
        menubar_->set_unified_character_size(unified_character_size);
        menubar_->center_text();
    }

    void redraw()
    {
        // forgot clearing desktop

        main_surface_->clear();
        render(main_surface_);
        main_surface_->update();
        draw_surface(main_surface_);

        update();
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
        } while ((!get_area().contains(Point2d{x + area.get_x() + area.get_width(), y + area.get_y()}))  ||
                 (!get_area().contains(Point2d{x + area.get_x() - area.get_width(), y + area.get_y()}))  ||
                 (!get_area().contains(Point2d{x + area.get_x(), y + area.get_y() + area.get_height()})) ||
                 (!get_area().contains(Point2d{x + area.get_x(), y + area.get_y() - area.get_height()})));
    
        cringe_button_->move(Point2d{x, y});
    }
//--------------------------------------------------------------------------------
private:

    Surface *main_surface_        = nullptr;

    Menubar *menubar_             = nullptr;
    ActionButton *cringe_button_  = nullptr;
    Raytracer *raytracer_         = nullptr;
    PaintEditor *pe_              = nullptr;
    
    Rectangle menubar_area_{};
    Rectangle raytracer_area_{};
    Rectangle pe_area_{};
};

#endif
