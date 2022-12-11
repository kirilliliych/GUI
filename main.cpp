#include "application.hpp"
#include "button.hpp"
#include "color.hpp"
#include "colorlibrary.hpp"
#include "materiallibrary.hpp"
#include "window.hpp"


int main()
{
    Application window_manager{};
    window_manager.execute();

    return 0;
}
