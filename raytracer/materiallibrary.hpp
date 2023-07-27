#ifndef MATERIAL_LIBRARY_HPP
#define MATERIAL_LIBRARY_HPP


#include "material.hpp"


const Material MIRROR       = {{0.0, 1.0, 0.0}, 1.33};
const Material MATTE        = {{1.0, 0.0, 0.0}, 1.5};
const Material TRANSPARENT  = {{0.0, 0.0, 1.0}, 1.33};
const Material LIGHT_SOURCE = {{0.0, 0.0, 0.0}, 0}; 

#endif