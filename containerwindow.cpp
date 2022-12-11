#include "containerwindow.hpp"


ContainerWindow::ContainerWindow(int width,
                                 int height,
                                 const char *name,
                                 Color background_color)
  : Window(width, height, name),
    ContainerWidget(Rectangle{0, 0, width, height}, nullptr),
    background_color_(background_color)
{}


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
    surface_->draw_rectangle(area_, background_color_);

    return ContainerWidget::render(surface);
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
