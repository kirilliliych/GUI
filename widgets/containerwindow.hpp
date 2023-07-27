#ifndef CONTAINERWINDOW_HPP
#define CONTAINERWINDOW_HPP


#include "containerwidget.hpp"
#include "../sfmlwrap/window.hpp"


static const Color *DEFAULT_CONTAINERWIDGET_BACKGROUND_COLOR = &SEA_GREEN;

class ContainerWindow : public Window, public ContainerWidget
{
public:
//-----------------------------------------------------
    ContainerWindow(int width  = STANDARD_WINDOW_WIDTH,
                    int height = STANDARD_WINDOW_HEIGHT,
                    const char *name = STANDARD_WINDOW_NAME);

    virtual ~ContainerWindow()
    {}
//-----------------------------------------------------
    void exit();

    void update_on_time() override;

    void render(Surface *surface) override;   

    const char *get_texture_name() const override;

    virtual EventHandlerState handle_event(const Event *event) override;
//-------------------Variables-------------------------
    Color background_color_ = *DEFAULT_CONTAINERWIDGET_BACKGROUND_COLOR;

    Signal<>                  exited;
    Signal<ContainerWindow *> exited_to_parent;
};

#endif