#ifndef DEFAULT_TOOLS_HPP
#define DEFAULT_TOOLS_HPP


#include "../sfmlwrap/image.hpp"
#include "../geometry/point2d.hpp"
#include "../toolsplugins/tool.hpp"


const int DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS = 5;
const int DEFAULT_TOOL_BRUSH_RADIUS           = 5;


void draw_line  (ImageSf *image, const Point2d &point1, const Point2d &point2, const Color &color,
                 void (*mode)(ImageSf *, const Point2d &, const Color &, const int) = nullptr,
                 const int radius = 1);
void draw_circle(ImageSf *image, const Point2d &center, const Color &color, const int radius);

double calc_color_norm(const Color &color1, const Color &color2);


class Pencil : public Tool
{
public:
//-------------------------------------------------------------------
    Pencil();

    virtual ~Pencil();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
//---------------------------Variables-------------------------------
private:

    bool is_pressed_    = false;

    Point2d prev_point_{};
};


class RectFiller : public Tool
{
public:
//-------------------------------------------------------------------
    RectFiller();

    virtual ~RectFiller();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
//---------------------------Variables-------------------------------
protected:

    Point2d fill_from_{};
    bool registered_from_ = false;
};


class Rect : public RectFiller
{
public:
//-------------------------------------------------------------------
    Rect();

    virtual ~Rect();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed(const ToolAction &action) override final;
    virtual const char *get_texture_name() const override final;
//---------------------------Variables-------------------------------
private:

    int thickness_ = DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS;
};


class EllipseFiller : public Tool
{
public:
//-------------------------------------------------------------------
    EllipseFiller();

    virtual ~EllipseFiller();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
//---------------------------Variables-------------------------------
protected:

    Point2d fill_from_{};
    bool registered_from_ = false;
};


class Ellipse : public EllipseFiller
{
public:
//-------------------------------------------------------------------
    Ellipse();

    virtual ~Ellipse();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed(const ToolAction &action) override final;
    virtual const char *get_texture_name() const override final;
//---------------------------Variables-------------------------------
private:

    int thickness_ = DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS;
};


class Filler : public Tool
{
public:
//-------------------------------------------------------------------
    Filler();

    virtual ~Filler();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
//---------------------------Variables-------------------------------
private:

    int max_color_diff_ = 5;
};


class Pipette : public Tool
{
public:
//-------------------------------------------------------------------
    Pipette();

    virtual ~Pipette();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
};


class Brush : public Tool
{
public:
//-------------------------------------------------------------------
    Brush();

    virtual ~Brush();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed (const ToolAction &action) override;
    virtual void on_mouse_released(const ToolAction &action) override;
    virtual void on_mouse_moved   (const ToolAction &action) override;
    virtual void on_mouse_left    (const ToolAction &action) override;
    virtual void on_timer_event   (const ToolAction &action) override;
    virtual const char *get_texture_name() const override;
//---------------------------Variables-------------------------------
protected:
    
    bool is_pressed_ = false;

    int thickness_ = DEFAULT_TOOL_BRUSH_RADIUS;
    Point2d prev_point_{};
};


class Eraser : public Brush
{
public:
//-------------------------------------------------------------------
    Eraser();

    virtual ~Eraser();
//-------------------------------------------------------------------
    virtual void on_mouse_pressed(const ToolAction &action) override final;
    virtual void on_mouse_moved  (const ToolAction &action) override final;
    virtual const char *get_texture_name() const override final;
};


#endif
