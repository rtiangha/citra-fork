// Copyright 2015 Citra Emulator Project
// Copyright 2024 Borked3DS Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/profiling.h"
#include "common/settings.h"
#include "core/core.h"
#include "core/frontend/emu_window.h"
#include "core/tracer/recorder.h"
#include "video_core/renderer_base.h"

namespace VideoCore {

[[maybe_unused]] static constexpr const char* EmuThreadFrame = "EmuThread";

RendererBase::RendererBase(Core::System& system_, Frontend::EmuWindow& window,
                           Frontend::EmuWindow* secondary_window_)
    : system{system_}, render_window{window}, secondary_window{secondary_window_} {
    BORKED3DS_FRAME_BEGIN(EmuThreadFrame);
}

RendererBase::~RendererBase() = default;

u32 RendererBase::GetResolutionScaleFactor() {
    const auto graphics_api = Settings::values.graphics_api.GetValue();
    if (graphics_api == Settings::GraphicsAPI::Software) {
        // Software renderer always renders at native resolution
        return 1;
    }

    const u32 scale_factor = Settings::values.resolution_factor.GetValue();
    return scale_factor != 0 ? scale_factor
                             : render_window.GetFramebufferLayout().GetScalingRatio();
}

void RendererBase::UpdateCurrentFramebufferLayout(bool is_portrait_mode) {
    const auto update_layout = [is_portrait_mode](Frontend::EmuWindow& window) {
        const Layout::FramebufferLayout& layout = window.GetFramebufferLayout();
        window.UpdateCurrentFramebufferLayout(layout.width, layout.height, is_portrait_mode);
    };
    update_layout(render_window);
    if (secondary_window) {
        update_layout(*secondary_window);
    }
}

void RendererBase::EndFrame() {
    current_frame++;

    system.perf_stats->EndSystemFrame();
    render_window.PollEvents();
    BORKED3DS_FRAME_END(EmuThreadFrame);

    system.frame_limiter.DoFrameLimiting(system.CoreTiming().GetGlobalTimeUs());
    system.perf_stats->BeginSystemFrame();
    BORKED3DS_FRAME_BEGIN(EmuThreadFrame);
}

bool RendererBase::IsScreenshotPending() const {
    return settings.screenshot_requested;
}

void RendererBase::RequestScreenshot(void* data, std::function<void(bool)> callback,
                                     const Layout::FramebufferLayout& layout) {
    if (settings.screenshot_requested) {
        LOG_ERROR(Render, "A screenshot is already requested or in progress, ignoring the request");
        return;
    }
    settings.screenshot_bits = data;
    settings.screenshot_complete_callback = callback;
    settings.screenshot_framebuffer_layout = layout;
    settings.screenshot_requested = true;
}

} // namespace VideoCore
