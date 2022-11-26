#include "containerwindow.hpp"


ContainerWindow::ContainerWindow(int width,
                                 int height,
                                 const char *name)
  : Window(width, height, name),
    ContainerWidget(Rectangle{0, 0, width, height})
{}


void ContainerWindow::exit()
{
    std::cout << "containerwindow::close " << std::endl;
    exited.emit();
    std::cout << "containerwindow::afteremit1" << std::endl;
    //closedp.emit(this);
    std::cout << "containerwindow::afteremit2" << std::endl;
}

void ContainerWindow::update_on_time()
{
    //std::cout << "update_on_time called" << std::endl;
    Event event{};
    while (poll_event(event))
    {
        //std::cout << "update_on_time: before handling" << std::endl;
        handle_event(&event);
        //std::cout << "update_on_time: after handling" << std::endl; 
    }
}

void ContainerWindow::render(Surface *surface)
{
    assert(surface != nullptr);

    surface->clear(*DEFAULT_BACKGROUND_COLOR);                                          // background

    return ContainerWidget::render(surface);
}


EventHandlerState ContainerWindow::handle_event(const Event *event)
{
    assert(event != nullptr);

    if (event->type_ == EventType::Closed)
    {  //std::cout << "CLOSING CONTAINER_WINDOW AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        close();

        return EventHandlerState::Accepted;
    }

    return ContainerWidget::handle_event(event);
}
