#ifndef TOOL_DIALOG_WINDOW_HPP
#define TOOL_DIALOG_WINDOW_HPP


#include "../containerwidget.hpp"


class ToolDialogWindow : public ContainerWidget
{
public:
//-------------------------------------------------------------------
    ToolDialogWindow(const Rectangle &rectangle, ContainerWidget *parent = nullptr);

    ~ToolDialogWindow() override;
//-------------------------------------------------------------------

};


#endif

