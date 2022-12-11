#include "scene.hpp"


void Scene::fill_scene(const unsigned raytracer_width, const unsigned raytracer_height)
{
    camera_.create(Vector3d{0,  10,   0},
                   Vector3d{0,  10, -10},
                   Vector3d{ 0 , 1,   0},
                   90, 
                   (static_cast<float> (raytracer_width)) / raytracer_height
                  );

    
    Sphere *front_plane  = new Sphere{   0.0,    0.0, -1000.0,  980.0, MATTE, LIME,    false};
    Sphere *left_plane   = new Sphere{-990.0,    0.0,     0.0,  970.0, MATTE, GREY,    false};
    Sphere *right_plane  = new Sphere{ 990.0,    0.0,     0.0,  970.0, MATTE, ORANGE,  false};
    Sphere *bottom_plane = new Sphere{   0.0, -990.0,     0.0,  990.0, MATTE, CRIMSON, false};
    Sphere *top_plane    = new Sphere{   0.0, 1000.0,     0.0,  980.0, MATTE, YELLOW,  false};

    spheres_.add(front_plane);
    spheres_.add(left_plane);
    spheres_.add(right_plane);
    spheres_.add(bottom_plane);
    spheres_.add(top_plane);
    
    Sphere *ball1 = new Sphere{ -1.0,  10.0,  -5.0, 1.0, MIRROR, GREEN,      false};
    Sphere *ball2 = new Sphere{  5.0,  12.0,  -7.0, 3.0, MIRROR, YELLOW,     false};
    Sphere *ball3 = new Sphere{-10.0,   5.0, -17.0, 5.0, MIRROR, TOMATO,     false};
    Sphere *ball4 = new Sphere{  7.0,   3.0, -13.0, 3.0, MIRROR, MAGENTA,    false};
    Sphere *ball5 = new Sphere{-13.0,   2.0, -11.5, 2.0, MIRROR, LIGHT_BLUE, false};
    Sphere *ball6 = new Sphere{  0.0,   1.0, -11.0, 1.0, MATTE,  ORANGE,     false};
    Sphere *ball7 = new Sphere{ 13.0,   2.0, -14.0, 2.0, TRANSPARENT, SEA_GREEN, false};
    spheres_.add(ball1);
    spheres_.add(ball2);
    spheres_.add(ball3);
    spheres_.add(ball4);
    spheres_.add(ball5);
    spheres_.add(ball6);
    spheres_.add(ball7);

    Sphere *light1    = new Sphere{-1.0,  3.0,   -17.0,  3.0,  LIGHT_SOURCE, WHITE, true};
    Sphere *light2    = new Sphere{12.0,  4.0,   -17.0,  4.0,  LIGHT_SOURCE, WHITE, true};
    Sphere *light3    = new Sphere{-7.0, 13.0,   -13.0,  3.0,  LIGHT_SOURCE, WHITE, true};
    Sphere *giant_sun = new Sphere{ 0.0,  0.0,  1000.0, 970.0, LIGHT_SOURCE, WHITE, true};
    spheres_.add(light1);
    spheres_.add(light2);
    spheres_.add(light3);
    spheres_.add(giant_sun);
}
