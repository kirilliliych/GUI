#include "containerwindow.hpp"


ContainerWindow::ContainerWindow(int width,
                                 int height,
                                 const char *name)
  : Window(width, height, name),
    ContainerWidget(Rectangle{0, 0, width, height}, nullptr),
    background_color_(*DEFAULT_CONTAINERWIDGET_BACKGROUND_COLOR)
{
    const char *tex_name = get_texture_name(); 
    if (tex_name != nullptr)
    {
        has_texture_ = skin_->load_from_file(tex_name);
    }
}


void ContainerWindow::exit()
{
    exited.emit();
    exited_to_parent.emit(this);
}

void ContainerWindow::update_on_time()
{
    Event event{};
    while (poll_event(event))
    {
        handle_event(&event);
    }
}

void ContainerWindow::render(Surface *surface)
{
    assert(surface != nullptr);

    surface_->clear(0);
    if (has_texture_)
    {
        surface_->draw_sprite(Sprite{*skin_, Rectangle{0, 0, area_.get_width(), area_.get_height()}});
    }
    else
    {
        surface_->draw_rectangle(area_, background_color_);
    }

    return ContainerWidget::render(surface);
}

const char *ContainerWindow::get_texture_name() const
{
    return "skins/main_background.jpg";
}

EventHandlerState ContainerWindow::handle_event(const Event *event)
{
    assert(event != nullptr);

    if (event->type_ == EventType::Closed)
    {
        close();

        return EventHandlerState::Accepted;
    }

    return ContainerWidget::handle_event(event);
}

