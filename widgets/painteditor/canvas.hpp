#ifndef CANVAS_HPP
#define CANVAS_HPP


#include <deque>
#include "../../utilities/color.hpp"
#include "../../utilities/colorlibrary.hpp"
#include "../../geometry/point2d.hpp"
#include "../../sfmlwrap/rectangle.hpp"
#include "../../toolsplugins/tool.hpp"


const int MAX_UNDO_REDO_DEPTH = 25;
const int SHOWN_AND_IMAGE_SIZE_DELTA_X = 320;
const int SHOWN_AND_IMAGE_SIZE_DELTA_Y = 180;


class Canvas : public Widget
{
public:
//------------------------------------------------------------------------
    Canvas(const Rectangle &rectangle, Widget *parent = nullptr);

    virtual ~Canvas() override;
//------------------------------------------------------------------------
    void set_image_start_x(int x);
    void set_image_start_y(int y);
    void move_image(const Point2d &offset);

    const char *get_texture_name() const override;

    EventHandlerState on_paint_event                (const Event *event) override;
    EventHandlerState on_mouse_button_pressed_event (const Event *event) override;
    EventHandlerState on_mouse_button_released_event(const Event *event) override;
    EventHandlerState on_mouse_moved_event          (const Event *event) override;
    EventHandlerState on_key_pressed_event          (const Event *event) override;
    EventHandlerState on_key_released_event         (const Event *event) override;
    EventHandlerState on_lost_focus_event           (const Event *event) override;
    EventHandlerState on_mouse_left_event           (const Event *event) override;

private:

    bool image_contains_(Point2d point);

    void save_cur_image_();

    void undo_();

    void redo_();

    void delete_remembered_images_();
//------------------------------Variables---------------------------------
    std::deque<ImageSf *> remembered_images_{};
    int cur_image_remembered_index_ = -1;
    
    ImageSf *image_ = nullptr;    
    Texture *image_texture_ = nullptr;
    Point2d image_start_{};

    bool requires_repaint_ = true;
    bool undo_redo_flag_   = false;

    ToolAction cur_action_{};
//------------------------------------------------------------------------

};


#endif
