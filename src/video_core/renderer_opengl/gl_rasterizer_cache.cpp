// Copyright 2023 Citra Emulator Project
// Copyright 2024 Borked3DS Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "video_core/rasterizer_cache/rasterizer_cache.h"
#include "video_core/renderer_opengl/gl_texture_runtime.h"

namespace VideoCore {
template class RasterizerCache<OpenGL::Traits>;
} // namespace VideoCore
