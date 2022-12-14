#ifndef MENUBAR_HPP
#define MENUBAR_HPP


#include <climits>
#include "button.hpp"
#include "containerwidget.hpp"
#include "widget.hpp"


static const Color *DEFAULT_MENUBAR_TEXT_COLOR = &BLACK;


static const int DEFAULT_MENUBAR_BUTTON_WIDTH  = 70;


class Menu;
class MenuButton;

class Menubar : public ContainerWidget
{
public:

    class Menu;
    class MenuButton;
//-------------------------------------------------------------
    Menubar(const Rectangle &rectangle,          Color background_color = *DEFAULT_WIDGET_COLOR, ContainerWidget *parent = nullptr);
    Menubar(int x, int y, int width, int height, Color background_color = *DEFAULT_WIDGET_COLOR, ContainerWidget *parent = nullptr);

    ~Menubar();
//-------------------------------------------------------------
    void add_new_menu(const char *name);
    const std::vector<Menu *> &get_menus() const;

    void set_text_color(const Color &color);
    void set_font(const char *font_name = DEFAULT_FONT_NAME);
    void set_character_size(size_t size);
    void set_unified_character_size(size_t size);
    void center_text();

    size_t get_unified_character_size() const;
    
    bool contains(const Point2d &position) override;

    EventHandlerState on_paint_event(const Event *event) override;

    Menu *operator [](size_t index) const;    
//-------------------------------------------------------------
private:
//public:
    void set_active_menu_(Menu *menu);
    void set_no_active_menu_();

//-----------------------Variables-----------------------
    Color text_color_  = *DEFAULT_MENUBAR_TEXT_COLOR;
    Color color_       = *DEFAULT_WIDGET_COLOR;
    Menu *active_menu_ = nullptr;
    std::vector<Menu *> menus_{};
//-------------------------------------------------------
};


class Menubar::Menu : public Widget
{
public:
//--------------------------------------------------------------
    Menu(const Rectangle &rectangle, const char *name, Widget *parent = nullptr);

    ~Menu();
//--------------------------------------------------------------
    const std::vector<MenuButton *> &get_buttons() const;
    MenuButton *get_head() const;
    void add_new_button(const char *text = "New button");

    void set_text_color(const Color &color);
    void set_font(const char *font_name = DEFAULT_FONT_NAME);
    void set_character_size(size_t size);
    size_t get_unified_character_size() const;
    void show();
    void hide();
    void switch_mode();
    void render(Surface *surface) override;

    bool contains(const Point2d &point) override;


    EventHandlerState on_paint_event(const Event *) override;

//---------------------Variables--------------------------------
private:
//public:
    bool is_enabled_ = false;

    MenuButton *head_ = nullptr;
    std::vector<MenuButton *> buttons_{};

public:

    Signal<Menu *> enabled;
    Signal<>       disabled;
//--------------------------------------------------------------
};


class Menubar::MenuButton : public Button
{
public:
//--------------------------------------------------------------
    MenuButton(const Rectangle &rectangle, const char *text, Widget *parent = nullptr);

    ~MenuButton();
//--------------------------------------------------------------
    void set_head();
    void set_is_active(bool is_active);
    size_t get_character_size() const;
    
    EventHandlerState on_paint_event(const Event *event) override;

private:
public:
    bool is_active_ = false;
    bool is_head_   = false;
};


#endif