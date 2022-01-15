#include "camera.h"

Camera::Camera(float near_plane, float far_plane, int width, int height)
    : Transform{}
    , m_near{near_plane}
    , m_far{far_plane}
    , m_width{width}
    , m_height{height}
{
}
