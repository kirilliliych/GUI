#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include "plug.hpp"


uint8_t get_r(uint32_t color) 
{
    return (color >> 24) & 0xFF;
}
uint8_t get_g(uint32_t color) 
{
    return (color >> 16) & 0xFF;
}
uint8_t get_b(uint32_t color) 
{
    return (color >>  8) & 0xFF;
}

double get_brightness(const uint32_t pixel)
{
    return 0.299 * get_r(pixel) + 0.587 * get_g(pixel) + 0.114 * get_b(pixel);
}

int brightness_comparator(const void *pixel1, const void *pixel2)
{
    assert(pixel1 != nullptr);
    assert(pixel2 != nullptr);

    double pixel1_brightness = get_brightness(*(reinterpret_cast<const uint32_t *> (pixel1)));
    double pixel2_brightness = get_brightness(*(reinterpret_cast<const uint32_t *> (pixel2)));

    double comparison_delta = 0.1;

    return  (pixel1_brightness > pixel2_brightness)                      ? 1 : 
           ((pixel2_brightness - pixel1_brightness) <= comparison_delta) ? 0 : -1;  
}

void MedianFilter::apply(booba::Image* image, const booba::Event* event)
{
    if ((event->type == booba::EventType::MousePressed) && (event->Oleg.mbedata.button == booba::MouseButton::Left))
    {
        uint32_t image_width  = image->getW();
        uint32_t image_height = image->getH();

        uint32_t *old_pixmap = new uint32_t[image_width * image_height];
        for (uint32_t x = 0; x < image_width; ++x)
        {
            for (uint32_t y = 0; y < image_height; ++y)
            {
                old_pixmap[y * image_width + x] = image->getPixel(x, y);   
            }
        }

        uint32_t zone_width  = FILTER_ZONE_WIDTH;
        uint32_t zone_height = FILTER_ZONE_HEIGHT;

        uint32_t edge_x = zone_width  / 2;
        uint32_t edge_y = zone_height / 2;

        uint32_t buffer[zone_width * zone_height];
        
        for (uint32_t x = edge_x; x < image_width - edge_x; ++x)
        {
            for (uint32_t y = edge_y; y < image_height - edge_y; ++y)
            {
                for (uint32_t fx = 0; fx < zone_width; ++fx)
                {
                    for (uint32_t fy = 0; fy < zone_height; ++fy)
                    {
                        buffer[fy * zone_width + fx] = old_pixmap[(y + fy - edge_y) * image_width + x + fx - edge_x];
                    }
                }
                qsort(buffer, zone_width * zone_height, sizeof(uint32_t), brightness_comparator);
                image->setPixel(x, y, buffer[edge_y * zone_width + edge_x]);
            }
        }

        delete old_pixmap;
    }
}


// extern "C" void init_module()
// {
//     addFilter(new MedianFilter);
// }

// extern "C" booba::GUID getGUID()
// {
//     return PLUGIN_GUID;
// } 