#ifndef COLORLIBRARY_HPP
#define COLORLIBRARY_HPP


#include <vector>


static const Color RED               = Color{0xFF0000FF};
static const Color GREEN             = Color{0x00FF00FF};
static const Color BLUE              = Color{0x0000FFFF};
static const Color WHITE             = Color{0xFFFFFFFF};
static const Color BLACK             = Color{0x000000FF};

static const Color LIME              = Color{0x32CD32FF};
static const Color GREY              = Color{0x708090FF};
static const Color LIGHT_GREY        = Color{0xD3D3D3FF};
static const Color YELLOW            = Color{0xFFFF00FF};
static const Color TOMATO            = Color{0xFF6347FF};
static const Color LIGHT_BLUE        = Color{0x81B3FFFF};
static const Color ORANGE            = Color{0xFFA500FF};
static const Color MEDIUM_VIOLET_RED = Color{0xC71585FF};
static const Color CRIMSON           = Color{0xDC143CFF};
static const Color MAGENTA           = Color{0xFF00FFFF};
static const Color SEA_GREEN         = Color{0x2E8B57FF};


const std::vector<Color> ALL_AVAILABLE_COLORS{                // awful
                                              RED,
                                              GREEN,
                                              BLUE,
                                              WHITE,
                                              BLACK,
                                              LIME,
                                              GREY,
                                              LIGHT_GREY,
                                              YELLOW,
                                              TOMATO,
                                              LIGHT_BLUE,
                                              ORANGE,
                                              MEDIUM_VIOLET_RED,
                                              CRIMSON,
                                              MAGENTA,
                                              SEA_GREEN
                                            };

#endif
