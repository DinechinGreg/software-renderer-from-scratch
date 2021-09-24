#include "camera.h"

camera::camera(float near, float far, int width, int height)
    : transform{}
    , m_near{near}
    , m_far{far}
    , m_width{width}
    , m_height{height}
{
}
