#include "painteditor.hpp"
#include "plugins.hpp"


PaintEditor *PaintEditor::application_ = nullptr;


PaintEditor::PaintEditor(const Rectangle &rectangle, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{
    assert(application_ == nullptr);
    application_ = this;

    create_default_tools_();
    import_plugins(plugins_);
    
    Rectangle canvas_area = rectangle;
    canvas_area.set_x(canvas_area.get_x() + 25);
    canvas_area.set_y(canvas_area.get_y() + 25);
    canvas_area.set_width(600);
    canvas_area.set_height(400);
    canvas_= new Canvas(canvas_area, this);

    Rectangle side_panel_area = {rectangle.get_x() + rectangle.get_width()  - 250, 
                                 rectangle.get_y() + rectangle.get_height() - 300,
                                 250,
                                 300
                                };
    side_panel_ = new SidePanel(side_panel_area, tools_, this);
}

PaintEditor::~PaintEditor()
{
    for (int tool_index = 0; tool_index < tools_.size(); ++tool_index)
    {
        delete tools_[tool_index];
    }

    delete canvas_;
    delete side_panel_;
}


void PaintEditor::set_current_tool(Tool *another_tool)
{
    if (cur_tool_ != another_tool)
    {
        cur_tool_ = another_tool;  
    }
}

Tool *PaintEditor::get_current_tool() const
{
    return cur_tool_;
}

void PaintEditor::set_foreground_color(const Color &color)
{
    basic_colors.foreground_color = color;
    context.fg_color = color.get_uint32_color();
}

void PaintEditor::set_background_color(const Color &color)
{
    basic_colors.background_color = color;
    context.bg_color = color.get_uint32_color();
}

const std::vector<Tool *> &PaintEditor::get_tools() const
{
    return tools_;
}

void PaintEditor::add_tool(Tool *tool)
{
    std::cout << __FILE__ << " add_tool_called" << std::endl;
    tools_.push_back(tool);
}

void PaintEditor::create_default_tools_()
{
    add_tool(new Pencil());
    add_tool(new Brush());
    add_tool(new Eraser());
    add_tool(new RectFiller());
    add_tool(new Rect());
    add_tool(new EllipseFiller());
    add_tool(new Ellipse());
    add_tool(new Filler());
    add_tool(new Pipette());
}
