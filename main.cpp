#include "widgets/application.hpp"
#include "widgets/button.hpp"
#include "utilities/color.hpp"
#include "utilities/colorlibrary.hpp"
#include "raytracer/materiallibrary.hpp"
#include "sfmlwrap/window.hpp"


//todo: TOTAL REDESIGN, dialogwindow to brush, do dialogwindow, do textinput and cursor, do headers to widgets, coloring


int main()
{
    Application window_manager{};
    window_manager.execute();

    return 0;
}
