#include "toolpanel.hpp"


ToolPanel::ToolPanel(const Rectangle &rectangle, const std::vector<Tool *> &tools, ContainerWidget *parent)
  : ContainerWidget(rectangle, parent),
    tools_(tools)
{
    connect(this, &ToolPanel::selected_tool_changed, PaintEditor::application_, &PaintEditor::set_current_tool);

    for (int tool_index = 0; tool_index < tools.size(); ++tool_index)
    {
        add_tool_button_(tools[tool_index]);
    }
}

ToolPanel::~ToolPanel()
{
    for (int tool_button_index = 0; tool_button_index < buttons_.size(); ++tool_button_index)
    {
        delete buttons_[tool_button_index];
    }
}


Tool *ToolPanel::get_selected_tool() const
{
    return selected_tool_button_ == nullptr ? nullptr : selected_tool_button_->get_tool();
}

EventHandlerState ToolPanel::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    surface_->draw_rectangle(area_, *DEFAULT_WIDGET_COLOR);

    return ContainerWidget::on_paint_event(event);
}

void ToolPanel::add_tool_button_(Tool *tool)
{
    assert(tool != nullptr);

    int cur_buttons_quantity = buttons_.size();

    int tool_buttons_in_a_row = area_.get_width() / DEFAULT_TOOL_BUTTON_WIDTH;
    ToolButton *new_button = new ToolButton(Rectangle{area_.get_x() + (DEFAULT_TOOL_BUTTON_WIDTH + 1)  * (cur_buttons_quantity % tool_buttons_in_a_row),
                                                      area_.get_y() +  DEFAULT_TOOL_BUTTON_HEIGHT      * (cur_buttons_quantity / tool_buttons_in_a_row),
                                                      DEFAULT_TOOL_BUTTON_WIDTH,
                                                      DEFAULT_TOOL_BUTTON_HEIGHT},
                                            tool, this);
    buttons_.push_back(new_button);
    new_button->chosen.connect(this, &ToolPanel::select_tool_button_);

    if (selected_tool_button_ == nullptr)
    {
        new_button->chosen.emit(new_button);
    }
}
    
void ToolPanel::select_tool_button_(ToolButton *button)
{
    assert(button != nullptr);

    if (selected_tool_button_ == button)
    {
        return;
    }

    if (selected_tool_button_ != nullptr)
    {
        selected_tool_button_->on_deselect();
    }
    selected_tool_button_ = button;
    selected_tool_button_->on_select();
    
    selected_tool_changed.emit(selected_tool_button_->get_tool());
}
//--------------------------------------------------------------------------------
ToolButton::ToolButton(const Rectangle &rectangle, Tool *tool, Widget *parent)
  : Button(rectangle, parent),
    tool_(tool)
{
    assert(tool != nullptr);

    has_texture_ = skin_->load_from_file(tool->get_texture_name());
}

ToolButton::~ToolButton()
{}


Tool *ToolButton::get_tool() const
{
    return tool_;
}

void ToolButton::on_select()
{
    is_selected_      = true;
    requires_repaint_ = true;
}

void ToolButton::on_deselect()
{
    is_selected_      = false;
    requires_repaint_ = true;
}


EventHandlerState ToolButton::on_mouse_button_released_event(const Event *event)
{
    assert(event != nullptr);

    if (is_pressed_ && (event->mbedata_.button == MouseButton::Left) && (area_.contains(event->mbedata_.position)))
    {
        chosen.emit(this);
    }

    return Button::on_mouse_button_released_event(event);
}

EventHandlerState ToolButton::on_paint_event(const Event *event)
{
    assert(event != nullptr);

    if (!requires_repaint_)
    {
        return EventHandlerState::Accepted;
    }

    surface_->clear(0);
    if (!has_texture_)
    {
        Color color_to_draw = is_hovered_ ? *DEFAULT_TOOL_BUTTON_HOVERING_COLOR : *DEFAULT_TOOL_BUTTON_COLOR;
        surface_->draw_rectangle(area_, color_to_draw);
    }
    else
    {
        surface_->draw_sprite(Sprite{*skin_, Rectangle{0, 0, area_.get_width(), area_.get_height()}});
        if (is_hovered_)
        {
            surface_->draw_rectangle({0, 0, area_.get_width(), area_.get_height()}, FOCUSED_WIDGET_TEXTURE_HOVERED_COLOR);
        }
    }
    draw_frame_();

    requires_repaint_ = false;

    return EventHandlerState::Accepted;
}

void ToolButton::draw_frame_(const Color &selected_tool_color, const Color &unselected_tool_color)
{
     // TODO: smart color choice in case of picture and frame colors similarity?
    surface_->draw_line({0,                      0}, {area_.get_width(),                      0}, is_selected_? selected_tool_color : unselected_tool_color);
    surface_->draw_line({area_.get_width(),      0}, {area_.get_width(), area_.get_height() - 1}, is_selected_? selected_tool_color : unselected_tool_color);
    surface_->draw_line({1,                      0}, {1,                 area_.get_height() - 1}, is_selected_? selected_tool_color : unselected_tool_color);
    surface_->draw_line({0, area_.get_height() - 1}, {area_.get_width(), area_.get_height() - 1}, is_selected_? selected_tool_color : unselected_tool_color);
}
