#include <queue>
#include <set>
#include "defaulttools.hpp"
#include "../widgets/painteditor/painteditor.hpp"


void draw_line(ImageSf *image, const Point2d &point1, const Point2d &point2, const Color &color,
               void (*mode)(ImageSf *, const Point2d &, const Color &, const int), const int radius)
{
    int dx     =  abs(point2.x - point1.x);
    int x_sign = point2.x > point1.x ? 1 : -1;
    int dy     = -abs(point2.y - point1.y);
    int y_sign = point2.y > point1.y ? 1 : -1;
    int err = dx + dy;

    int from_x = point1.x;
    int from_y = point1.y;
    while (Point2d{from_x, from_y} != point2)
    {
        if (mode == nullptr)
        {
            image->set_pixel(from_x, from_y, color);
        }
        else
        {
            mode(image, Point2d{from_x, from_y}, color, radius);
        }
        int err2 = 2 * err;

        if (err2 >= dy)
        {
            if (from_x == point2.x)
            {
                break;
            }
            err += dy;
            from_x += x_sign;
        }
        if (err2 <= dy)
        {
            if (from_y == point2.y)
            {
                break;
            }
            err += dx;
            from_y += y_sign;
        }
    }
    if (mode == nullptr)
    {
        image->set_pixel(from_x, from_y, color);
    }
    else
    {
        mode(image, Point2d{from_x, from_y}, color, radius);
    }
}

void draw_circle(ImageSf *image, const Point2d &center, const Color &color, const int radius)
{
    assert(radius > 0);

    Point2d smaller_coords{std::max(static_cast<int> (center.x - radius), 0),
                           std::max(static_cast<int> (center.y - radius), 0)
                          };
    Point2d bigger_coords {std::min(static_cast<int> (center.x + radius), image->get_width()  - 1),
                           std::min(static_cast<int> (center.y + radius), image->get_height() - 1)
                          };

    int r2 = radius * radius;
    for (int x = static_cast<int> (smaller_coords.x); x <= static_cast<int> (bigger_coords.x); ++x)
    {
        for (int y = static_cast<int> (smaller_coords.y); y <= static_cast<int> (bigger_coords.y); ++y)
        {
            int dx = abs(x - center.x);
            int dy = abs(y - center.y);
            int dx2 = dx * dx;
            int dy2 = dy * dy;
            if (dx2 + dy2 <= r2)
            {
                image->set_pixel(x, y, color);
            }
        }
    }       
}

double calc_color_norm(const Color &color1, const Color &color2)
{
    double norm = (color1.get_r() - color2.get_r()) * (color1.get_r() - color2.get_r()) +
                  (color1.get_g() - color2.get_g()) * (color1.get_g() - color2.get_g()) +
                  (color1.get_b() - color2.get_b()) * (color1.get_b() - color2.get_b()) +
                  (color1.get_a() - color2.get_a()) * (color1.get_a() - color2.get_a()) / 256;

    return norm;
}
//------------------------------------------------------------------------------------
Pencil::Pencil()
  : Tool()
{
    changed_canvas_ = true;
    requires_panel_ = false;
}

Pencil::~Pencil()
{}


void Pencil::on_mouse_pressed(const ToolAction &action)
{   
    action.image->set_pixel(action.point.x, action.point.y, PaintEditor::application_->basic_colors.foreground_color);
    if (action.shift)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.foreground_color);
    }
    prev_point_ = action.point;
    is_pressed_ = true;
}

void Pencil::on_mouse_released(const ToolAction &action)
{
    is_pressed_ = false;
}

void Pencil::on_mouse_moved(const ToolAction &action)
{
    if (is_pressed_)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.foreground_color);       // artifacts when cursor is out of canvas
        prev_point_ = action.point;
    }
}

void Pencil::on_mouse_left(const ToolAction &action)
{
    is_pressed_ = false;
}

void Pencil::on_timer_event(const ToolAction &action)
{}

const char *Pencil::get_texture_name() const
{
    return "skins/pencil.png";
}
//------------------------------------------------------------------------------------
RectFiller::RectFiller()
  : Tool()
{
    changed_canvas_ = false;
    requires_panel_ = false;
}

RectFiller::~RectFiller()
{}

void RectFiller::on_mouse_pressed(const ToolAction &action)
{
    if (registered_from_)
    {
        Point2d fill_to = action.point;
        Point2d smaller_coords{fill_from_.x > fill_to.x ? fill_to.x : fill_from_.x,
                               fill_from_.y > fill_to.y ? fill_to.y : fill_from_.y
                              };
        Point2d bigger_coords {fill_from_.x > fill_to.x ? fill_from_.x : fill_to.x,
                               fill_from_.y > fill_to.y ? fill_from_.y : fill_to.y
                              };
        for (int x = static_cast<int> (smaller_coords.x); x <= static_cast<int> (bigger_coords.x); ++x)
        {
            for (int y = static_cast<int> (smaller_coords.y); y <= static_cast<int> (bigger_coords.y); ++y)
            {
                action.image->set_pixel(x, y, PaintEditor::application_->basic_colors.foreground_color);
            }
        }

        registered_from_ = false;
        changed_canvas_  = true;
    }
    else
    {
        fill_from_ = action.point;
        registered_from_ = true;
        changed_canvas_  = false;
    }
}

void RectFiller::on_mouse_released(const ToolAction &action)
{}

void RectFiller::on_mouse_moved   (const ToolAction &action)
{}

void RectFiller::on_mouse_left    (const ToolAction &action)
{}

void RectFiller::on_timer_event   (const ToolAction &action)
{}

const char *RectFiller::get_texture_name() const
{
    return "skins/rectfiller.png";
}
//------------------------------------------------------------------------------------
Rect::Rect()
  : RectFiller(),
    thickness_(DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS)
{}

Rect::~Rect()
{}

void Rect::on_mouse_pressed(const ToolAction &action)
{
    if (registered_from_)
    {
        Point2d fill_to = action.point;
        Point2d smaller_coords{fill_from_.x > fill_to.x ? fill_to.x : fill_from_.x,
                               fill_from_.y > fill_to.y ? fill_to.y : fill_from_.y
                              };
        Point2d bigger_coords {fill_from_.x > fill_to.x ? fill_from_.x : fill_to.x,
                               fill_from_.y > fill_to.y ? fill_from_.y : fill_to.y
                              };
        for (int x = smaller_coords.x; x <= bigger_coords.x; ++x)
        {
            for (int y = smaller_coords.y; y <= bigger_coords.y; ++y)
            {
                if ((abs(x - smaller_coords.x) <= thickness_) || 
                    (abs(x - bigger_coords.x ) <= thickness_) ||
                    (abs(y - smaller_coords.y) <= thickness_) ||
                    (abs(y - bigger_coords.y ) <= thickness_))
                {
                    action.image->set_pixel(x, y, PaintEditor::application_->basic_colors.foreground_color);
                    
                }
            }
        }

        registered_from_ = false;
        changed_canvas_  = true;
    }
    else
    {
        fill_from_ = action.point;
        registered_from_ = true;
        changed_canvas_  = false;
    }
}

const char *Rect::get_texture_name() const
{
    return "skins/rect.png";
}
//------------------------------------------------------------------------------------
EllipseFiller::EllipseFiller()
  : Tool()
{
    changed_canvas_ = false;
    requires_panel_ = false;
}

EllipseFiller::~EllipseFiller()
{}

void EllipseFiller::on_mouse_pressed(const ToolAction &action)
{
    if (registered_from_)
    {
        Point2d fill_to = action.point;
        Point2d smaller_coords{fill_from_.x > fill_to.x ? fill_to.x : fill_from_.x,
                               fill_from_.y > fill_to.y ? fill_to.y : fill_from_.y
                              };
        Point2d bigger_coords {fill_from_.x > fill_to.x ? fill_from_.x : fill_to.x,
                               fill_from_.y > fill_to.y ? fill_from_.y : fill_to.y
                              };

        int xsa = (bigger_coords.x - smaller_coords.x) / 2;
        int ysa = (bigger_coords.y - smaller_coords.y) / 2;
        int xsa2 = xsa * xsa;
        int ysa2 = ysa * ysa;
        for (int x = smaller_coords.x; x <= bigger_coords.x; ++x)
        {
            for (int y = smaller_coords.y; y <= bigger_coords.y; ++y)
            {
                int dx = x - smaller_coords.x - xsa;
                int dy = y - smaller_coords.y - ysa;
                int dx2 = dx * dx;
                int dy2 = dy * dy;
                if (dx2 * ysa2 + dy2 * xsa2 <= xsa2 * ysa2)
                {
                    action.image->set_pixel(x, y, PaintEditor::application_->basic_colors.foreground_color);
                }
            }
        }

        registered_from_ = false;
        changed_canvas_  = true;
    }
    else
    {
        fill_from_ = action.point;
        registered_from_ = true;
        changed_canvas_  = false;
    }
}

void EllipseFiller::on_mouse_released(const ToolAction &action)
{}

void EllipseFiller::on_mouse_moved   (const ToolAction &action)
{}

void EllipseFiller::on_mouse_left    (const ToolAction &action)
{}

void EllipseFiller::on_timer_event   (const ToolAction &action)
{}

const char *EllipseFiller::get_texture_name() const
{
    return "skins/ellipsefiller.png";
}
//------------------------------------------------------------------------------------
Ellipse::Ellipse()
  : EllipseFiller(),
    thickness_(DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS)
{}

Ellipse::~Ellipse()
{}

void Ellipse::on_mouse_pressed(const ToolAction &action)
{
    if (registered_from_)
    {
        ImageSf image_copy = *action.image;
        EllipseFiller::on_mouse_pressed(action);

        Point2d fill_to = action.point;
        Point2d smaller_coords{fill_from_.x > fill_to.x ? fill_to.x : fill_from_.x,
                               fill_from_.y > fill_to.y ? fill_to.y : fill_from_.y
                              };
        Point2d bigger_coords {fill_from_.x > fill_to.x ? fill_from_.x : fill_to.x,
                               fill_from_.y > fill_to.y ? fill_from_.y : fill_to.y
                              };

        smaller_coords.x += thickness_;
        smaller_coords.y += thickness_;
        bigger_coords.x  -= thickness_;
        bigger_coords.y  -= thickness_;

        int xsa = (bigger_coords.x - smaller_coords.x) / 2;
        int ysa = (bigger_coords.y - smaller_coords.y) / 2;
        int xsa2 = xsa * xsa;
        int ysa2 = ysa * ysa;
        for (int x = smaller_coords.x; x <= bigger_coords.x; ++x)
        {
            for (int y = smaller_coords.y; y <= bigger_coords.y; ++y)
            {
                int dx = x - smaller_coords.x - xsa;
                int dy = y - smaller_coords.y - ysa;
                int dx2 = dx * dx;
                int dy2 = dy * dy;
                if (dx2 * ysa2 + dy2 * xsa2 <= xsa2 * ysa2)
                {
                    Color cur_pixel = image_copy.get_pixel(x, y);
                    action.image->set_pixel(x, y, cur_pixel);
                }
            }
        }

        registered_from_ = false;
        changed_canvas_  = true;
    }
    else
    {
        fill_from_ = action.point;
        registered_from_ = true;
        changed_canvas_  = false;
    }
}

const char *Ellipse::get_texture_name() const
{
    return "skins/ellipse.png";
}
//------------------------------------------------------------------------------------
Filler::Filler()
  : Tool()
{
    changed_canvas_ = true;
    requires_panel_ = false;
}

Filler::~Filler()
{}

void Filler::on_mouse_pressed(const ToolAction &action)
{
    Color clicked_pixel_color = action.image->get_pixel(action.point.x, action.point.y);
    std::queue<Point2d> queue{};
    std::set<Point2d> set;
    queue.push(action.point);
    set.insert(action.point);

    while (!queue.empty())
    {
        Point2d cur_point = queue.front();
        queue.pop();
        action.image->set_pixel(cur_point.x, cur_point.y, PaintEditor::application_->basic_colors.foreground_color);

        ++cur_point.x;
        if ((cur_point.x < (action.image->get_width())) && 
            (set.find(cur_point) == set.end())          && 
            calc_color_norm(clicked_pixel_color, action.image->get_pixel(cur_point.x, cur_point.y)) < max_color_diff_)
        {
            queue.push(cur_point);
            set.insert(cur_point);
        }

        cur_point.x -= 2;
        if ((cur_point.x >= 0)                 &&
            (set.find(cur_point) == set.end()) &&
            (calc_color_norm(clicked_pixel_color, action.image->get_pixel(cur_point.x, cur_point.y))) < max_color_diff_)
        {
            queue.push(cur_point);
            set.insert(cur_point);
        }

        ++cur_point.x;
        ++cur_point.y;
        if ((cur_point.y < action.image->get_height()) &&
            (set.find(cur_point) == set.end())         &&
            (calc_color_norm(clicked_pixel_color, action.image->get_pixel(cur_point.x, cur_point.y))) < max_color_diff_)
        {
            queue.push(cur_point);
            set.insert(cur_point);
        }

        cur_point.y -= 2;
        if ((cur_point.y >= 0)                 &&
            (set.find(cur_point) == set.end()) &&
            (calc_color_norm(clicked_pixel_color, action.image->get_pixel(cur_point.x, cur_point.y))) < max_color_diff_)
        {
            queue.push(cur_point);
            set.insert(cur_point);
        }
    }
}

void Filler::on_mouse_released(const ToolAction &action)
{}

void Filler::on_mouse_moved   (const ToolAction &action)
{}

void Filler::on_mouse_left    (const ToolAction &action)
{}

void Filler::on_timer_event   (const ToolAction &action)
{}

const char *Filler::get_texture_name() const
{
    return "skins/filler.png";
}
//------------------------------------------------------------------------------------
Pipette::Pipette()
  : Tool()
{
    changed_canvas_ = false;
    requires_panel_ = false;
}

Pipette::~Pipette()
{}

void Pipette::on_mouse_pressed(const ToolAction &action)
{
    if (action.ctrl)
    {
        PaintEditor::application_->basic_colors.background_color = action.image->get_pixel(action.point.x, action.point.y);
    }
    else
    {
        PaintEditor::application_->basic_colors.foreground_color = action.image->get_pixel(action.point.x, action.point.y);
    }
}
void Pipette::on_mouse_released(const ToolAction &action)
{}

void Pipette::on_mouse_moved   (const ToolAction &action)
{}

void Pipette::on_mouse_left    (const ToolAction &action)
{}

void Pipette::on_timer_event   (const ToolAction &action)
{}

const char *Pipette::get_texture_name() const
{
    return "skins/pipette.png";
}
//------------------------------------------------------------------------------------
Brush::Brush()
  : Tool(),
    thickness_(DEFAULT_TOOL_BRUSH_RADIUS)
{
    changed_canvas_ = true;
    requires_panel_ = false;
}

Brush::~Brush()
{}

void Brush::on_mouse_pressed(const ToolAction &action)
{
    draw_circle(action.image, action.point, PaintEditor::application_->basic_colors.foreground_color, thickness_);
    if (action.shift)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.foreground_color,
                  draw_circle, thickness_);
    }
    prev_point_ = action.point;
    is_pressed_ = true;
}

void Brush::on_mouse_released(const ToolAction &action)
{
    is_pressed_ = false;
}

void Brush::on_mouse_moved(const ToolAction &action)
{
    if (is_pressed_)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.foreground_color,
                  draw_circle, thickness_);
        prev_point_ = action.point;
    }
}

void Brush::on_mouse_left(const ToolAction &action)
{
    is_pressed_ = false;
}

void Brush::on_timer_event(const ToolAction &action)
{}

const char *Brush::get_texture_name() const
{
    return "skins/brush.png";
}
//-------------------------------------------------------------------
Eraser::Eraser()
  : Brush()
{}

Eraser::~Eraser()
{}

void Eraser::on_mouse_pressed(const ToolAction &action)
{
    draw_circle(action.image, action.point, PaintEditor::application_->basic_colors.background_color, thickness_);
    if (action.shift)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.background_color,
                draw_circle, thickness_);
    }
    prev_point_ = action.point;
    is_pressed_ = true;
}

void Eraser::on_mouse_moved(const ToolAction &action)
{
    if (is_pressed_)
    {
        draw_line(action.image, prev_point_, action.point, PaintEditor::application_->basic_colors.background_color,
                  draw_circle, thickness_);
        prev_point_ = action.point;
    }
}

const char *Eraser::get_texture_name() const
{
    return "skins/eraser.png";
}
