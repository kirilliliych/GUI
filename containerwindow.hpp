#ifndef CONTAINERWINDOW_HPP
#define CONTAINERWINDOW_HPP


#include "window.hpp"


static const Color *DEFAULT_BACKGROUND_COLOR = &WHITE;


class ContainerWindow : public Window, public ContainerWidget
{
public:
//-----------------------------------------------------
    ContainerWindow(int width  = STANDARD_WINDOW_WIDTH,
                    int height = STANDARD_WINDOW_HEIGHT,
                    const char *name = STANDARD_WINDOW_NAME);

    ~ContainerWindow() override
    {}
//-----------------------------------------------------
    void exit();

    void update_on_time() override;

    void render(Surface *surface);    

    virtual EventHandlerState handle_event(const Event *event) override;

//-------------------Variables-------------------------
    Signal<> exited;
    Signal<ContainerWindow *> exitedp;
};

#endif