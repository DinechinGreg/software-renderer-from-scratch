#pragma once

#include <utilities/defines.h>

#if defined(RENDERER_TO_FILE)

#include <renderer/renderer_to_file.h>
using renderer = renderer_to_file;

#elif defined(RENDERER_OPENGL)

#include <renderer/renderer_opengl.h>
using renderer = renderer_opengl;

#endif
