#include "tools.hpp"


const int FILTER_ZONE_WIDTH  = 3;
const int FILTER_ZONE_HEIGHT = 3;
const int FILTER_ZONE_PIXELS_QUANTITY_SQUARE = 9;


class MedianFilter : public booba::Tool
{
public:
//-------------------------------------------------------------------
    MedianFilter() 
    {}

    ~MedianFilter() override
    {}
//-------------------------------------------------------------------
    void apply(booba::Image* image, const booba::Event* event) override;

    const char* getTexture() override
    { 
        return "medianfilter.png";
    } 

    void buildSetupWidget() override
    {}
};
