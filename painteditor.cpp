#include "painteditor.hpp"


PaintEditor *PaintEditor::application_ = nullptr;


PaintEditor::PaintEditor(const Rectangle &rectangle, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent)
{
    assert(application_ == nullptr);
    application_ = this;

    create_default_tools_();
    //here import plugins

    event_manager_.identif = 1000;
    
    Rectangle canvas_area = rectangle;
    canvas_area.set_width(canvas_area.get_width() * 0.75);
    canvas_= new Canvas(canvas_area, this);

    Rectangle side_panel_area = {canvas_area.get_width() + 1, 
                                 canvas_area.get_y(),
                                 rectangle.get_width() - canvas_area.get_width() - 1,
                                 canvas_area.get_height()
                                };
    side_panel_ = new SidePanel(side_panel_area, tools_, this);

    assert(cur_tool_ != nullptr);
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
        //tool_changed.emit(another_tool);
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
}

void PaintEditor::set_background_color(const Color &color)
{
    basic_colors.background_color = color;
}

const std::vector<Tool *> &PaintEditor::get_tools() const
{
    return tools_;
}

void PaintEditor::add_tool(Tool *tool)
{
    tools_.push_back(tool);
}

void PaintEditor::create_default_tools_()
{
    add_tool(new Pencil());
    add_tool(new RectFiller());
    add_tool(new Rect());
    add_tool(new EllipseFiller());
    add_tool(new Ellipse());
    add_tool(new Filler());
    add_tool(new Pipette());
}
