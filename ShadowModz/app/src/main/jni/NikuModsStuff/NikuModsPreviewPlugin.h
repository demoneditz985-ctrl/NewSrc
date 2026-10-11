#pragma once

// Include this header in the native ImGui .so you select from the app.
// The app calls these functions on its OpenGL ES 3 render thread with a current EGL context.
// Your plugin must own its ImGui context and OpenGL3 backend and bundle a matching ImGui core.
#if defined(__GNUC__)
#define NIKUMODS_PREVIEW_EXPORT __attribute__((visibility("default")))
#else
#define NIKUMODS_PREVIEW_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Return 0 on success. Create your ImGui context, initialize the GLES3 backend and set size.
NIKUMODS_PREVIEW_EXPORT int NikuModsPreview_Init(int width, int height);

// Called after surface size changes.
NIKUMODS_PREVIEW_EXPORT void NikuModsPreview_Resize(int width, int height);

// Called once per frame. Run your ImGui NewFrame -> UI -> Render sequence here.
NIKUMODS_PREVIEW_EXPORT void NikuModsPreview_Render(void);

// action: 1 = finger down/move, 0 = finger up/cancel, 2 = scroll; scroll_y is nonzero for action 2.
NIKUMODS_PREVIEW_EXPORT void NikuModsPreview_Touch(float x, float y, int action, float scroll_y);

// Optional. Release your ImGui/GLES resources. If omitted, the loader skips shutdown.
NIKUMODS_PREVIEW_EXPORT void NikuModsPreview_Shutdown(void);

#ifdef __cplusplus
}
#endif
