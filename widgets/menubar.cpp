#include "menubar.hpp"


Menubar::Menubar(const Rectangle &rectangle,
                 Color background_color, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{}


Menubar::Menubar(int x, int y, int width, int height, 
                 Color background_color, ContainerWidget *parent)
  : Menubar(Rectangle{x, y, width, height}, background_color, parent)
{}

Menubar::~Menubar()
{
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        delete menus_[menu_index];
    }
}



void Menubar::add_new_menu(const char *name)
{
    assert(name != nullptr);

    int new_menu_x_coord = menus_.empty() ? area_.get_x() : menus_.back()->get_area().get_top_right_corner_coords().x;
    Rectangle new_menu_area{new_menu_x_coord, area_.get_y(), DEFAULT_MENUBAR_BUTTON_WIDTH, area_.get_height()};
    Menu *new_menu = new Menu(new_menu_area, name, this);
    
    menus_.push_back(new_menu);
    new_menu->enabled.connect (this, &Menubar::set_active_menu_);
    new_menu->disabled.connect(this, &Menubar::set_no_active_menu_);
    new_menu->set_text_color(text_color_);
}

const std::vector<Menubar::Menu *> &Menubar::get_menus() const
{
    return menus_;
}


void Menubar::set_text_color(const Color &color)
{
    text_color_ = color;
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        menus_[menu_index]->set_text_color(color);
    }
}

void Menubar::set_font(const char *font_name)
{
    assert(font_name != nullptr);

    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        menus_[menu_index]->set_font(font_name);
    }
}

void Menubar::set_character_size(size_t size)
{
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        menus_[menu_index]->set_character_size(size);
    }
}

void Menubar::set_unified_character_size(size_t size)
{
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        menus_[menu_index]->set_character_size(size);
    }
}

void Menubar::center_text()
{
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        Menu *cur_menu = menus_[menu_index];
        cur_menu->get_head()->center_text();
        for (int button_index = 0; button_index < cur_menu->get_buttons().size(); ++button_index)
        {
            cur_menu->get_buttons()[button_index]->center_text();
        }
    }
}

size_t Menubar::get_unified_character_size() const
{
    size_t max_satisfying_character_size = INT_MAX;
    for (int menu_index = 0; menu_index < menus_.size(); ++menu_index)
    {
        size_t menu_unified_character_size = menus_[menu_index]->get_unified_character_size();
        if (max_satisfying_character_size > menu_unified_character_size)
        {
            max_satisfying_character_size = menu_unified_character_size;
        }
    }

    return max_satisfying_character_size;
}



bool Menubar::contains(const Point2d &position)
{    
    return ContainerWidget::contains(position) || (active_menu_ != nullptr ? active_menu_->contains(position) :
                                                                             false);
}


EventHandlerState Menubar::on_paint_event(const Event *event)
{
    surface_->clear(0);
    surface_->draw_rectangle(area_, color_);

    return ContainerWidget::on_paint_event(event);
}


Menubar::Menu *Menubar::operator [](size_t index) const
{
    return menus_[index];
}


void Menubar::set_active_menu_(Menu *menu)
{
    if ((active_menu_ != nullptr) && (active_menu_ != menu))
    {
        active_menu_->hide();
    }

    active_menu_ = menu;
}

void Menubar::set_no_active_menu_()
{
    active_menu_ = nullptr;
}
//----------------------------------------------------------------------------------------------------------------
Menubar::Menu::Menu(const Rectangle &rectangle, const char *name, Widget *parent)
  : Widget(rectangle, parent)
{
    assert(name != nullptr);

    head_ = new MenuButton(rectangle, name, this);
    head_->clicked.connect<Menu>(this, &Menu::switch_mode);
    head_->set_head();
}

Menubar::Menu::~Menu()
{
    delete head_;
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        delete buttons_[button_index];
    }
}

Menubar::MenuButton *Menubar::Menu::get_head() const
{
    return head_;
}

const std::vector<Menubar::MenuButton *> &Menubar::Menu::get_buttons() const
{
    return buttons_;
}


void Menubar::Menu::add_new_button(const char *text)
{
    assert(text != nullptr);

    int buttons_size = buttons_.size();
    Rectangle new_button_area{area_.get_x(), area_.get_y() + area_.get_height() * (buttons_size + 1),
                                             area_.get_width(),
                                             area_.get_height()};
    
    MenuButton *new_button = new MenuButton(new_button_area, text, this);
    new_button->clicked.connect(this, &Menu::hide);
    buttons_.push_back(new_button);
}


void Menubar::Menu::set_text_color(const Color &color)
{   
    head_->set_text_color(color);
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        buttons_[button_index]->set_text_color(color);
    }
}

void Menubar::Menu::set_font(const char *font_name)
{
    head_->set_font(font_name);
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        buttons_[button_index]->set_font(font_name);
    }
}

void Menubar::Menu::set_character_size(size_t size)
{
    head_->set_character_size(size);
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        buttons_[button_index]->set_character_size(size);
    }
}

size_t Menubar::Menu::get_unified_character_size() const
{
    size_t result = head_->get_character_size();
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        size_t cur_button_character_size = buttons_[button_index]->get_character_size();
        if (result > cur_button_character_size)
        {
            result = cur_button_character_size;
        }
    }

    return result;
}

void Menubar::Menu::show()
{
    if (!is_enabled_)
    {
        enabled.emit(this);
    }
    is_enabled_ = true;
    head_->set_is_active(true);
}

void Menubar::Menu::hide()
{
    is_enabled_ = false;
    disabled.emit();
    head_->set_is_active(false);
}

void Menubar::Menu::switch_mode()
{
    is_enabled_ ? hide() : show();
}

void Menubar::Menu::render(Surface *surface)
{
    assert(surface != nullptr);

    head_->render(surface);
    
    if (is_enabled_)
    {
        for (int button_index = 0; button_index < buttons_.size(); ++button_index)
        {
            buttons_[button_index]->render(surface);
        }
    }
}

bool Menubar::Menu::contains(const Point2d &position)
{
    if (!is_enabled_)
    {
        return false;
    }

    return ((position.x >= area_.get_x())                                               &&
            (position.x <= area_.get_x() + area_.get_width())                           &&
            (position.y >= area_.get_y())                                               &&
            (position.y <= area_.get_y() + area_.get_height() * (buttons_.size() + 1)));
}


EventHandlerState Menubar::Menu::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    head_->on_paint_event(event);
    for (int button_index = 0; button_index < buttons_.size(); ++button_index)
    {
        buttons_[button_index]->on_paint_event(event);
    }

    return EventHandlerState::Accepted;
}
//---------------------------------------------------------------------------------------------------
Menubar::MenuButton::MenuButton(const Rectangle &rectangle, const char *text, Widget *parent)
    : Button(rectangle, text, parent)
{}

Menubar::MenuButton::~MenuButton()
{}

void Menubar::MenuButton::set_head()
{
    is_head_ = true;
}

void Menubar::MenuButton::set_is_active(bool is_active)
{
    is_active_        = is_active;
    requires_repaint_ = true;
}

size_t Menubar::MenuButton::get_character_size() const
{
    return text_.get_character_size();
}


EventHandlerState Menubar::MenuButton::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    const Color *to_paint = is_pressed_ ? DEFAULT_BUTTON_PRESSED_COLOR : is_hovered_ ? DEFAULT_BUTTON_HOVERING_COLOR : DEFAULT_WIDGET_COLOR;
    surface_->clear(0);
    surface_->draw_rectangle(area_, *to_paint);
    
    draw_frame_();

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}

