#ifndef DEFAULT_TOOLS_HPP
#define DEFAULT_TOOLS_HPP


#include "image.hpp"
#include "point2d.hpp"
#include "tool.hpp"


const int DEFAULT_TOOL_HOLLOW_SHAPE_THICKNESS = 5;


void draw_line(Image *image, const Point2d &point1, const Point2d &point2, const Color &color);

double calc_color_norm(const Color &color1, const Color &color2);


class Pencil : public Tool
{
public:
//-------------------------------------------------------------------
    Pencil();

    ~Pencil();
//-------------------------------------------------------------------
    void on_mouse_pressed (const ToolAction &action) override;
    void on_mouse_released(const ToolAction &action) override;
    void on_mouse_moved   (const ToolAction &action) override;
    const char *get_texture_name() const override;
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

    ~Rect();
//-------------------------------------------------------------------
    void on_mouse_pressed(const ToolAction &action) override;
    const char *get_texture_name() const override;
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

    ~Ellipse();
//-------------------------------------------------------------------
    void on_mouse_pressed(const ToolAction &action) override;
    const char *get_texture_name() const override;
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
    virtual const char *get_texture_name() const override;
};





#endif
