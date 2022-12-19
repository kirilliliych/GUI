#include "application.hpp"
#include "button.hpp"
#include "color.hpp"
#include "colorlibrary.hpp"
#include "materiallibrary.hpp"
#include "window.hpp"


//todo: TOTAL REDESIGN, dialogwindow to brush, do dialogwindow, do textinput and cursor, do headers to widgets


int main()
{
    Application window_manager{};
    window_manager.execute();

    return 0;
}
