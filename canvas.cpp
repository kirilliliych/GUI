#include "canvas.hpp"
#include "painteditor.hpp"


Canvas::Canvas(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent)
{
    image_texture_ = new Texture();
    image_         = new Image(area_.get_width(),
                               area_.get_height());
    
    cur_action_.image = image_;
    
    assert(image_->is_created());

    id_ = 25;
}

Canvas::~Canvas()
{
    delete image_; 
    delete image_texture_;
    delete_remembered_images_();
}


EventHandlerState Canvas::on_paint_event(const Event *event)
{
    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    if ((image_ != nullptr) && (image_->is_created()))
    {
        image_texture_->load_from_image(*image_);

        Sprite temp_sprite(*image_texture_);
        temp_sprite.set_position(image_start_);
        surface_->clear(0);
        surface_->draw_sprite(temp_sprite);
    }

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}

EventHandlerState Canvas::on_mouse_button_pressed_event(const Event *event)
{
    if (!image_contains_(event->mbedata_.position))
    {
        return EventHandlerState::Skipped;
    }
    if (event->mbedata_.button == MouseButton::Left)
    { 
        if (PaintEditor::application_->get_current_tool() == nullptr)
        {
            return EventHandlerState::Accepted;
        }
        cur_action_.point = event->mbedata_.position - area_.get_top_left_corner_coords();
        if (PaintEditor::application_->get_current_tool()->is_to_change())
        {
            save_cur_image_();
        }
        PaintEditor::application_->get_current_tool()->on_mouse_pressed(cur_action_);

        requires_repaint_ = true;
    }

    return Widget::on_mouse_button_pressed_event(event);
}

EventHandlerState Canvas::on_mouse_button_released_event(const Event *event)
{
    if (!image_contains_(event->mbedata_.position))
    {
        return EventHandlerState::Skipped;
    }
    if (event->mbedata_.button == MouseButton::Left)
    {
        if (PaintEditor::application_->get_current_tool() == nullptr)
        {
            return EventHandlerState::Accepted;
        }
        cur_action_.point = event->mbedata_.position - area_.get_top_left_corner_coords();
        PaintEditor::application_->get_current_tool()->on_mouse_released(cur_action_);

        requires_repaint_ = true;
    }

    return Widget::on_mouse_button_pressed_event(event);
}

EventHandlerState Canvas::on_mouse_moved_event(const Event *event)
{
    if (!image_contains_(event->mmedata_.position))
    {
        return EventHandlerState::Skipped;
    }
    if (PaintEditor::application_->get_current_tool() == nullptr)
    {
        return EventHandlerState::Accepted;
    }
    cur_action_.point = event->mmedata_.position - area_.get_top_left_corner_coords();
    PaintEditor::application_->get_current_tool()->on_mouse_moved(cur_action_);

    requires_repaint_ = true;

    return Widget::on_mouse_moved_event(event);
}

EventHandlerState Canvas::on_key_pressed_event(const Event *event)
{
    switch (event->kedata_.key_code)
    {
        case KeyboardKey::LShift:
        {
            cur_action_.shift = true;

            return EventHandlerState::Accepted;
        }

        case KeyboardKey::LControl:
        {
            cur_action_.ctrl = true;

            return EventHandlerState::Accepted;
        }

        case KeyboardKey::LAlt:
        {
            cur_action_.alt = true;

            return EventHandlerState::Accepted;
        }

        default:
        {
            return Widget::on_key_pressed_event(event);
        }
    }
}

EventHandlerState Canvas::on_key_released_event(const Event *event)
{
    switch (event->kedata_.key_code)
    {
        case KeyboardKey::LShift:
        {
            cur_action_.shift = false;

            return EventHandlerState::Accepted;
        }

        case KeyboardKey::LControl:
        {
            cur_action_.ctrl = false;

            return EventHandlerState::Accepted;
        }

        case KeyboardKey::LAlt:
        {
            cur_action_.alt = false;

            return EventHandlerState::Accepted;
        }

        case KeyboardKey::Z:
        {
            if (cur_action_.ctrl)
            {
                undo_();
            }

            return EventHandlerState::Accepted;
        }

        default:
        {
            return Widget::on_key_released_event(event);
        }
    }
}

EventHandlerState Canvas::on_lost_focus_event(const Event *event)
{
    cur_action_.shift = false;
    cur_action_.ctrl  = false;
    cur_action_.alt   = false;

    return EventHandlerState::Accepted;
}

void Canvas::move_image(const Point2d &offset)
{
    image_start_ += offset;
    requires_repaint_ = true;
}


bool Canvas::image_contains_(Point2d point)
{
    point -= area_.get_top_left_corner_coords();
    
    return Rectangle{0, 0, image_->get_width() - 1, image_->get_height() - 1}.contains(point);
}

void Canvas::save_cur_image_()
{
    if (remembered_images_.size() == MAX_UNDO_DEPTH)
    {
        Image *popped_out = remembered_images_.front();
        delete popped_out;
        remembered_images_.pop_front();
    }
    Image *new_remembered_image = new Image{};
    *new_remembered_image = *image_;
    remembered_images_.push_back(new_remembered_image);
    //std::cout << "saved_image\n";
}

void Canvas::undo_()
{
    if (remembered_images_.size() > 0)
    {
        Image *popped_out = remembered_images_.back();
        *image_ = *popped_out;
        delete popped_out;
        remembered_images_.pop_back();
        //std::cout << "did undo\n";
    }

    requires_repaint_ = true;
}


void Canvas::delete_remembered_images_()
{
    while (!remembered_images_.empty())
    {
        delete remembered_images_.back();
        remembered_images_.pop_back();
    }
}
