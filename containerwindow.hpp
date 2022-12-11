#ifndef CONTAINERWINDOW_HPP
#define CONTAINERWINDOW_HPP


#include "containerwidget.hpp"
#include "window.hpp"



class ContainerWindow : public Window, public ContainerWidget
{
public:
//-----------------------------------------------------
    ContainerWindow(int width  = STANDARD_WINDOW_WIDTH,
                    int height = STANDARD_WINDOW_HEIGHT,
                    const char *name = STANDARD_WINDOW_NAME,
                    Color background_color = *DEFAULT_CONTAINERWIDGET_BACKGROUND_COLOR);

    virtual ~ContainerWindow()
    {}
//-----------------------------------------------------
    void exit();

    void update_on_time() override;

    void render(Surface *surface);   

    virtual EventHandlerState handle_event(const Event *event) override;

//-------------------Variables-------------------------
    Color background_color_ = *DEFAULT_CONTAINERWIDGET_BACKGROUND_COLOR;

    Signal<>                  exited;
    Signal<ContainerWindow *> exited_to_parent;
};

#endif