#include "application.hpp"
#include "button.hpp"
#include "color.hpp"
#include "colorlibrary.hpp"
#include "materiallibrary.hpp"
#include "window.hpp"


//redesign: header and frames, cross with each containerwidget except menubar и скрытие по нажатию, hsv, button_size to text, drag-n-drop
// fix background color troubles сделать cropper
//2 standart! 

//saturday:: scrollbar, slider, hsv
//sunday:: header and frames with closebutton, fixbutton, drag-n-drop

int main()
{
    Application window_manager{};
    window_manager.execute();

    return 0;
}
