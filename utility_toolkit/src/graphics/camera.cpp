#include "camera.h"

Camera::Camera(float near, float far, int width, int height)
    : Transform{}
    , m_near{near}
    , m_far{far}
    , m_width{width}
    , m_height{height}
{
}
