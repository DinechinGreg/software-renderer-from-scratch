#pragma once

#include <utilities/defines.h>

#if defined(RENDERER_TO_FILE)

#include <renderer/renderer_to_file.h>
using Renderer = Renderer_To_File;

#elif defined(RENDERER_OPENGL)

#include <renderer/renderer_opengl.h>
using Renderer = Renderer_OpenGL;

#endif
