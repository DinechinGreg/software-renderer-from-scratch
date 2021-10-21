#pragma once

// Modify define values here to change the project's behaviour
#define RENDERER_TYPE 0 // 0 = OpenGL, 1 = to file

// The following is then automatically computed, no need to modify anything here
#if RENDERER_TYPE == 0
#define RENDERER_OPENGL
#elif RENDERER_TYPE == 1
#define RENDERER_TO_FILE
#endif
