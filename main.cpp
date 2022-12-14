#include "application.hpp"
#include "button.hpp"
#include "color.hpp"
#include "colorlibrary.hpp"
#include "materiallibrary.hpp"
#include "window.hpp"


//redesign: header and frames, cross with each containerwidget except menubar, hsv, button_size to text, drag-n-drop


int main()
{
    Application window_manager{};
    window_manager.execute();

    return 0;
}
