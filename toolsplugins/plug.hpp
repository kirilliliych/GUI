#include "../standard/tools.hpp"


const int FILTER_ZONE_WIDTH  = 3;
const int FILTER_ZONE_HEIGHT = 3;

static const booba::GUID PLUGIN_GUID = {"a697f375-fb4d-46a9-abb3-fea886d6e712"};


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
