#include "canvas.hpp"
#include "painteditor.hpp"


Canvas::Canvas(const Rectangle &rectangle, Widget *parent)
  : Widget(rectangle, parent)
{
    image_texture_ = new Texture();
    image_         = new ImageSf(rectangle.get_width()  + SHOWN_AND_IMAGE_SIZE_DELTA_X,
                                 rectangle.get_height() + SHOWN_AND_IMAGE_SIZE_DELTA_Y);
    cur_action_.image = image_;

    const char *tex_name = get_texture_name(); 
    if (tex_name != nullptr)
    {
        has_texture_ = skin_->load_from_file(tex_name);
        if (has_texture_)
        {
            ImageSf temp_image{};
            temp_image.load_from_file(tex_name);
            image_->copy(temp_image, 0, 0);
        }
    }

    save_cur_image_();
}

Canvas::~Canvas()
{
    delete image_; 
    delete image_texture_;
    delete_remembered_images_();
}


void Canvas::set_image_start_x(int x)
{
    image_start_.x = -x;

    requires_repaint_ = true;
}

void Canvas::set_image_start_y(int y)
{
    image_start_.y = -y;

    requires_repaint_ = true;
}

void Canvas::move_image(const Point2d &offset)
{
    image_start_ += offset;

    requires_repaint_ = true;
}

const char *Canvas::get_texture_name() const
{
    return "skins/canvas_background.jpg";
}

EventHandlerState Canvas::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    if ((image_ != nullptr) && (image_->is_created()))
    {
        image_texture_->load_from_image(*image_);

        Sprite temp_sprite(*image_texture_);
        temp_sprite.set_position(image_start_);
        surface_->clear(WHITE);
        surface_->draw_sprite(temp_sprite);

        draw_frame_();
    }

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}

EventHandlerState Canvas::on_mouse_button_pressed_event(const Event *event)
{
    assert(event != nullptr);

    if (!image_contains_(event->mbedata_.position))
    {
        std::cout << "Canvas::on_mouse_button_pressed_event failed because image does not contain" << std::endl;
        return EventHandlerState::Skipped;
    }
    if (event->mbedata_.button == MouseButton::Left)
    { 
        std::cout << "Canvas_on_mouse_button_pressed_event success" << std::endl;
        std::cout << "point_position: x " << event->mbedata_.position.x << " y " << event->mbedata_.position.y << std::endl;
        if (PaintEditor::application_->get_current_tool() == nullptr)
        {
            return EventHandlerState::Accepted;
        }
        cur_action_.point = event->mbedata_.position - area_.get_top_left_corner_coords() - image_start_;
        std::cout << "cur_action point: x " << cur_action_.point.x << " y " << cur_action_.point.y << std::endl; 
        PaintEditor::application_->get_current_tool()->on_mouse_pressed(cur_action_);

        requires_repaint_ = true;
    }

    return Widget::on_mouse_button_pressed_event(event);
}

EventHandlerState Canvas::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

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
        cur_action_.point = event->mbedata_.position - area_.get_top_left_corner_coords() - image_start_;
        PaintEditor::application_->get_current_tool()->on_mouse_released(cur_action_);
        if (PaintEditor::application_->get_current_tool()->changed_canvas())
        {
            if (undo_redo_flag_)
            {
                for (int i = cur_image_remembered_index_ + 1; i < remembered_images_.size(); ++i)
                {
                    delete remembered_images_[i];
                }
                remembered_images_.resize(cur_image_remembered_index_ + 1);
                undo_redo_flag_ = false;
            }
            save_cur_image_();
        }

        requires_repaint_ = true;
    }

    return Widget::on_mouse_button_pressed_event(event);
}

EventHandlerState Canvas::on_mouse_moved_event(const Event *event)
{
    assert(event != nullptr);

    if (!image_contains_(event->mmedata_.position))
    {
        return EventHandlerState::Skipped;
    }
    if (PaintEditor::application_->get_current_tool() == nullptr)
    {
        return EventHandlerState::Accepted;
    }
    cur_action_.point = event->mmedata_.position - area_.get_top_left_corner_coords() - image_start_;
    PaintEditor::application_->get_current_tool()->on_mouse_moved(cur_action_);

    requires_repaint_ = true;

    return Widget::on_mouse_moved_event(event);
}

EventHandlerState Canvas::on_key_pressed_event(const Event *event)
{
    assert(event != nullptr);

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
    assert(event != nullptr);

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

        case KeyboardKey::X:
        {
            if (cur_action_.ctrl)
            {
                redo_();
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
    assert(event != nullptr);

    cur_action_.shift = false;
    cur_action_.ctrl  = false;
    cur_action_.alt   = false;

    return EventHandlerState::Accepted;
}

EventHandlerState Canvas::on_mouse_left_event(const Event *event)
{
    assert(event != nullptr);

    PaintEditor::application_->get_current_tool()->on_mouse_left(cur_action_);

    return Widget::on_mouse_left_event(event);
}


bool Canvas::image_contains_(Point2d point)
{
    point -= area_.get_top_left_corner_coords();
    
    return Rectangle{0, 0, image_->get_width() - 1, image_->get_height() - 1}.contains(point);
}

void Canvas::save_cur_image_()
{
    if (remembered_images_.size() == MAX_UNDO_REDO_DEPTH)
    {
        ImageSf *popped_out = remembered_images_.front();
        delete popped_out;
        remembered_images_.pop_front();
    }
    ImageSf *new_remembered_image = new ImageSf{};
    *new_remembered_image = *image_;
    remembered_images_.push_back(new_remembered_image);

    cur_image_remembered_index_ = remembered_images_.size() - 1;
}

void Canvas::undo_()
{
    if (cur_image_remembered_index_ > 0)
    {
        --cur_image_remembered_index_;

        *image_ = *remembered_images_[cur_image_remembered_index_];
    }
    requires_repaint_ = true;
    undo_redo_flag_   = true;
}

void Canvas::redo_()
{
    if (cur_image_remembered_index_ < remembered_images_.size() - 1)
    {
        ++cur_image_remembered_index_;

        *image_ = *remembered_images_[cur_image_remembered_index_];
    }
    requires_repaint_ = true;
    undo_redo_flag_   = true;
}

void Canvas::delete_remembered_images_()
{
    while (!remembered_images_.empty())
    {
        delete remembered_images_.back();
        remembered_images_.pop_back();
    }
}
