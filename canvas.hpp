#ifndef CANVAS_HPP
#define CANVAS_HPP


#include <deque>
#include "color.hpp"
#include "colorlibrary.hpp"
#include "point2d.hpp"
#include "rectangle.hpp"
#include "tool.hpp"


const int MAX_UNDO_DEPTH = 25;


class Canvas : public Widget
{
public:
//------------------------------------------------------------------------
    Canvas(const Rectangle &rectangle, Widget *parent = nullptr);

    ~Canvas();
//------------------------------------------------------------------------
    void move_image(const Point2d &offset);

    EventHandlerState on_paint_event                (const Event *event) override;
    EventHandlerState on_mouse_button_pressed_event (const Event *event) override;
    EventHandlerState on_mouse_button_released_event(const Event *event) override;
    EventHandlerState on_mouse_moved_event          (const Event *event) override;
    EventHandlerState on_key_pressed_event          (const Event *event) override;
    EventHandlerState on_key_released_event         (const Event *event) override;
    EventHandlerState on_lost_focus_event           (const Event *event) override;

private:

    bool image_contains_(Point2d point);

    void save_cur_image_();

    void undo_();

    void delete_remembered_images_();
//------------------------------Variables---------------------------------
    std::deque<Image *> remembered_images_{};

    Image *image_           = nullptr;    
    Texture *image_texture_ = nullptr;
    Point2d image_start_{};

    bool requires_repaint_ = true;

    ToolAction cur_action_{};
//------------------------------------------------------------------------
};


#endif
