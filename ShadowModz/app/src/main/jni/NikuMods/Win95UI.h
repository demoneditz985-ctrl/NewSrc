#pragma once
/* ===========================================================================
 * SHADOW MODZ shell  (floating glass panel, dark black + violet skin)
 * ---------------------------------------------------------------------------
 *   font   : NikuMods/Win95Assets/MicrosFont.hpp   (MS Sans Serif, 16px)
 *   ico    : NikuMods/Win95Assets/Win95Icon.hpp    (SHADOW MODZ brand icon)
 *   brand  : NikuMods/ShadowBrand.h                (every string + colour)
 *
 * The upstream Windows-XP desktop shell has been replaced by a modern floating
 * panel - no title bar, no min/max/close row, no menu bar, no toolbar, no tab
 * strip, no status bar, no resize grip:
 *   - BeginPanel()/EndPanel() draw one rounded glass card that is dragged by
 *     its header and dismissed with the round close button (back to the
 *     floating icon).
 *   - BeginGroup()/EndGroup() are section headers, so every feature page is
 *     stacked in a single scrolling column instead of hidden behind tabs.
 *   - HostMode::Standalone / HWND / win32 SetWindowPos removed. Dragging uses
 *     ImGui::SetWindowPos("##shadowmodz"), the embedded path upstream used.
 *   - ShellExecuteA replaced by an OpenUrl() no-op (no browser on device).
 *   - WIC/D3D11 icon upload replaced by stb_image + a GLES3 texture.
 *   - Palette: dark black surfaces, violet accents.
 *   - Telegram: COMING SOON (see ShadowBrand::TelegramUrl).
 * =========================================================================== */

#include "imgui.h"
#include "stb_image.h"
#include "ShadowBrand.h"

#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <cfloat>

#include "Win95Assets/MicrosFont.hpp"
#include "Win95Assets/Win95Icon.hpp"

// GLES3 is only present in the Android build. The header stays compilable in a
// desktop harness, where the title-bar icon simply falls back to a placeholder.
#if defined(__has_include)
#  if __has_include(<GLES3/gl3.h>)
#    include <GLES3/gl3.h>
#    define W95_HAS_GLES 1
#  endif
#endif

#ifndef BRU_LEGACY_FORK_GLOBALS
#define BRU_LEGACY_FORK_GLOBALS
// SHADOW MODZ accent (violet) - was the upstream teal.
float  menu[3] = { 168.f / 255.f, 85.f / 255.f, 247.f / 255.f };
ImFont* F52    = nullptr;
ImFont* F19    = nullptr;
ImFont* F48    = nullptr;
ImFont* CLOSE  = nullptr;
ImFont* SR     = nullptr;
ImFont* COLOR  = nullptr;
#endif
#if defined(NIKU_HARNESS) && !defined(W95_HARNESS_GLOBALS)
#define W95_HARNESS_GLOBALS
bool Aimbot        = false;
bool SilentAimv222 = false;
bool Headshot      = false;
bool norecoil      = false;
bool checkVisible2 = true;
bool esp           = false;
bool ignoreKnocked = true;
bool SpeedHack     = false;
bool EnemyCount    = false;
bool GameTimer     = false;
bool HighFps       = false;
bool TeleportMark1 = false;
bool HasMapMarker  = false;
float Size          = 1000.0f;
float aimSmoothness = 0.1f;
struct _w95_cfg
{
    struct _esp
    {
        bool Box, Line, Health, Skeleton, Distancia, IsName, Target, RamaMods;
    } ESP;
} Config;
#endif  // NIKU_HARNESS

namespace W95
{

/* ---------------------------------------------------------------------------
 * styling.hpp
 * ------------------------------------------------------------------------ */
namespace style
{
/* Every pixel constant below is expressed in units of 1.0 = upstream Win95 at
 * 16px, then multiplied by S. Bump S alone to grow the whole UI (font, rows,
 * chrome, scrollbar) without rows overlapping or controls overflowing. */
constexpr float S           = 1.25f;

// ---- SHADOW MODZ palette (dark black + violet). Edit ShadowBrand.h only. ---
constexpr ImU32 Face         = IM_COL32(ShadowBrand::Face.r,         ShadowBrand::Face.g,         ShadowBrand::Face.b,         255);
constexpr ImU32 Light        = IM_COL32(ShadowBrand::Light.r,        ShadowBrand::Light.g,        ShadowBrand::Light.b,        255);
constexpr ImU32 Highlight    = IM_COL32(ShadowBrand::Highlight.r,    ShadowBrand::Highlight.g,    ShadowBrand::Highlight.b,    255);
constexpr ImU32 Shadow       = IM_COL32(ShadowBrand::Shadow.r,       ShadowBrand::Shadow.g,       ShadowBrand::Shadow.b,       255);
constexpr ImU32 DarkShadow   = IM_COL32(ShadowBrand::DarkShadow.r,   ShadowBrand::DarkShadow.g,   ShadowBrand::DarkShadow.b,   255);

constexpr ImU32 Field        = IM_COL32(ShadowBrand::Field.r,        ShadowBrand::Field.g,        ShadowBrand::Field.b,        255);
constexpr ImU32 Text         = IM_COL32(ShadowBrand::Text.r,         ShadowBrand::Text.g,         ShadowBrand::Text.b,         255);
constexpr ImU32 TextDisabled = IM_COL32(ShadowBrand::TextDisabled.r, ShadowBrand::TextDisabled.g, ShadowBrand::TextDisabled.b, 255);

constexpr ImU32 TitleLeft    = IM_COL32(ShadowBrand::TitleLeft.r,    ShadowBrand::TitleLeft.g,    ShadowBrand::TitleLeft.b,    255);
constexpr ImU32 TitleRight   = IM_COL32(ShadowBrand::TitleRight.r,   ShadowBrand::TitleRight.g,   ShadowBrand::TitleRight.b,   255);
constexpr ImU32 TitleText    = IM_COL32(ShadowBrand::TitleText.r,    ShadowBrand::TitleText.g,    ShadowBrand::TitleText.b,    255);

constexpr ImU32 Selection    = IM_COL32(ShadowBrand::Selection.r,    ShadowBrand::Selection.g,    ShadowBrand::Selection.b,    255);
constexpr ImU32 SelText      = IM_COL32(ShadowBrand::SelText.r,      ShadowBrand::SelText.g,      ShadowBrand::SelText.b,      255);
constexpr ImU32 SliderFill   = IM_COL32(ShadowBrand::SliderFill.r,   ShadowBrand::SliderFill.g,   ShadowBrand::SliderFill.b,   255);

// accents the original Win95 palette had no equivalent for
constexpr ImU32 BgDeep       = IM_COL32(ShadowBrand::BgDeep.r,       ShadowBrand::BgDeep.g,       ShadowBrand::BgDeep.b,       255);
constexpr ImU32 Accent       = IM_COL32(ShadowBrand::Accent.r,       ShadowBrand::Accent.g,       ShadowBrand::Accent.b,       255);
constexpr ImU32 AccentDeep   = IM_COL32(ShadowBrand::AccentDeep.r,   ShadowBrand::AccentDeep.g,   ShadowBrand::AccentDeep.b,   255);
constexpr ImU32 AccentGlow   = IM_COL32(ShadowBrand::AccentGlow.r,   ShadowBrand::AccentGlow.g,   ShadowBrand::AccentGlow.b,   255);
constexpr ImU32 BtnTop       = IM_COL32(ShadowBrand::BtnTop.r,       ShadowBrand::BtnTop.g,       ShadowBrand::BtnTop.b,       255);
constexpr ImU32 BtnBot       = IM_COL32(ShadowBrand::BtnBot.r,       ShadowBrand::BtnBot.g,       ShadowBrand::BtnBot.b,       255);
constexpr ImU32 BtnTopHover  = IM_COL32(ShadowBrand::BtnTopHover.r,  ShadowBrand::BtnTopHover.g,  ShadowBrand::BtnTopHover.b,  255);
constexpr ImU32 BtnBotHover  = IM_COL32(ShadowBrand::BtnBotHover.r,  ShadowBrand::BtnBotHover.g,  ShadowBrand::BtnBotHover.b,  255);
constexpr ImU32 GroupBg      = IM_COL32(ShadowBrand::GroupBg.r,      ShadowBrand::GroupBg.g,      ShadowBrand::GroupBg.b,      255);
constexpr ImU32 StatusBg     = IM_COL32(ShadowBrand::StatusBg.r,     ShadowBrand::StatusBg.g,     ShadowBrand::StatusBg.b,     255);

constexpr float FrameBorder    = 3. * S;
constexpr float TitleBarH      = 18. * S;
constexpr float TitleBtnW      = 16. * S;
constexpr float TitleBtnH      = 14. * S;
constexpr float MenuBarH       = 18. * S;
constexpr float ToolbarH       = 26. * S;
constexpr float TabBarH        = 21. * S;
constexpr float TabPadX        = 8. * S;
constexpr float StatusBarH     = 20. * S;
constexpr float GripSize       = 16. * S;
constexpr float ScrollbarW     = 16. * S;
constexpr float ScrollArrowBtn = 16. * S;
constexpr float ScrollMinThumb = 16. * S;

constexpr float ContentPad     = 5. * S;
constexpr float ColumnGutter   = 5. * S;
constexpr float GroupGap       = 6. * S;
constexpr float GroupTopPad    = 15. * S;
constexpr float GroupInnerX    = 9. * S;
constexpr float GroupBottomPad = 9. * S;

constexpr float RowSpacing     = 4. * S;
constexpr float CheckboxSize   = 13. * S;
constexpr float RowHeight      = 14. * S;
constexpr float SwatchW        = 26. * S;
constexpr float SwatchH        = 14. * S;
constexpr float KeybindW       = 34. * S;
constexpr float KeybindH       = 14. * S;
constexpr float SliderTrackH   = 22. * S;
constexpr float ThumbW         = 11. * S;
constexpr float ThumbBodyH     = 13. * S;
constexpr float ThumbPointH    = 5. * S;
constexpr float TickH          = 3. * S;
constexpr float ComboH         = 20. * S;
constexpr float ButtonH        = 23. * S;

/* SHADOW MODZ floating-panel metrics (no window chrome any more) */
constexpr float PanelW        = 340.f * S;
constexpr float PanelH        = 470.f * S;
constexpr float PanelRadius   = 18.f * S;
constexpr float HeaderH       = 44.f * S;   // logo + name + close, drag handle
constexpr float FooterH       = 28.f * S;   // live dot + uid
constexpr float SectionHeadH  = 26.f * S;   // feature section title
constexpr float SwitchW       = 34.f * S;   // pill switch
constexpr float SwitchH       = 18.f * S;

inline void FillFace(ImDrawList* dl, const ImVec2& a, const ImVec2& b)
{
    dl->AddRectFilled(a, b, Face);
}

inline void Edge(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 topLeft, ImU32 bottomRight)
{
    dl->AddRectFilled(ImVec2(a.x, a.y), ImVec2(b.x, a.y + 1.f), topLeft);
    dl->AddRectFilled(ImVec2(a.x, a.y), ImVec2(a.x + 1.f, b.y), topLeft);
    dl->AddRectFilled(ImVec2(a.x, b.y - 1.f), ImVec2(b.x, b.y), bottomRight);
    dl->AddRectFilled(ImVec2(b.x - 1.f, a.y), ImVec2(b.x, b.y), bottomRight);
}

inline void Raised(ImDrawList* dl, const ImVec2& a, const ImVec2& b)
{
    Edge(dl, a, b, Highlight, DarkShadow);
    Edge(dl, ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), Light, Shadow);
}

inline void Pressed(ImDrawList* dl, const ImVec2& a, const ImVec2& b)
{
    Edge(dl, a, b, Shadow, Highlight);
    Edge(dl, ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), DarkShadow, Face);
}

inline void Sunken(ImDrawList* dl, const ImVec2& a, const ImVec2& b)
{
    Edge(dl, a, b, Shadow, Highlight);
    Edge(dl, ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), DarkShadow, Light);
}

inline void Etched(ImDrawList* dl, const ImVec2& a, const ImVec2& b)
{
    Edge(dl, a, b, Shadow, Highlight);
    Edge(dl, ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), Highlight, Shadow);
}

inline void Stipple(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 c0, ImU32 c1)
{
    dl->AddRectFilled(a, b, c0);
    int row = 0;
    for (float y = a.y; y < b.y; y += 1.f, ++row)
    {
        for (float x = a.x + ((row & 1) ? 1.f : 0.f); x < b.x; x += 2.f)
        {
            dl->AddRectFilled(ImVec2(x, y), ImVec2(x + 1.f, y + 1.f), c1);
        }
    }
}

/* --- SHADOW MODZ extras ------------------------------------------------- */

// Vertical gradient, used for the title bar, buttons and the launcher disc.
inline void FillGradientV(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 top, ImU32 bottom)
{
    dl->AddRectFilledMultiColor(a, b, top, top, bottom, bottom);
}

// Soft violet halo: five translucent outlines fading outwards. Pure draw-list
// work, so it costs nothing on a phone and needs no texture or shader.
inline void Glow(ImDrawList* dl, const ImVec2& a, const ImVec2& b, float strength)
{
    if (strength <= 0.f) { return; }

    for (int i = 5; i >= 1; --i)
    {
        const float o  = (float)i * 2.f;
        const int   al = (int)(15.f * strength);
        dl->AddRect(ImVec2(a.x - o, a.y - o), ImVec2(b.x + o, b.y + o),
                    IM_COL32(ShadowBrand::Accent.r, ShadowBrand::Accent.g, ShadowBrand::Accent.b, al),
                    0.f, 0, 1.f + (float)i * 0.5f);
    }
}

// Hairline accent border - lifts panels and controls off the black.
inline void AccentEdge(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 col)
{
    dl->AddRect(a, b, col, 0.f, 0, 1.f);
}

// Glossy highlight along the top inside edge of a control.
inline void Gloss(ImDrawList* dl, const ImVec2& a, const ImVec2& b, int alpha)
{
    dl->AddRectFilled(ImVec2(a.x + 2.f, a.y + 1.f),
                      ImVec2(b.x - 2.f, a.y + 2.f),
                      IM_COL32(255, 255, 255, alpha));
}

// Triangle wave 0..1 on the frame clock, for the launcher pulse.
inline float Pulse(float speed = 1.6f)
{
    const float t = (float)ImGui::GetTime() * speed;
    const float f = t - std::floor(t);
    return (f < 0.5f) ? (f * 2.f) : ((1.f - f) * 2.f);
}

// Halo that follows a rounded rect - used by the panel, switches and pills.
inline void GlowRound(ImDrawList* dl, const ImVec2& a, const ImVec2& b, float r, float strength)
{
    for (int i = 5; i >= 1; --i)
    {
        const float o = (float)i * 2.5f;
        dl->AddRect(ImVec2(a.x - o, a.y - o), ImVec2(b.x + o, b.y + o),
                    IM_COL32(ShadowBrand::Accent.r, ShadowBrand::Accent.g, ShadowBrand::Accent.b,
                             (int)(11.f * strength)),
                    r + o, 0, 1.5f);
    }
}
}
/* ---------------------------------------------------------------------------
 * shell.cpp - window frame, tab bar, groups, scrollbar, status bar
 * ------------------------------------------------------------------------ */
inline constexpr int   kMinWidth  = (int)(390.f * style::S);
inline constexpr int   kMinHeight = (int)(292.f * style::S);
inline constexpr float kDefaultW  = 457.f * style::S;
inline constexpr float kDefaultH  = 423.f * style::S;
inline constexpr float kFontSize   = 16.f * style::S;

inline ImDrawList* s_dl              = nullptr;
inline bool        s_forcePlace      = true;
inline bool        s_closeRequested  = false;

inline ImVec2 s_winMin     = ImVec2(0.f, 0.f);
inline ImVec2 s_winMax     = ImVec2(0.f, 0.f);
inline ImVec2 s_contentMin = ImVec2(0.f, 0.f);
inline ImVec2 s_contentMax = ImVec2(0.f, 0.f);

inline int   s_columns = 2;
inline float s_colX[4] = { 0.f, 0.f, 0.f, 0.f };
inline float s_colW[4] = { 0.f, 0.f, 0.f, 0.f };
inline float s_colY[4] = { 0.f, 0.f, 0.f, 0.f };

inline int    s_curCol       = 0;
inline float  s_panelX       = 0.f;
inline float  s_panelW       = 0.f;
inline float  s_indent       = 0.f;

inline ImU32  s_accent     = style::Accent;
inline bool   s_accentOn   = false;
inline float  s_groupStartY = 0.f;
inline char   s_groupLabel[64] = { 0 };
inline ImVec2 s_cursor     = ImVec2(0.f, 0.f);
inline ImVec2 s_lastItemMin = ImVec2(0.f, 0.f);
inline ImVec2 s_lastItemMax = ImVec2(0.f, 0.f);

inline float  s_tabX   = 0.f;
inline bool   s_toolbarVisible = true;
inline int    s_currentTab = 0;

inline char   s_popup[64]        = { 0 };
inline bool   s_popupBlockClicks = false;

inline bool   s_dragging   = false;
inline bool   s_resizing   = false;
inline ImVec2 s_dragOffset = ImVec2(0.f, 0.f);

inline float  s_scrollY      = 0.f;
inline float  s_maxColBottom  = 0.f;
inline bool   s_needScrollbar = false;
inline ImVec2 s_bodyMin       = ImVec2(0.f, 0.f);
inline ImVec2 s_bodyMax       = ImVec2(0.f, 0.f);
inline bool   s_contentClipped = false;
inline bool   s_scrollDrag    = false;
inline float  s_scrollGrab    = 0.f;

inline bool MenuIsOpen();
inline bool PopUpsAreClosed();
inline ImU32 AccentColor();
inline ImTextureID IconTex();
inline bool s_treeOpen   = false;
#ifdef NIKU_HARNESS
inline ImVec2 s_dbgItemA = ImVec2(0.f, 0.f);   // harness: rect of the first
inline ImVec2 s_dbgItemB = ImVec2(0.f, 0.f);   // widget drawn in a frame
inline int    s_dbgFrame = -1;
inline ImVec2 s_dbgSliderA = ImVec2(0.f, 0.f);  // harness: rect of the slider
inline ImVec2 s_dbgSliderB = ImVec2(0.f, 0.f);
inline int    s_dbgSliderFrame = -1;
inline void   DbgRecord(const ImVec2& a, const ImVec2& b)
{
    const int f = ImGui::GetFrameCount();
    if (s_dbgFrame != f) { s_dbgFrame = f; s_dbgItemA = a; s_dbgItemB = b; }
}
#endif
inline int  s_focused    = -1;

inline void CopyStr(char* dst, const char* src, size_t cap)
{
    size_t i = 0;
    for (; src[i] != '\0' && i + 1 < cap; ++i)
    {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

inline void SetupColumns()
{
    // Single scrolling column - the floating panel stacks its sections.
    s_colX[0] = s_contentMin.x;
    s_colW[0] = s_contentMax.x - s_contentMin.x;
    s_colY[0] = s_contentMin.y - s_scrollY;
}
inline void SetColumns(int count)
{
    (void)count;
    // Everything is one column now; kept so the pages stay untouched.
}
inline int& CurrentTab() { return s_currentTab; }

inline void DrawTitleGlyph(const ImVec2& c, int glyph, ImU32 col)
{
    if (glyph == 0)
    {
        s_dl->AddRectFilled(ImVec2(c.x - 4.f, c.y + 3.f), ImVec2(c.x + 2.f, c.y + 5.f), col);
    }
    else if (glyph == 1)
    {
        s_dl->AddRect(ImVec2(c.x - 4.f, c.y - 4.f), ImVec2(c.x + 4.f, c.y + 4.f), col);
        s_dl->AddRectFilled(ImVec2(c.x - 4.f, c.y - 4.f), ImVec2(c.x + 4.f, c.y - 2.f), col);
    }
    else
    {
        for (float t = -3.f; t <= 3.f; t += 1.f)
        {
            s_dl->AddRectFilled(ImVec2(c.x + t - 1.f, c.y + t - 1.f), ImVec2(c.x + t + 1.f, c.y + t + 1.f), col);
            s_dl->AddRectFilled(ImVec2(c.x + t - 1.f, c.y - t - 1.f), ImVec2(c.x + t + 1.f, c.y - t + 1.f), col);
        }
    }
}


inline void UpdateDrag(const ImVec2& captionMin, const ImVec2& captionMax)
{
    if (s_dragging)
    {
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            s_dragging = false;
            return;
        }
        const ImVec2 m = ImGui::GetMousePos();
        ImGui::SetWindowPos("##shadowmodz", ImVec2(m.x - s_dragOffset.x, m.y - s_dragOffset.y));
        return;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(captionMin, captionMax))
    {
        const ImVec2 m = ImGui::GetMousePos();
        s_dragOffset = ImVec2(m.x - s_winMin.x, m.y - s_winMin.y);
        s_dragging = true;
    }
}


/* SHADOW MODZ floating panel: a rounded glass card with no window chrome.
 * Drag it by the header, close it with the round button (back to the floating
 * icon). Every feature lives in the scrolling section list below. */
inline void BeginPanel()
{
    const ImVec2 size = ImGui::GetIO().DisplaySize;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));

    float w = style::PanelW;
    float h = style::PanelH;
    if (w > size.x - 24.f) { w = size.x - 24.f; }
    if (h > size.y - 40.f) { h = size.y - 40.f; }

    const ImGuiCond cond = s_forcePlace ? ImGuiCond_Always : ImGuiCond_FirstUseEver;
    ImVec2 pos((size.x - w) * 0.5f, (size.y - h) * 0.5f);
    if (pos.x < 12.f) { pos.x = 12.f; }
    if (pos.y < 12.f) { pos.y = 12.f; }
    ImGui::SetNextWindowPos(pos, cond);
    ImGui::SetNextWindowSize(ImVec2(w, h), cond);
    s_forcePlace = false;

    ImGui::Begin("##shadowmodz", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings);

    s_dl     = ImGui::GetWindowDrawList();
    s_winMin = ImGui::GetWindowPos();
    const ImVec2 winSize = ImGui::GetWindowSize();
    s_winMax = ImVec2(s_winMin.x + winSize.x, s_winMin.y + winSize.y);

    const float r = style::PanelRadius;

    // ---- glass card ------------------------------------------------------
    style::GlowRound(s_dl, s_winMin, s_winMax, r, 0.9f);
    s_dl->AddRectFilledMultiColorRounded(s_winMin, s_winMax, style::BgDeep,
                                         style::Face, style::Face, style::BgDeep, style::BgDeep, r,
                                         ImDrawFlags_RoundCornersAll);
    s_dl->AddRect(s_winMin, s_winMax, style::AccentDeep, r, 0, 2.f);
    s_dl->AddRect(ImVec2(s_winMin.x + 1.f, s_winMin.y + 1.f),
                  ImVec2(s_winMax.x - 1.f, s_winMax.y - 1.f),
                  IM_COL32(ShadowBrand::AccentGlow.r, ShadowBrand::AccentGlow.g,
                           ShadowBrand::AccentGlow.b, 34), r - 1.f, 0, 1.f);

    // ---- header ----------------------------------------------------------
    const float  hh   = style::HeaderH;
    const ImVec2 hMin(s_winMin.x + 10.f, s_winMin.y + 10.f);
    const ImVec2 hMax(s_winMax.x - 10.f, s_winMin.y + 10.f + hh);

    style::GlowRound(s_dl, hMin, hMax, 12.f, 0.35f);
    s_dl->AddRectFilledMultiColorRounded(hMin, hMax, style::AccentDeep,
                                         style::AccentDeep, style::Accent, style::Accent, style::AccentDeep,
                                         12.f, ImDrawFlags_RoundCornersAll);
    s_dl->AddRect(hMin, hMax,
                  IM_COL32(ShadowBrand::Accent.r, ShadowBrand::Accent.g, ShadowBrand::Accent.b, 90),
                  12.f, 0, 1.f);

    // logo disc
    const float  lr = (hh - 10.f) * 0.5f;
    const ImVec2 lc(hMin.x + 8.f + lr, (hMin.y + hMax.y) * 0.5f);
    s_dl->AddCircleFilled(lc, lr, style::BgDeep, 24);
    s_dl->AddCircle(lc, lr, style::AccentGlow, 24, 2.f);
    if (IconTex() != 0)
    {
        const float ir = lr - 4.f;
        s_dl->AddImage(IconTex(), ImVec2(lc.x - ir, lc.y - ir), ImVec2(lc.x + ir, lc.y + ir));
    }

    // name + version chip
    const float  tx = lc.x + lr + 10.f;
    const float  ty = (hMin.y + hMax.y) * 0.5f - ImGui::GetTextLineHeight() * 0.5f;
    const ImVec2 ns = ImGui::CalcTextSize(ShadowBrand::BrandName);
    s_dl->AddText(ImVec2(tx + 1.f, ty + 1.f), IM_COL32(0, 0, 0, 170), ShadowBrand::BrandName);
    s_dl->AddText(ImVec2(tx, ty), style::TitleText, ShadowBrand::BrandName);

    const ImVec2 vs = ImGui::CalcTextSize(ShadowBrand::Version);
    const ImVec2 cMin(tx + ns.x + 8.f, ty + 1.f);
    const ImVec2 cMax(cMin.x + vs.x + 12.f, cMin.y + vs.y + 4.f);
    s_dl->AddRectFilled(cMin, cMax, IM_COL32(255, 255, 255, 40), (cMax.y - cMin.y) * 0.5f);
    s_dl->AddText(ImVec2(cMin.x + 6.f, ty), style::TitleText, ShadowBrand::Version);

    // round close button
    const float  br     = 11.f * style::S;
    const ImVec2 bc(hMax.x - 8.f - br, (hMin.y + hMax.y) * 0.5f);
    const bool   bHover = ImGui::IsMouseHoveringRect(ImVec2(bc.x - br, bc.y - br),
                                                     ImVec2(bc.x + br, bc.y + br));
    s_dl->AddCircleFilled(bc, br, bHover ? IM_COL32(196, 62, 88, 255) : IM_COL32(38, 27, 60, 255), 20);
    s_dl->AddCircle(bc, br, bHover ? style::AccentGlow : style::Light, 20, 1.5f);
    s_dl->AddLine(ImVec2(bc.x - 4.f, bc.y - 4.f), ImVec2(bc.x + 4.f, bc.y + 4.f), style::Text, 2.f);
    s_dl->AddLine(ImVec2(bc.x + 4.f, bc.y - 4.f), ImVec2(bc.x - 4.f, bc.y + 4.f), style::Text, 2.f);
    if (bHover && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) { s_closeRequested = true; }

    UpdateDrag(hMin, ImVec2(bc.x - br - 6.f, hMax.y));

    // ---- body ------------------------------------------------------------
    s_bodyMin = ImVec2(s_winMin.x + 12.f, hMax.y + 8.f);
    s_bodyMax = ImVec2(s_winMax.x - 12.f, s_winMax.y - style::FooterH);
    s_contentMin = ImVec2(s_bodyMin.x + 6.f, s_bodyMin.y + 4.f);
    s_contentMax = ImVec2(s_bodyMax.x - 6.f, s_bodyMax.y - 4.f);

    s_panelX        = s_contentMin.x;
    s_panelW        = s_contentMax.x - s_contentMin.x;
    s_cursor        = ImVec2(s_panelX, s_contentMin.y - s_scrollY);
    s_maxColBottom  = s_contentMin.y - s_scrollY;
    s_contentClipped = false;
    s_popupBlockClicks = (s_popup[0] != '\0') || MenuIsOpen();
}

inline bool BeginGroup(const char* label, int column)
{
    (void)column;

    // Clip the whole panel body once, so long feature lists scroll cleanly.
    if (!s_contentClipped)
    {
        s_dl->PushClipRect(s_bodyMin, s_bodyMax, true);
        s_contentClipped = true;
    }

    s_curCol      = 0;
    s_panelX      = s_contentMin.x;
    s_panelW      = s_contentMax.x - s_contentMin.x;
    s_groupStartY = s_maxColBottom;
    s_cursor      = ImVec2(s_panelX, s_groupStartY + style::SectionHeadH);
    s_indent      = 0.f;
    CopyStr(s_groupLabel, label, sizeof(s_groupLabel));

    // Section head: violet accent bar + uppercase title + hairline divider.
    char up[64];
    CopyStr(up, label, sizeof(up));
    for (char* p = up; *p; ++p) { *p = (char)std::toupper((unsigned char)*p); }

    const float y  = s_groupStartY;
    const float hh = style::SectionHeadH;
    s_dl->AddRectFilled(ImVec2(s_panelX, y + hh * 0.25f),
                        ImVec2(s_panelX + 3.f, y + hh * 0.75f), style::Accent, 1.5f);
    s_dl->AddText(ImVec2(s_panelX + 12.f, y + (hh - ImGui::GetTextLineHeight()) * 0.5f),
                  style::AccentGlow, up);
    s_dl->AddLine(ImVec2(s_panelX + 12.f, y + hh - 3.f),
                  ImVec2(s_panelX + s_panelW, y + hh - 3.f),
                  IM_COL32(ShadowBrand::Accent.r, ShadowBrand::Accent.g, ShadowBrand::Accent.b, 45));
    return true;
}
inline void EndGroup()
{
    // No box to close: sections are separated by their header + divider.
    s_maxColBottom = s_cursor.y + style::GroupGap + 6.f;
}
inline ImVec2 CursorPos() { return s_cursor; }

inline void AddItem(const ImVec2& size)
{
    s_lastItemMin = s_cursor;
    s_lastItemMax = ImVec2(s_cursor.x + size.x, s_cursor.y + size.y);
    s_cursor.y += size.y + style::RowSpacing;
}

inline bool IsLastItemHovered() { return PopUpsAreClosed() && ImGui::IsMouseHoveringRect(s_lastItemMin, s_lastItemMax); }
inline void Indent(float w = 14.f)   { s_cursor.x += w; s_indent += w; }
inline void Unindent(float w = 14.f) { s_cursor.x -= w; s_indent -= w; }
inline float ContentWidth() { return s_panelW - style::GroupInnerX * 2.f - s_indent; }
inline float PanelLeftX()   { return s_panelX + style::GroupInnerX; }
inline float PanelRightX()  { return s_panelX + s_panelW - style::GroupInnerX; }

inline float BodyBottom() { return s_bodyMax.y; }
inline float BodyLeft()   { return s_bodyMin.x; }
inline float BodyRight()  { return s_bodyMax.x; }
inline void  SetToolbarVisible(bool visible) { s_toolbarVisible = visible; }

inline void SetAccent(const float col[4], bool enabled)
{
    s_accentOn = enabled;
    s_accent   = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.f));
}

inline ImU32 AccentColor()
{
    if (s_accentOn) { return s_accent; }
    return style::Selection;
}

inline bool PopUpsAreClosed() { return s_popup[0] == '\0' && !s_popupBlockClicks; }
inline const char* SelectedItem() { return s_popup; }

inline void SetSelectedItem(const char* id)
{
    if (!id) { s_popup[0] = '\0'; return; }
    CopyStr(s_popup, id, sizeof(s_popup));
}

inline void OpenUrl(const char*) { }

inline bool CloseRequested()
{
    const bool r = s_closeRequested;
    s_closeRequested = false;
    return r;
}


inline void ScrollArrow(const ImVec2& a, const ImVec2& b, bool up)
{
    const bool held = ImGui::IsMouseHoveringRect(a, b) && ImGui::IsMouseDown(ImGuiMouseButton_Left);
    style::FillFace(s_dl, a, b);
    if (held) { style::Pressed(s_dl, a, b); }
    else      { style::Raised(s_dl, a, b); }

    const float push = held ? 1.f : 0.f;
    const ImVec2 c((a.x + b.x) * 0.5f + push, (a.y + b.y) * 0.5f + push);
    if (up)
    {
        s_dl->AddTriangleFilled(ImVec2(c.x, c.y - 3.f), ImVec2(c.x - 3.f, c.y + 2.f), ImVec2(c.x + 3.f, c.y + 2.f), style::Text);
    }
    else
    {
        s_dl->AddTriangleFilled(ImVec2(c.x - 3.f, c.y - 2.f), ImVec2(c.x + 3.f, c.y - 2.f), ImVec2(c.x, c.y + 3.f), style::Text);
    }
}


inline void EndPanel()
{
    if (s_contentClipped)
    {
        s_dl->PopClipRect();
        s_contentClipped = false;
    }

    const float availH        = s_contentMax.y - s_contentMin.y;
    const float contentHeight = (s_maxColBottom + s_scrollY) - s_contentMin.y;
    const float maxScroll     = (contentHeight > availH) ? (contentHeight - availH) : 0.f;
    s_needScrollbar = maxScroll > 0.f;

    if (s_needScrollbar)
    {
        if (ImGui::IsMouseHoveringRect(s_bodyMin, s_bodyMax) && PopUpsAreClosed())
        {
            const float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.f) { s_scrollY -= wheel * 40.f; }
        }
        if (s_scrollY < 0.f) { s_scrollY = 0.f; }
        if (s_scrollY > maxScroll) { s_scrollY = maxScroll; }

        // slim scrollbar hugging the right edge of the card
        const float trackH = s_bodyMax.y - s_bodyMin.y;
        float thumbH = trackH * (availH / contentHeight);
        if (thumbH < 30.f) { thumbH = 30.f; }
        const float ty     = s_bodyMin.y + (trackH - thumbH) *
                             (maxScroll > 0.f ? (s_scrollY / maxScroll) : 0.f);
        s_dl->AddRectFilled(ImVec2(s_bodyMax.x - 3.f, s_bodyMin.y),
                            ImVec2(s_bodyMax.x, s_bodyMax.y), IM_COL32(255, 255, 255, 16), 1.5f);
        s_dl->AddRectFilled(ImVec2(s_bodyMax.x - 3.f, ty),
                            ImVec2(s_bodyMax.x, ty + thumbH), style::Accent, 1.5f);
    }
    else
    {
        s_scrollY = 0.f;
    }

    // ---- footer: live dot + uid -----------------------------------------
    const ImVec2 fMin(s_winMin.x + 14.f, s_winMax.y - style::FooterH);
    const ImVec2 fMax(s_winMax.x - 14.f, s_winMax.y - 10.f);
    s_dl->AddLine(ImVec2(fMin.x, fMin.y), ImVec2(fMax.x, fMin.y),
                  IM_COL32(ShadowBrand::Accent.r, ShadowBrand::Accent.g, ShadowBrand::Accent.b, 70));

    const float dy = (fMin.y + fMax.y) * 0.5f;
    s_dl->AddCircleFilled(ImVec2(fMin.x + 6.f, dy), 3.5f * style::S, IM_COL32(74, 222, 128, 255), 12);
    s_dl->AddCircle(ImVec2(fMin.x + 6.f, dy), 3.5f * style::S, IM_COL32(74, 222, 128, 90), 12, 3.f);
    s_dl->AddText(ImVec2(fMin.x + 18.f, dy - ImGui::GetTextLineHeight() * 0.5f),
                  style::AccentGlow, ShadowBrand::StatusUser);

    const char*  uid = ShadowBrand::StatusUid;
    const ImVec2 us  = ImGui::CalcTextSize(uid);
    s_dl->AddText(ImVec2(fMax.x - us.x, dy - ImGui::GetTextLineHeight() * 0.5f),
                  style::TextDisabled, uid);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}/* ---------------------------------------------------------------------------
 * widgets.cpp - checkbox, button, label, progress, radio, separator, tree
 * ------------------------------------------------------------------------ */
inline ImVec2 InsetMin(const ImVec2& a, float e) { return ImVec2(a.x + e, a.y + e); }
inline ImVec2 InsetMax(const ImVec2& b, float e) { return ImVec2(b.x - e, b.y - e); }

inline void ToolTip(const char* text)
{
    if (!text || !text[0] || !IsLastItemHovered()) { return; }

    const ImVec2 ts  = ImGui::CalcTextSize(text);
    const float  lh  = ImGui::GetTextLineHeight();
    const float  px  = (ImGui::GetMousePos().x > (s_winMin.x + s_winMax.x) * 0.5f) ? -1.f : 1.f;

    const float  ax  = px > 0.f ? (s_cursor.x) : (s_cursor.x - ts.x - 12.f);
    const ImVec2 boxA(ax, s_cursor.y);
    const ImVec2 boxB(ax + ts.x + 12.f, s_cursor.y + lh + 6.f);

    s_dl->AddRectFilled(boxA, boxB, IM_COL32(255, 255, 225, 255));
    style::Etched(s_dl, boxA, boxB);
    s_dl->AddText(ImVec2(boxA.x + 6.f, boxA.y + 3.f), style::Text, text);
}

inline bool Checkbox(const char* label, bool* value)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  th = ImGui::GetTextLineHeight();
    const float  ty = s_cursor.y + (style::RowHeight - th) * 0.5f;

    const ImVec2 boxA(s_cursor.x, s_cursor.y + (style::RowHeight - style::SwitchH) * 0.5f);
    const ImVec2 boxB(boxA.x + style::SwitchW, boxA.y + style::SwitchH);
    const float  rad = style::SwitchH * 0.5f;

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(boxA, boxB);

    // SHADOW MODZ pill switch: sliding knob, violet halo while on.
    if (*value) { style::GlowRound(s_dl, boxA, boxB, rad, 0.75f); }

    s_dl->AddRectFilled(boxA, boxB, *value ? style::AccentDeep : style::Field, rad);
    s_dl->AddRect(boxA, boxB, *value ? style::Accent : style::Light, rad, 0, 1.f);

    const float kx = *value ? (boxB.x - rad) : (boxA.x + rad);
    s_dl->AddCircleFilled(ImVec2(kx, (boxA.y + boxB.y) * 0.5f), rad - 3.f,
                          *value ? style::AccentGlow : style::TextDisabled, 16);

    s_dl->AddText(ImVec2(boxB.x + 8.f, ty), *value ? style::Text : style::TextDisabled, label);
#ifdef NIKU_HARNESS
    DbgRecord(boxA, boxB);
#endif

    bool changed = false;
    if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        *value = !*value;
        changed = true;
    }

    AddItem(ImVec2(style::SwitchW + 8.f + ts.x, style::RowHeight));
    return changed;
}
inline void Label(const char* text)
{
    const ImVec2 ts = ImGui::CalcTextSize(text);
    s_dl->AddText(s_cursor, style::Text, text);
    AddItem(ImVec2(ts.x, ImGui::GetTextLineHeight()));
}

inline bool Button(const char* label, const ImVec2& size)
{
    const ImVec2 a = s_cursor;
    const ImVec2 b(s_cursor.x + size.x, s_cursor.y + size.y);
    const float  r = size.y * 0.5f;

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(a, b);
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    // SHADOW MODZ pill button: rounded gradient face + violet halo.
    if (hovered) { style::GlowRound(s_dl, a, b, r, held ? 1.0f : 0.75f); }

    s_dl->AddRectFilledMultiColorRounded(a, b, style::BtnBot,
                                         hovered ? style::BtnTopHover : style::BtnTop,
                                         hovered ? style::BtnTopHover : style::BtnTop,
                                         hovered ? style::BtnBotHover : style::BtnBot,
                                         hovered ? style::BtnBotHover : style::BtnBot, r,
                                         ImDrawFlags_RoundCornersAll);
    s_dl->AddRect(a, b, hovered ? style::AccentGlow : style::AccentDeep, r, 0, 1.5f);
    s_dl->AddRectFilled(ImVec2(a.x + 6.f, a.y + 2.f), ImVec2(b.x - 6.f, a.y + 3.f),
                        IM_COL32(255, 255, 255, hovered ? 56 : 26));

    const float  push = held ? 1.f : 0.f;
    const ImVec2 ts   = ImGui::CalcTextSize(label);
    const ImVec2 tp(a.x + (size.x - ts.x) * 0.5f + push, a.y + (size.y - ts.y) * 0.5f + push);
    s_dl->AddText(ImVec2(tp.x + 1.f, tp.y + 1.f), IM_COL32(0, 0, 0, 170), label);
    s_dl->AddText(tp, hovered ? style::AccentGlow : style::Text, label);

    AddItem(size);
    return hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
}
// Full-violet variant for the primary action of a page (Telegram, Save, ...).
inline bool AccentButton(const char* label, const ImVec2& size, bool enabled = true)
{
    const ImVec2 a = s_cursor;
    const ImVec2 b(s_cursor.x + size.x, s_cursor.y + size.y);
    const float  r = size.y * 0.5f;

    const bool live    = enabled && PopUpsAreClosed();
    const bool hovered = live && ImGui::IsMouseHoveringRect(a, b);
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    if (hovered) { style::GlowRound(s_dl, a, b, r, 1.0f); }

    if (!enabled)
    {
        s_dl->AddRectFilled(a, b, style::BtnBot, r);
        s_dl->AddRect(a, b, style::Light, r, 0, 1.f);
    }
    else
    {
        s_dl->AddRectFilledMultiColorRounded(a, b, style::AccentDeep,
                                             hovered ? style::AccentGlow : style::Accent,
                                             hovered ? style::AccentGlow : style::Accent,
                                             hovered ? style::Accent : style::AccentDeep,
                                             hovered ? style::Accent : style::AccentDeep, r,
                                             ImDrawFlags_RoundCornersAll);
        s_dl->AddRect(a, b, hovered ? style::AccentGlow : style::AccentDeep, r, 0, 1.5f);
    }
    s_dl->AddRectFilled(ImVec2(a.x + 6.f, a.y + 2.f), ImVec2(b.x - 6.f, a.y + 3.f),
                        IM_COL32(255, 255, 255, enabled ? (hovered ? 80 : 46) : 14));

    const float  push = held ? 1.f : 0.f;
    const ImVec2 ts   = ImGui::CalcTextSize(label);
    const ImVec2 tp(a.x + (size.x - ts.x) * 0.5f + push, a.y + (size.y - ts.y) * 0.5f + push);
    s_dl->AddText(ImVec2(tp.x + 1.f, tp.y + 1.f), IM_COL32(0, 0, 0, 170), label);
    s_dl->AddText(tp, enabled ? style::TitleText : style::TextDisabled, label);

    AddItem(size);
    return hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
}
inline bool Button(const char* label)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    return Button(label, ImVec2(ts.x + 18.f, style::ButtonH));
}

inline bool AccentButton(const char* label)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    return AccentButton(label, ImVec2(ts.x + 18.f, style::ButtonH));
}

inline bool AccentButton(const char* label, bool enabled)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    return AccentButton(label, ImVec2(ts.x + 18.f, style::ButtonH), enabled);
}

inline void ProgressBar(const char* label, float fraction)
{
    if (fraction < 0.f) { fraction = 0.f; }
    if (fraction > 1.f) { fraction = 1.f; }

    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  y  = s_cursor.y;

    s_dl->AddText(ImVec2(s_cursor.x, y + (style::RowHeight - ts.y) * 0.5f), style::Text, label);

    const ImVec2 barA(s_cursor.x + ts.x + 6.f, y + 2.f);
    const ImVec2 barB(s_cursor.x + ContentWidth(), y + style::RowHeight - 2.f);

    style::FillFace(s_dl, barA, barB);
    style::Raised(s_dl, barA, barB);

    const ImVec2 fillA(barA.x + 2.f, barA.y + 2.f);
    const float  fw    = (barB.x - barA.x - 4.f) * fraction;
    const ImVec2 fillB(fillA.x + fw, barB.y - 2.f);
    if (fw > 0.f)
    {
        for (float x = fillA.x; x < fillB.x; x += 2.f)
        {
            const float x2 = (x + 1.f < fillB.x) ? (x + 1.f) : fillB.x;
            s_dl->AddRectFilled(ImVec2(x, fillA.y), ImVec2(x2, fillB.y), AccentColor());
        }
    }

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
}

inline bool RadioButton(const char* label, bool* selected)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  th = ImGui::GetTextLineHeight();
    const float  ty = s_cursor.y + (style::RowHeight - th) * 0.5f;

    const float  r  = 5.5f;
    const ImVec2 c(s_cursor.x + r + 0.5f, s_cursor.y + style::RowHeight * 0.5f);

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(ImVec2(s_cursor.x, s_cursor.y),
                                                                ImVec2(s_cursor.x + 14.f + ts.x,
                                                                       s_cursor.y + style::RowHeight));

    const int seg = 10;
    for (int i = 0; i < seg; ++i)
    {
        const float a0 = ((float)i / seg) * 6.2831853f;
        const float a1 = ((float)(i + 1) / seg) * 6.2831853f;
        const ImVec2 p0(c.x - r * cosf(a0), c.y - r * sinf(a0));
        const ImVec2 p1(c.x - r * cosf(a1), c.y - r * sinf(a1));
        if (*selected)
        {
            s_dl->AddTriangleFilled(ImVec2(c.x, c.y - r), p0, p1, AccentColor());
        }
        else
        {
            s_dl->AddTriangleFilled(ImVec2(c.x, c.y - r), p0, p1, style::Face);
        }
    }
    s_dl->AddCircle(c, r, style::Shadow, 0, 1.f);

    s_dl->AddText(ImVec2(s_cursor.x + 14.f, ty), style::Text, label);

    bool changed = false;
    if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        *selected = true;
        changed   = true;
    }

    AddItem(ImVec2(14.f + ts.x, style::RowHeight));
    return changed;
}

inline void Separator()
{
    const float mid = s_cursor.y + style::RowHeight * 0.5f;
    const float x0  = s_cursor.x;
    const float x1  = s_cursor.x + ContentWidth();
    const float xm  = (x0 + x1) * 0.5f;

    s_dl->AddRectFilled(ImVec2(x0, mid), ImVec2(xm - 1.f, mid + 1.f), style::Shadow);
    s_dl->AddRectFilled(ImVec2(xm, mid), ImVec2(x1, mid + 1.f), style::Highlight);

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
}

inline void Spacing()
{
    AddItem(ImVec2(ContentWidth(), 4.f));
}

inline bool TreeNode(const char* label)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  h  = style::RowHeight;
    const float  y  = s_cursor.y;
    const float  tw = ContentWidth();

    const ImVec2 boxA(s_cursor.x + 1.f, y + (h - style::CheckboxSize) * 0.5f);
    const ImVec2 boxB(boxA.x + style::CheckboxSize, boxA.y + style::CheckboxSize);

    style::FillFace(s_dl, boxA, boxB);
    style::Sunken(s_dl, boxA, boxB);

    const float ax = (boxA.x + boxB.x) * 0.5f;
    const float ay = (boxA.y + boxB.y) * 0.5f;
    const float d  = 3.5f;
    if (s_treeOpen)
    {
        s_dl->AddLine(ImVec2(ax - d, ay), ImVec2(ax + d, ay), style::Text, 1.f);
        s_dl->AddLine(ImVec2(ax, ay - d), ImVec2(ax, ay + d), style::Text, 1.f);
    }
    else
    {
        s_dl->AddLine(ImVec2(ax - d, ay - d), ImVec2(ax + d, ay + d), style::Text, 1.f);
    }

    s_dl->AddText(ImVec2(boxB.x + 5.f, y + (h - ts.y) * 0.5f), style::Text, label);

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(ImVec2(s_cursor.x, y), ImVec2(s_cursor.x + tw, y + h));
    if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        s_treeOpen = !s_treeOpen;
    }

    AddItem(ImVec2(tw, h));
    return s_treeOpen;
}

inline void SameLine(float spacing)
{
    s_cursor.y = s_lastItemMin.y;
    s_cursor.x = s_lastItemMax.x + spacing;
}
/* ---------------------------------------------------------------------------
 * slider.cpp / keybind.cpp
 * ------------------------------------------------------------------------ */
inline float Slider(const char* label, float value, float vmin, float vmax, const char* fmt)
{
    char buf[32];
    std::snprintf(buf, sizeof(buf), fmt ? fmt : "%.2f", value);

    const float w  = ContentWidth();
    const float hh = style::RowHeight;

    s_dl->AddText(ImVec2(s_cursor.x, s_cursor.y), style::Text, label);
    const ImVec2 vs = ImGui::CalcTextSize(buf);
    s_dl->AddText(ImVec2(s_cursor.x + w - vs.x, s_cursor.y), style::AccentGlow, buf);

    const float  cy = s_cursor.y + hh + 4.f;
    const ImVec2 tA(s_cursor.x, cy - 3.f);
    const ImVec2 tB(s_cursor.x + w, cy + 3.f);

    s_dl->AddRectFilled(tA, tB, style::Field, 3.f);
    s_dl->AddRect(tA, tB, style::Light, 3.f, 0, 1.f);

    if (vmax <= vmin) { vmax = vmin + 1e-4f; }
    float frac = (value - vmin) / (vmax - vmin);
    if (frac < 0.f) { frac = 0.f; }
    if (frac > 1.f) { frac = 1.f; }

    const float kx = tA.x + (tB.x - tA.x) * frac;
    s_dl->AddRectFilled(ImVec2(tA.x, tA.y), ImVec2(kx, tB.y), style::Accent, 3.f);

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(
        ImVec2(tA.x - 8.f, tA.y - 10.f), ImVec2(tB.x + 8.f, tB.y + 10.f));
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    if (hovered || held)
    {
        style::GlowRound(s_dl, ImVec2(tA.x, cy - 8.f), ImVec2(tB.x, cy + 8.f), 8.f, 0.5f);
    }
    s_dl->AddCircleFilled(ImVec2(kx, cy), held ? 9.f : 7.f, style::AccentGlow, 16);
    s_dl->AddCircle(ImVec2(kx, cy), held ? 9.f : 7.f, style::AccentDeep, 16, 1.5f);

    if (held)
    {
        float nf = (ImGui::GetMousePos().x - tA.x) / (tB.x - tA.x);
        if (nf < 0.f) { nf = 0.f; }
        if (nf > 1.f) { nf = 1.f; }
        value = vmin + nf * (vmax - vmin);
    }

    AddItem(ImVec2(w, hh + 16.f));
    return value;
}
inline const char* KeyName(int key)
{
    const char* n = ImGui::GetKeyName((ImGuiKey)key);
    if (n && n[0]) { return n; }
    return "None";
}

inline bool Keybind(const char* label, int* key)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  lh = ImGui::GetTextLineHeight();
    s_dl->AddText(ImVec2(s_cursor.x, s_cursor.y + (style::RowHeight - lh) * 0.5f), style::Text, label);

    const float  fw = ContentWidth() - ts.x - 6.f;
    const ImVec2 boxA(s_cursor.x + ts.x + 6.f, s_cursor.y + (style::RowHeight - style::KeybindH) * 0.5f);
    const ImVec2 boxB(boxA.x + fw, boxA.y + style::KeybindH);

    const bool  waiting = (*key < 0);
    const bool  enabled = PopUpsAreClosed();
    const bool  hovered = enabled && ImGui::IsMouseHoveringRect(boxA, boxB);
    const bool  held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    style::FillFace(s_dl, boxA, boxB);
    if (waiting || held) { style::Pressed(s_dl, boxA, boxB); }
    else                 { style::Raised(s_dl, boxA, boxB); }

    const char* txt = waiting ? "Press key" : KeyName(*key);
    const ImVec2 tt  = ImGui::CalcTextSize(txt);
    s_dl->AddText(ImVec2((boxA.x + boxB.x) * 0.5f - tt.x * 0.5f, (boxA.y + boxB.y) * 0.5f - tt.y * 0.5f),
                  waiting ? style::SelText : style::Text, txt);

    if (waiting && enabled)
    {
        ImGui::IsKeyPressed(ImGuiKey_Escape, true);
        for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; ++k)
        {
            if (ImGui::IsKeyPressed((ImGuiKey)k, true))
            {
                *key = k;
                break;
            }
        }
    }

    bool changed = false;
    if (hovered && !held && !waiting && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        *key    = -1;
        changed = true;
    }

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
    return changed;
}

inline void ColorSwatch(const char* label, const float col[4])
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  lh = ImGui::GetTextLineHeight();
    s_dl->AddText(ImVec2(s_cursor.x, s_cursor.y + (style::RowHeight - lh) * 0.5f), style::Text, label);

    const ImVec2 a(s_cursor.x + ts.x + 6.f, s_cursor.y + (style::RowHeight - style::SwatchH) * 0.5f);
    const ImVec2 b(a.x + style::SwatchW, a.y + style::SwatchH);

    style::FillFace(s_dl, a, b);
    s_dl->AddRectFilled(InsetMin(a, 1.f), InsetMax(b, 1.f), ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.f)));
    style::Raised(s_dl, a, b);

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
}

inline void ToolGlyph(ImDrawList* dl, const ImVec2& c, int glyph, ImU32 col)
{
    switch (glyph)
    {
    case 0:
        dl->AddRectFilled(ImVec2(c.x - 4.f, c.y - 4.f), ImVec2(c.x + 4.f, c.y + 4.f), col);
        dl->AddRectFilled(ImVec2(c.x - 2.f, c.y - 2.f), ImVec2(c.x + 2.f, c.y + 2.f), style::Field);
        break;
    case 1:
        dl->AddRect(ImVec2(c.x - 4.f, c.y - 4.f), ImVec2(c.x + 4.f, c.y + 4.f), col, 1.5f);
        break;
    case 2:
        dl->AddCircleFilled(c, 4.f, col);
        dl->AddCircle(c, 5.5f, col, 0, 1.5f);
        break;
    case 3:
        for (int i = 0; i < 3; ++i)
        {
            dl->AddLine(ImVec2(c.x - 5.f, c.y - 3.f + (float)i * 3.f),
                        ImVec2(c.x + 5.f, c.y - 3.f + (float)i * 3.f), col, 1.f);
        }
        break;
    case 4:
        dl->AddTriangleFilled(ImVec2(c.x - 5.f, c.y + 4.f), ImVec2(c.x, c.y - 5.f), ImVec2(c.x + 5.f, c.y + 4.f), col);
        break;
    default:
        break;
    }
}
/* ---------------------------------------------------------------------------
 * color.cpp - inline colour editor used by the Colours dropdown
 * ------------------------------------------------------------------------ */
inline float DragFloat(const char* label, float value, float vmin, float vmax, const char* fmt)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  lh = ImGui::GetTextLineHeight();

    s_dl->AddText(ImVec2(s_cursor.x, s_cursor.y + (style::RowHeight - lh) * 0.5f), style::Text, label);

    const float  fw = ContentWidth() - ts.x - 46.f;
    const ImVec2 boxA(s_cursor.x + ts.x + 4.f, s_cursor.y + (style::RowHeight - style::KeybindH) * 0.5f);
    const ImVec2 boxB(boxA.x + fw, boxA.y + style::KeybindH);

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(boxA, boxB);
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    style::FillFace(s_dl, boxA, boxB);
    if (held) { style::Pressed(s_dl, boxA, boxB); }
    else      { style::Raised(s_dl, boxA, boxB); }

    char num[32];
    snprintf(num, sizeof(num), fmt ? fmt : "%.2f", value);
    const ImVec2 nt = ImGui::CalcTextSize(num);
    s_dl->AddText(ImVec2((boxA.x + boxB.x) * 0.5f - nt.x * 0.5f, (boxA.y + boxB.y) * 0.5f - nt.y * 0.5f),
                  style::Text, num);

    const ImVec2 ax(s_cursor.x + ContentWidth() - 10.f, s_cursor.y + style::RowHeight * 0.5f);
    s_dl->AddTriangleFilled(ImVec2(ax.x - 3.f, ax.y - 2.f), ImVec2(ax.x + 3.f, ax.y - 2.f), ImVec2(ax.x, ax.y + 2.f),
                            style::Text);

    if (held)
    {
        const float my = ImGui::GetMousePos().y - boxA.y;
        float t  = (my - 2.f) / (boxB.y - boxA.y - 4.f);
        if (t < 0.f) { t = 0.f; }
        if (t > 1.f) { t = 1.f; }
        value = vmin + t * (vmax - vmin);
    }

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
    return value;
}

inline void ColorEdit(const char* label, float col[4], bool withAlpha, bool withHue, bool withPicker)
{
    ColorSwatch(label, col);

    const float swatchH = style::SwatchH;

    if (withHue)
    {
        const ImVec2 barA(s_cursor.x, s_cursor.y + 2.f);
        const ImVec2 barB(s_cursor.x + ContentWidth(), s_cursor.y + style::RowHeight - 2.f);

        const bool enabled = PopUpsAreClosed();
        const bool held    = enabled && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                             ImGui::IsMouseHoveringRect(barA, barB);

        const float h = col[0];
        for (int i = 0; i <= 32; ++i)
        {
            const float t  = (float)i / 32.f;
            const float hh = h + (t - 0.5f) * 0.1666f;
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(hh < 0.f ? hh + 1.f : (hh > 1.f ? hh - 1.f : hh), 1.f, 1.f, r, g, b);
            s_dl->AddRectFilled(ImVec2(barA.x + (barB.x - barA.x) * t, barA.y),
                                ImVec2(barA.x + (barB.x - barA.x) * (t + 1.f / 32.f), barB.y),
                                ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, 1.f)));
        }

        style::Etched(s_dl, barA, barB);

        if (held)
        {
            const float t = (ImGui::GetMousePos().x - barA.x) / (barB.x - barA.x);
            col[0] = h + (t - 0.5f) * 0.1666f;
            if (col[0] < 0.f) { col[0] += 1.f; }
            if (col[0] > 1.f) { col[0] -= 1.f; }
        }

        const float mx = barA.x + (barB.x - barA.x) * (h - 0.5f) / 0.1666f + 0.5f;
        if (mx < barA.x) { }
        s_dl->AddRectFilled(ImVec2(mx - 3.f, barA.y - 2.f), ImVec2(mx + 3.f, barB.y + 2.f), style::Highlight);
        s_dl->AddRect(ImVec2(mx - 3.f, barA.y - 2.f), ImVec2(mx + 3.f, barB.y + 2.f), style::DarkShadow, 1.f, 0, 1.f);

        AddItem(ImVec2(ContentWidth(), style::RowHeight));
    }

    if (withPicker)
    {
        const ImVec2 areaA(s_cursor.x, s_cursor.y + 2.f);
        const float  size = (ContentWidth() - style::ColumnGutter) * 0.5f;
        const ImVec2 areaB(areaA.x + size, areaA.y + size);

        const bool enabled = PopUpsAreClosed();
        const bool held    = enabled && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
                             ImGui::IsMouseHoveringRect(areaA, areaB);

        // Bilinear SV field drawn as a coarse grid: a per-pixel loop would blow
        // past the 16-bit index limit (5 colour editors on the Visuals tab).
        constexpr int kGrid = 24;
        for (int x = 0; x < kGrid; ++x)
        {
            for (int y = 0; y < kGrid; ++y)
            {
                const float fx0 = (float)x / kGrid, fx1 = (float)(x + 1) / kGrid;
                const float fy0 = (float)y / kGrid, fy1 = (float)(y + 1) / kGrid;
                const ImVec2 g0(areaA.x + (areaB.x - areaA.x) * fx0, areaA.y + (areaB.y - areaA.y) * fy0);
                const ImVec2 g1(areaA.x + (areaB.x - areaA.x) * fx1, areaA.y + (areaB.y - areaA.y) * fy1);
                const float v = 1.f - (fy0 + fy1) * 0.5f;
                const float sat = (fx0 + fx1) * 0.5f;
                s_dl->AddRectFilled(g0, g1, ImGui::ColorConvertFloat4ToU32(ImVec4(v, v + (1.f - v) * sat, v, 1.f)));
            }
        }

        style::Etched(s_dl, areaA, areaB);

        if (held)
        {
            const float sx = (ImGui::GetMousePos().x - areaA.x) / (areaB.x - areaA.x);
            const float sy = (ImGui::GetMousePos().y - areaA.y) / (areaB.y - areaA.y);
            float tv = 1.f - sy;
            float ts = sx;
            if (tv < 0.f) { tv = 0.f; }
            if (tv > 1.f) { tv = 1.f; }
            if (ts < 0.f) { ts = 0.f; }
            if (ts > 1.f) { ts = 1.f; }
            col[1] = tv;
            col[2] = (1.f - tv) + tv * ts;
        }

        AddItem(ImVec2(ContentWidth(), size + 4.f));
    }

    if (withAlpha)
    {
        col[3] = DragFloat("Alpha", col[3], 0.f, 1.f, "%.2f");
    }
    if (withHue)
    {
        col[0] = DragFloat("Hue", col[0], 0.f, 1.f, "%.2f");
    }
    col[1] = DragFloat("Sat", col[1], 0.f, 1.f, "%.2f");
    col[2] = DragFloat("Val", col[2], 0.f, 1.f, "%.2f");
    (void)swatchH;
}

inline void ColorEdit4(const char* label, float col[4], bool flags) { ColorEdit(label, col, flags, true, true); }
inline void ColorEdit3(const char* label, float col[4], bool flags) { ColorEdit(label, col, false, true, false); }
/* ---------------------------------------------------------------------------
 * combo.cpp / listbox.cpp / input.cpp
 * ------------------------------------------------------------------------ */
inline void DropDownArrows(const ImVec2& a, const ImVec2& b)
{
    const float x = (a.x + b.x) * 0.5f + 3.5f;
    const float y = (a.y + b.y) * 0.5f;
    s_dl->AddRectFilled(ImVec2(x - 3.5f, y - 4.5f), ImVec2(x + 4.5f, y + 1.5f), style::Shadow);
    s_dl->AddRectFilled(ImVec2(x - 3.5f, y - 5.5f), ImVec2(x + 4.5f, y + 0.5f), style::Light);
    s_dl->AddTriangleFilled(ImVec2(x - 3.f, y - 2.f), ImVec2(x + 3.f, y - 2.f), ImVec2(x, y + 2.5f), style::Text);
}

inline const char* Combo(const char* label, int selected, const char* items, int count)
{
    (void)label;
    if (count <= 0) { return nullptr; }

    const ImVec2 a = s_cursor;
    const ImVec2 b(s_cursor.x + ContentWidth(), s_cursor.y + style::ComboH);

    const char* preview = nullptr;
    int         index   = -1;
    int         start   = 0;
    const char* cur     = items;
    while (start < count && *cur)
    {
        if (index == -1) { preview = cur; }
        if (start == selected) { preview = cur; }
        const char* next = strchr(cur, '\x01');
        if (!next) { break; }
        cur = next + 1;
        ++start;
    }
    if (!preview || preview[0] == '\0') { return nullptr; }
    index = selected;

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(a, b);
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    // SHADOW MODZ combo: rounded field with a violet edge, no bevel.
    s_dl->AddRectFilled(a, b, hovered ? style::Field : style::BgDeep, style::ComboH * 0.5f);
    s_dl->AddRect(a, b, hovered ? style::Accent : style::AccentDeep, style::ComboH * 0.5f, 0, 1.5f);

    s_dl->AddText(ImVec2(a.x + 10.f, a.y + (style::ComboH - ImGui::GetTextLineHeight()) * 0.5f),
                  style::Text, preview);
    DropDownArrows(a, b);

    int   chosen  = selected;
    float itemH   = ImGui::GetTextLineHeight() + 3.f;
    float listW   = ContentWidth();
    float maxListH = itemH * (float)count;
    if (maxListH > 120.f) { maxListH = 120.f; }
    const ImVec2 listA(a.x, b.y + 1.f);
    const ImVec2 listB(b.x, listA.y + maxListH + 4.f);

    const bool open = (held && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) ||
                      (held && ImGui::IsMouseHoveringRect(listA, listB));
    if (open)
    {
        ImGui::SetNextWindowPos(ImVec2(a.x, b.y + 1.f), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(1.f);
        ImGui::SetNextWindowSize(ImVec2(listW, maxListH + 4.f), ImGuiCond_Always);
        ImGui::Begin("##combo", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
        s_dl = ImGui::GetWindowDrawList();
        const ImVec2 lp = ImGui::GetWindowPos();
        style::GlowRound(s_dl, lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), 10.f, 0.6f);
        s_dl->AddRectFilled(lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), style::Face, 10.f);
        s_dl->AddRect(lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), style::AccentDeep, 10.f, 0, 1.5f);
        ImGui::End();

        float iy = listA.y + 2.f;
        int   ci = 0;
        cur = items;
        while (*cur)
        {
            const char* next = strchr(cur, '\x01');
            const int   len  = next ? (int)(next - cur) : (int)strlen(cur);
            const ImVec2 ia(a.x + 2.f, iy);
            const ImVec2 ib(a.x + listW - 2.f, iy + itemH);
            const bool  ih  = PopUpsAreClosed() && ImGui::IsMouseHoveringRect(ia, ib);
            if (ci == selected)
            {
                s_dl->AddRectFilled(ia, ib, style::Selection);
            }
            else if (ih)
            {
                s_dl->AddRectFilled(ia, ib, style::Highlight);
            }
            s_dl->AddText(ImVec2(ia.x + 4.f, ia.y + 1.5f), ci == selected ? style::SelText : style::Text, cur);
            if (ih && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) { chosen = ci; }
            iy += itemH;
            ++ci;
            if (!next) { break; }
            cur = next + 1;
        }

        if (!ImGui::IsMouseHoveringRect(listA, listB) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            chosen = selected;
        }

        if (chosen != selected)
        {
            const char* pick = items;
            for (int i = 0; i < chosen; ++i)
            {
                const char* n = strchr(pick, '\x01');
                if (!n) { break; }
                pick = n + 1;
            }
            AddItem(ImVec2(ContentWidth(), style::ComboH));
            return pick;
        }
    }

    AddItem(ImVec2(ContentWidth(), style::ComboH));
    (void)index;
    (void)start;
    return nullptr;
}

inline void InputText(const char* label, char* buf, size_t bufSize, bool password)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  lh = ImGui::GetTextLineHeight();
    s_dl->AddText(ImVec2(s_cursor.x, s_cursor.y + (style::RowHeight - lh) * 0.5f), style::Text, label);

    const ImVec2 boxA(s_cursor.x + ts.x + 6.f, s_cursor.y + (style::RowHeight - style::KeybindH) * 0.5f);
    const ImVec2 boxB(s_cursor.x + ContentWidth(), boxA.y + style::KeybindH);

    const bool enabled = PopUpsAreClosed();
    const bool hovered = enabled && ImGui::IsMouseHoveringRect(boxA, boxB);
    const bool held    = hovered && ImGui::IsMouseDown(ImGuiMouseButton_Left);

    style::FillFace(s_dl, boxA, boxB);
    style::Sunken(s_dl, boxA, boxB);

    ImGui::SetNextWindowPos(ImVec2(boxA.x + 1.f, boxA.y + 1.f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.f);
    ImGui::SetNextWindowSize(ImVec2(boxB.x - boxA.x - 2.f, boxB.y - boxA.y - 2.f), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_Text, style::Text);
    if (ImGui::Begin("##field", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        s_dl = ImGui::GetWindowDrawList();
        s_dl->AddText(ImVec2(boxA.x + 4.f, boxA.y + 2.f), style::Text,
                      password ? "********" : buf);
        ImGui::End();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    const ImVec2 ax(boxB.x - 8.f, (boxA.y + boxB.y) * 0.5f);
    s_dl->AddTriangleFilled(ImVec2(ax.x - 3.f, ax.y - 2.f), ImVec2(ax.x + 3.f, ax.y - 2.f),
                            ImVec2(ax.x, ax.y + 2.f), style::Shadow);
    (void)held;
    (void)bufSize;
    (void)s_focused;

    AddItem(ImVec2(ContentWidth(), style::RowHeight));
}
/* ---------------------------------------------------------------------------
 * msgbox.cpp - modal dialogs
 * ------------------------------------------------------------------------ */
inline constexpr int kMsgInfo    = 0;
inline constexpr int kMsgWarn    = 1;
inline constexpr int kMsgError   = 2;
inline constexpr int kMsgYesNo   = 3;
inline constexpr int kMsgOkCancel = 4;

inline bool  s_msgActive  = false;
inline int   s_msgType    = 0;
inline int   s_msgResult  = 0;
inline char  s_msgTitle[64] = { 0 };
inline char  s_msgText[256] = { 0 };
inline ImVec2 s_msgMin    = ImVec2(0.f, 0.f);
inline ImVec2 s_msgMax    = ImVec2(0.f, 0.f);

inline void ShowMsgBox(int type, const char* title, const char* text)
{
    s_msgActive = true;
    s_msgType   = type;
    s_msgResult = 0;
    CopyStr(s_msgTitle, title ? title : "Windows", sizeof(s_msgTitle));
    CopyStr(s_msgText, text ? text : "", sizeof(s_msgText));
}

inline void ShowInfo(const char* title, const char* text)    { ShowMsgBox(kMsgInfo, title, text); }
inline void ShowWarn(const char* title, const char* text)    { ShowMsgBox(kMsgWarn, title, text); }
inline void ShowError(const char* title, const char* text)   { ShowMsgBox(kMsgError, title, text); }
inline void ShowYesNo(const char* title, const char* text)   { ShowMsgBox(kMsgYesNo, title, text); }
inline void ShowOkCancel(const char* title, const char* text){ ShowMsgBox(kMsgOkCancel, title, text); }

inline bool IsMsgBoxOpen() { return s_msgActive; }
inline int  MsgBoxResult() { return s_msgResult; }
inline void CloseMsgBox()   { s_msgActive = false; s_msgResult = 0; }

inline ImU32 MsgBoxIconColor()
{
    switch (s_msgType)
    {
    case kMsgWarn:  return IM_COL32(255, 200, 0, 255);
    case kMsgError: return IM_COL32(200, 0, 0, 255);
    default:        return style::TitleLeft;
    }
}

inline void MsgBoxIcon(int type, const ImVec2& c)
{
    switch (type)
    {
    case kMsgInfo:
    {
        s_dl->AddCircle(c, 10.f, style::TitleLeft, 0, 2.f);
        s_dl->AddRectFilled(ImVec2(c.x - 1.5f, c.y - 7.f), ImVec2(c.x + 1.5f, c.y + 1.f), style::TitleLeft);
        s_dl->AddCircleFilled(ImVec2(c.x, c.y + 6.f), 1.6f, style::TitleLeft);
        break;
    }
    case kMsgWarn:
        s_dl->AddTriangleFilled(ImVec2(c.x, c.y - 10.f), ImVec2(c.x - 11.f, c.y + 8.f),
                                ImVec2(c.x + 11.f, c.y + 8.f), MsgBoxIconColor());
        s_dl->AddRectFilled(ImVec2(c.x - 1.5f, c.y - 4.f), ImVec2(c.x + 1.5f, c.y + 2.f),
                            IM_COL32(0, 0, 0, 255));
        s_dl->AddCircleFilled(ImVec2(c.x, c.y + 4.5f), 1.5f, IM_COL32(0, 0, 0, 255));
        break;
    case kMsgError:
    {
        s_dl->AddRectFilled(ImVec2(c.x - 11.f, c.y - 11.f), ImVec2(c.x + 11.f, c.y + 11.f), MsgBoxIconColor());
        for (int i = -8; i <= 8; ++i)
        {
            s_dl->AddLine(ImVec2(c.x + (float)i, c.y + (float)i), ImVec2(c.x + (float)i + 4.f, c.y + (float)i + 4.f),
                          IM_COL32(255, 255, 255, 255), 2.f);
            s_dl->AddLine(ImVec2(c.x + (float)i + 4.f, c.y + (float)i), ImVec2(c.x + (float)i, c.y + (float)i + 4.f),
                          IM_COL32(255, 255, 255, 255), 2.f);
        }
        break;
    }
    case kMsgYesNo:
    case kMsgOkCancel:
    {
        s_dl->AddRectFilled(ImVec2(c.x - 11.f, c.y - 11.f), ImVec2(c.x + 11.f, c.y + 11.f), MsgBoxIconColor());
        const char* q = (type == kMsgYesNo) ? "?" : "!";
        const ImVec2 qs = ImGui::CalcTextSize(q);
        s_dl->AddText(ImVec2(c.x - qs.x * 0.5f, c.y - qs.y * 0.5f), IM_COL32(255, 255, 255, 255), q);
        break;
    }
    default:
        break;
    }
}

inline void MsgBoxButton(const char* label, int result, bool defaultBtn)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  w  = ts.x + 26.f;
    const ImVec2 a(s_cursor.x, s_cursor.y);
    const ImVec2 b(a.x + w, a.y + style::ButtonH);

    if (Button(label, ImVec2(w, style::ButtonH)))
    {
        s_msgResult = result;
        s_msgActive = false;
    }

    if (defaultBtn && !ImGui::IsAnyItemHovered())
    {
        s_dl->AddRect(ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), AccentColor(),
                      (b.y - a.y) * 0.5f, 0, 1.f);
    }
}

inline void DrawMsgBox()
{
    if (!s_msgActive) { return; }

    const ImVec2 vsize = ImGui::GetIO().DisplaySize;
    const ImVec2 ts    = ImGui::CalcTextSize(s_msgTitle);
    const ImVec2 bs    = ImGui::CalcTextSize(s_msgText);

    const float w = (bs.x > ts.x ? bs.x : ts.x) + 78.f;
    const float h = 32.f + ImGui::GetTextLineHeight() + 20.f + style::ButtonH + 12.f;

    ImGui::SetNextWindowPos(ImVec2((vsize.x - w) * 0.5f, (vsize.y - h) * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, style::Face);

    ImGui::Begin("##msgbox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize |
                                      ImGuiWindowFlags_Modal);

    s_dl     = ImGui::GetWindowDrawList();
    s_msgMin = ImGui::GetWindowPos();
    const ImVec2 wsz = ImGui::GetWindowSize();
    s_msgMax = ImVec2(s_msgMin.x + wsz.x, s_msgMin.y + wsz.y);

    // rounded glass card - no window frame, just a violet accent strip
    const float r = 16.f * style::S;
    style::GlowRound(s_dl, s_msgMin, s_msgMax, r, 0.8f);
    s_dl->AddRectFilledMultiColorRounded(s_msgMin, s_msgMax, style::BgDeep,
                                         style::Face, style::Face, style::BgDeep, style::BgDeep, r,
                                         ImDrawFlags_RoundCornersAll);
    s_dl->AddRect(s_msgMin, s_msgMax, style::AccentDeep, r, 0, 2.f);

    s_dl->AddRectFilled(ImVec2(s_msgMin.x + 14.f, s_msgMin.y + 9.f),
                        ImVec2(s_msgMax.x - 14.f, s_msgMin.y + 10.f), style::Accent);
    s_dl->AddText(ImVec2(s_msgMin.x + 14.f, s_msgMin.y + 14.f), style::AccentGlow, s_msgTitle);

    const ImVec2 iconC(s_msgMin.x + 28.f, s_msgMin.y + 54.f);
    MsgBoxIcon(s_msgType, iconC);

    s_dl->AddText(ImVec2(s_msgMin.x + 52.f, s_msgMin.y + 46.f), style::Text, s_msgText);

    const float btns = (s_msgType == kMsgYesNo || s_msgType == kMsgOkCancel)
                           ? ((s_msgType == kMsgYesNo) ? 2.f : 1.f)
                           : 0.f;
    const float btnW = 78.f;
    const float totalW = (btns + 1.f) * btnW + btns * 6.f;
    float bx = s_msgMax.x - 12.f - totalW;
    const float by = s_msgMax.y - 12.f - style::ButtonH;

    if (s_msgType == kMsgOkCancel)
    {
        s_cursor = ImVec2(bx, by);
        MsgBoxButton("OK", 1, true);
        s_cursor = ImVec2(bx + btnW + 6.f, by);
        MsgBoxButton("Cancel", 2, false);
    }
    else if (s_msgType == kMsgYesNo)
    {
        s_cursor = ImVec2(bx, by);
        MsgBoxButton("Yes", 1, true);
        s_cursor = ImVec2(bx + btnW + 6.f, by);
        MsgBoxButton("No", 0, false);
    }
    else
    {
        s_cursor = ImVec2(bx, by);
        MsgBoxButton("OK", 1, true);
    }

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape, true))
    {
        s_msgResult = 0;
        s_msgActive = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Enter, true))
    {
        s_msgResult = 1;
        s_msgActive = false;
    }
}

inline void PushModal()
{
    s_popupBlockClicks = true;
}

inline void PopModal()
{
    s_popupBlockClicks = false;
}

inline void BeginDisabled()
{
    ImGui::BeginDisabled();
}

inline void EndDisabled()
{
    ImGui::EndDisabled();
}

inline bool RadioButtonMenu(const char* label, bool* selected)
{
    return RadioButton(label, selected);
}
/* ---------------------------------------------------------------------------
 * menu.cpp - menu bar, dropdowns, context menus
 * ------------------------------------------------------------------------ */
inline constexpr int kMaxMenuItems = 12;

inline int   s_openMenu      = -1;
inline bool  s_menuPressed   = false;
inline bool  s_mouseReleased = false;
inline char  s_mouseOwner[32] = { 0 };
inline float s_menuOpenX    = 0.f;
inline float s_menuOpenY    = 0.f;
inline bool  s_ctxOpen       = false;

struct MenuItem
{
    char  label[64];
    char  sub[8];
    int   id;
    bool  checkable;
    bool  checked;
};

struct MenuDef
{
    const char* label;
    const char* owner;
};

inline constexpr MenuDef kMenus[] =
{
    { "File",       "file" },
    { "Settings",   "settings" },
    { "Skins",      "skins" },
    { "Help",       "help" },
};
inline constexpr int kMenuCount = 4;

inline bool MenuIsOpen() { return s_openMenu >= 0 || s_ctxOpen; }

inline bool MenuItemClicked() { return s_menuPressed; }


inline void MenuBarItem(const char* label)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  h  = style::MenuBarH;
    const float  x  = s_cursor.x;
    const float  y  = s_cursor.y;

    int idx = s_openMenu;
    bool opened = false;
    for (int i = 0; i < kMenuCount; ++i)
    {
        if (strcmp(kMenus[i].label, label) == 0) { idx = i; }
    }

    const ImVec2 a(x, y);
    const ImVec2 b(x + ts.x + 16.f, y + h);
    const bool hovered = PopUpsAreClosed() && ImGui::IsMouseHoveringRect(a, b);

    if (idx >= 0 && s_openMenu == idx)
    {
        s_dl->AddRectFilled(a, b, style::Selection);
        s_dl->AddText(ImVec2(a.x + 8.f, y + (h - ts.y) * 0.5f), style::SelText, label);
    }
    else if (hovered)
    {
        s_dl->AddRectFilled(a, b, style::Highlight);
        s_dl->AddText(ImVec2(a.x + 8.f, y + (h - ts.y) * 0.5f), style::Text, label);
    }
    else
    {
        s_dl->AddText(ImVec2(a.x + 8.f, y + (h - ts.y) * 0.5f), style::Text, label);
    }

    if (hovered && s_openMenu >= 0 && s_openMenu != idx)
    {
        s_openMenu = idx;
        s_menuOpenX = x;
        s_menuOpenY = y + h;
    }
    if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && s_openMenu < 0)
    {
        s_openMenu = idx;
        s_menuOpenX = x;
        s_menuOpenY = y + h;
        opened = true;
    }

    s_cursor.x = b.x;
    (void)opened;
}


inline void DropDownItem(const char* label, int id, bool checkable = false, bool checked = false,
                         bool separator = false, int sub = -1, bool enabled = true)
{
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const float  ih = ImGui::GetTextLineHeight() + 6.f;
    const float  w  = (s_ctxOpen ? 110.f : 150.f);

    const ImVec2 a(s_menuOpenX, s_cursor.y);
    const ImVec2 b(s_menuOpenX + w, s_cursor.y + ih);

    const bool hovered = enabled && PopUpsAreClosed() && ImGui::IsMouseHoveringRect(a, b);

    if (separator)
    {
        s_dl->AddRectFilled(ImVec2(a.x + 6.f, s_cursor.y + ih * 0.5f), ImVec2(b.x - 6.f, s_cursor.y + ih * 0.5f + 1.f),
                            style::Shadow);
        AddItem(ImVec2(w, ih));
        return;
    }

    if (hovered)
    {
        s_dl->AddRectFilled(a, b, style::Selection);
    }

    const bool disabled = !enabled;
    const ImU32 col = disabled ? style::TextDisabled : (hovered ? style::SelText : style::Text);

    if (checkable)
    {
        const float bx = a.x + 7.f;
        const float by = s_cursor.y + ih * 0.5f;
        if (checked)
        {
            s_dl->AddLine(ImVec2(bx, by), ImVec2(bx + 3.f, by + 3.f), col, 1.6f);
            s_dl->AddLine(ImVec2(bx + 3.f, by + 3.f), ImVec2(bx + 8.f, by - 3.f), col, 1.6f);
        }
    }

    s_dl->AddText(ImVec2(a.x + 20.f, s_cursor.y + 3.f), col, label);

    if (sub >= 0)
    {
        const ImVec2 t(b.x - 11.f, s_cursor.y + ih * 0.5f - 2.f);
        s_dl->AddTriangleFilled(ImVec2(t.x, t.y), ImVec2(t.x - 4.f, t.y - 3.5f), ImVec2(t.x - 4.f, t.y + 3.5f), col);
    }

    if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        s_menuPressed = true;
        CopyStr(s_mouseOwner, "menu", sizeof(s_mouseOwner));
        if (sub < 0) { s_openMenu = -1; }
    }

    AddItem(ImVec2(w, ih));
}

inline void BeginDropDown(const char* owner)
{
    const ImVec2 ts = ImGui::CalcTextSize(owner);
    const float  w  = 150.f;
    const int    idx = s_openMenu;
    (void)idx; (void)ts; (void)w;
    s_cursor = ImVec2(s_menuOpenX, s_menuOpenY);
    CopyStr(s_mouseOwner, owner, sizeof(s_mouseOwner));
}

inline void EndDropDown()
{
    const ImVec2 a(s_menuOpenX, s_menuOpenY);
    const ImVec2 b(s_menuOpenX + (s_ctxOpen ? 110.f : 150.f), s_cursor.y);
    // Items are already drawn, so only the rounded frame goes on top.
    style::GlowRound(s_dl, a, b, 10.f, 0.55f);
    s_dl->AddRect(a, b, style::AccentDeep, 10.f, 0, 1.5f);
    s_cursor.y = s_winMin.y + style::HeaderH + 20.f;
    s_cursor.x = s_contentMin.x;
}

inline void EndDropDownAuto()
{
    EndDropDown();
}

inline void OpenContext(const char* owner, float x, float y)
{
    if (strcmp(s_mouseOwner, owner) == 0 && s_ctxOpen)
    {
        s_ctxOpen = false;
        s_menuPressed = false;
        return;
    }
    s_ctxOpen      = true;
    s_menuOpenX    = x;
    s_menuOpenY    = y;
    s_menuPressed  = false;
    CopyStr(s_mouseOwner, owner, sizeof(s_mouseOwner));
}

inline bool IsContextOpen(const char* owner)
{
    return s_ctxOpen && strcmp(s_mouseOwner, owner) == 0;
}

inline bool BeginContext(const char* owner, float x, float y)
{
    if (!IsContextOpen(owner)) { return false; }
    BeginDropDown(owner);
    return true;
}

inline void EndContext()
{
    EndDropDown();
    if (s_ctxOpen && !PopUpsAreClosed()) { }
    s_ctxOpen = false;
}

inline void MenuEnd()
{
    if (!PopUpsAreClosed()) { }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !s_menuPressed)
    {
        if (!ImGui::IsMouseHoveringRect(ImVec2(s_menuOpenX, s_menuOpenY),
                                        ImVec2(s_menuOpenX + 160.f, s_cursor.y)))
        {
            s_openMenu = -1;
            s_ctxOpen  = false;
        }
    }
}

inline bool WasMenuItemClicked() { return s_menuPressed; }
/* ---------------------------------------------------------------------------
 * toolbar.cpp
 * ------------------------------------------------------------------------ */




inline void EndWindow() { }

/* ---------------------------------------------------------------------------
 * listbox.cpp
 * ------------------------------------------------------------------------ */
inline int ListBox(const char* items, int count, int selected, int height, const char* id)
{
    const float itemH = ImGui::GetTextLineHeight() + 3.f;
    const float boxH  = height > 0 ? (float)height : itemH * (float)count + 4.f;

    const ImVec2 a(s_cursor.x, s_cursor.y);
    const ImVec2 b(s_cursor.x + ContentWidth(), s_cursor.y + boxH);

    style::FillFace(s_dl, a, b);
    style::Sunken(s_dl, a, b);

    const ImVec2 ia(a.x + 2.f, a.y + 2.f);
    const ImVec2 ib(b.x - 2.f, b.y - 2.f);

    int   chosen = selected;
    float iy     = ia.y;
    int   i      = 0;
    const char* cur = items;
    const bool  enabled = PopUpsAreClosed();

    while (*cur && i < count)
    {
        const char*  next = strchr(cur, '\x01');
        const ImVec2 ia2(ia.x, iy);
        const ImVec2 ib2(ib.x, iy + itemH);
        const bool   ih  = enabled && ImGui::IsMouseHoveringRect(ia2, ib2) &&
                           iy + itemH > ia.y && iy < ib.y;

        if (i == selected)      { s_dl->AddRectFilled(ia2, ib2, style::Selection); }
        else if (ih)            { s_dl->AddRectFilled(ia2, ib2, style::Highlight); }

        if (ib2.y > ia.y && ia2.y < ib.y)
        {
            s_dl->AddText(ImVec2(ia2.x + 3.f, ia2.y + 1.5f), i == selected ? style::SelText : style::Text, cur);
        }
        if (ih && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            chosen = i;
        }
        iy += itemH;
        ++i;
        if (!next) { break; }
        cur = next + 1;
    }

    AddItem(ImVec2(ContentWidth(), boxH));
    (void)id;
    return chosen;
}

/* ---------------------------------------------------------------------------
 * assets: micross font + title-bar icon
 * ------------------------------------------------------------------------ */
inline ImFont* s_font = nullptr;
inline ImTextureID s_icon = 0;

inline ImFont* Win95Font() { return s_font; }

inline ImTextureID IconTex() { return s_icon; }

inline void BuildFonts()
{
    if (s_font) { return; }
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;   // static array, ImGui must not free it
    cfg.OversampleH           = 1;
    cfg.OversampleV           = 2;
    cfg.PixelSnapH            = true;
    cfg.SizePixels            = kFontSize;
    s_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)MicrosFont_data,
                                                       (int)MicrosFont_size, kFontSize, &cfg);
}

#if defined(W95_HAS_GLES) && !defined(NIKU_HARNESS)
inline void BuildIcons()
{
    if (s_icon) { return; }

    int w = 0, h = 0, comp = 0;
    unsigned char* px = stbi_load_from_memory(Win95IconBytes, (int)sizeof(Win95IconBytes), &w, &h, &comp, 4);
    if (!px) { return; }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, px);
    stbi_image_free(px);

    s_icon = (ImTextureID)(intptr_t)tex;
}
#else
inline void BuildIcons() { }
#endif

inline void PushFont()
{
    if (s_font) { ImGui::PushFont(s_font); }
}

inline void PopFont()
{
    if (s_font) { ImGui::PopFont(); }
}

inline void ApplyStyle()
{
    ImGuiStyle& st = ImGui::GetStyle();
    st.WindowPadding     = ImVec2(0.f, 0.f);
    st.FramePadding      = ImVec2(1.f, 1.f);
    st.ItemSpacing       = ImVec2(4.f, 2.f);
    st.ItemInnerSpacing  = ImVec2(4.f, 4.f);
    st.CellPadding       = ImVec2(0.f, 0.f);
    st.ScrollbarSize     = style::ScrollbarW;
    st.GrabMinSize       = style::ScrollMinThumb;
    st.WindowBorderSize  = 0.f;
    st.ChildBorderSize   = 0.f;
    st.PopupBorderSize   = 0.f;
    st.FrameRounding     = 0.f;
    st.GrabRounding      = 0.f;
    st.ScrollbarRounding = 0.f;
    st.WindowRounding    = 0.f;

    // SHADOW MODZ: dark surfaces, violet accents (see ShadowBrand.h).
    st.Colors[ImGuiCol_Text]            = ImGui::ColorConvertU32ToFloat4(style::Text);
    st.Colors[ImGuiCol_TextDisabled]    = ImGui::ColorConvertU32ToFloat4(style::TextDisabled);
    st.Colors[ImGuiCol_WindowBg]        = ImGui::ColorConvertU32ToFloat4(style::BgDeep);
    st.Colors[ImGuiCol_PopupBg]         = ImGui::ColorConvertU32ToFloat4(style::GroupBg);
    st.Colors[ImGuiCol_Border]          = ImGui::ColorConvertU32ToFloat4(style::AccentDeep);
    st.Colors[ImGuiCol_FrameBg]         = ImGui::ColorConvertU32ToFloat4(style::Field);
    st.Colors[ImGuiCol_FrameBgHovered]  = ImGui::ColorConvertU32ToFloat4(style::Light);
    st.Colors[ImGuiCol_FrameBgActive]   = ImGui::ColorConvertU32ToFloat4(style::Highlight);
    st.Colors[ImGuiCol_TitleBg]         = ImGui::ColorConvertU32ToFloat4(style::AccentDeep);
    st.Colors[ImGuiCol_TitleBgActive]   = ImGui::ColorConvertU32ToFloat4(style::Accent);
    st.Colors[ImGuiCol_Button]          = ImGui::ColorConvertU32ToFloat4(style::BtnBot);
    st.Colors[ImGuiCol_ButtonHovered]   = ImGui::ColorConvertU32ToFloat4(style::BtnTopHover);
    st.Colors[ImGuiCol_ButtonActive]    = ImGui::ColorConvertU32ToFloat4(style::Accent);
    st.Colors[ImGuiCol_Header]          = ImGui::ColorConvertU32ToFloat4(style::AccentDeep);
    st.Colors[ImGuiCol_HeaderHovered]   = ImGui::ColorConvertU32ToFloat4(style::Selection);
    st.Colors[ImGuiCol_HeaderActive]    = ImGui::ColorConvertU32ToFloat4(style::Accent);
    st.Colors[ImGuiCol_SliderGrab]      = ImGui::ColorConvertU32ToFloat4(style::Accent);
    st.Colors[ImGuiCol_SliderGrabActive]= ImGui::ColorConvertU32ToFloat4(style::AccentGlow);
    st.Colors[ImGuiCol_CheckMark]       = ImGui::ColorConvertU32ToFloat4(style::AccentGlow);
    st.Colors[ImGuiCol_ScrollbarBg]     = ImGui::ColorConvertU32ToFloat4(style::BgDeep);
    st.Colors[ImGuiCol_ScrollbarGrab]   = ImGui::ColorConvertU32ToFloat4(style::AccentDeep);
    st.Colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::ColorConvertU32ToFloat4(style::Accent);
    st.Colors[ImGuiCol_ScrollbarGrabActive]  = ImGui::ColorConvertU32ToFloat4(style::AccentGlow);
}
/* ---------------------------------------------------------------------------
 * launcher.cpp - overlay entry point (menu starts hidden, like upstream)
 * ------------------------------------------------------------------------ */
inline bool s_launcherVisible = false;
inline bool s_launcherOpen    = false;

inline bool LauncherVisible() { return s_launcherVisible; }

inline void ToggleLauncher()
{
    s_launcherOpen    = !s_launcherOpen;
    s_launcherVisible = !s_launcherOpen;
    s_openMenu        = -1;
    s_ctxOpen         = false;
    s_scrollY         = 0.f;
}

/* SHADOW MODZ floating icon: a draggable, pulsing violet disc instead of the
 * old grey "Niku Mods" rectangle. Tap opens the menu, drag moves it, and it
 * never leaves the screen. Draws itself, so it adds no ImGui items. */
inline void DrawLauncher()
{
    if (!s_launcherOpen) { return; }

    const float      R = 27.f;
    static ImVec2    s_pos   = ImVec2(18.f + R, 18.f + R);   // centre of the disc
    static bool      s_drag  = false;
    static bool      s_moved = false;
    static ImVec2    s_grab  = ImVec2(0.f, 0.f);

    const ImVec2 m = ImGui::GetIO().MousePos;
    const float  dx = m.x - s_pos.x;
    const float  dy = m.y - s_pos.y;
    const bool   hovered = (dx * dx + dy * dy) <= (R * R);

    // --- drag / tap -------------------------------------------------------
    if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        s_drag  = true;
        s_moved = false;
        s_grab  = ImVec2(dx, dy);
    }
    if (s_drag)
    {
        const ImVec2 np(m.x - s_grab.x, m.y - s_grab.y);
        if (std::fabs(np.x - s_pos.x) > 2.f || std::fabs(np.y - s_pos.y) > 2.f) { s_moved = true; }

        // clamp so the disc always stays fully on screen
        const ImVec2 disp = ImGui::GetIO().DisplaySize;
        s_pos.x = (np.x < R) ? R : ((np.x > disp.x - R) ? disp.x - R : np.x);
        s_pos.y = (np.y < R) ? R : ((np.y > disp.y - R) ? disp.y - R : np.y);

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            s_drag = false;
            if (!s_moved)          // a tap, not a drag -> open the menu
            {
                ToggleLauncher();
                return;
            }
        }
    }

    // --- paint ------------------------------------------------------------
    const float pad = R + 16.f;   // room for the halo
    ImGui::SetNextWindowPos(ImVec2(s_pos.x - pad, s_pos.y - pad), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.f);
    // Fixed size, not AlwaysAutoResize: the launcher draws directly and adds no
    // ImGui items, so auto-fit would collapse it to a zero-size clipped window.
    ImGui::SetNextWindowSize(ImVec2(pad * 2.f, pad * 2.f), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    if (ImGui::Begin("##shadow_launcher", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const float pulse = style::Pulse(1.3f);
        const int   ar = ShadowBrand::Accent.r;
        const int   ag = ShadowBrand::Accent.g;
        const int   ab = ShadowBrand::Accent.b;

        // breathing halo rings
        for (int i = 3; i >= 1; --i)
        {
            const float rr = R + 2.f + (float)i * 4.f + pulse * 4.f;
            dl->AddCircle(s_pos, rr, IM_COL32(ar, ag, ab, 14 + i * 11), 30, 2.f);
        }

        // disc: dark base + lighter top half so it reads as a lit sphere
        dl->AddCircleFilled(s_pos, R, style::BgDeep, 32);
        const float kPi = 3.14159265f;
        dl->PathArcTo(s_pos, R - 1.5f, kPi, kPi * 2.0f, 24);
        dl->PathFillConvex(hovered ? style::BtnTopHover : style::BtnTop);
        dl->AddCircleFilled(s_pos, R - 3.f, style::BtnBot, 32);

        // violet rim + inner hairline
        dl->AddCircle(s_pos, R - 1.f, hovered ? style::AccentGlow : style::Accent, 32, hovered ? 3.f : 2.f);
        dl->AddCircle(s_pos, R - 5.f, IM_COL32(ar, ag, ab, 70), 32, 1.f);

        // centre: brand icon texture if it built, otherwise the SM glyph
        const float ir = R - 11.f;
        if (IconTex() != 0)
        {
            dl->AddImage(IconTex(), ImVec2(s_pos.x - ir, s_pos.y - ir),
                         ImVec2(s_pos.x + ir, s_pos.y + ir));
        }
        else
        {
            const char* txt = ShadowBrand::LauncherText;
            const ImVec2 ts = ImGui::CalcTextSize(txt);
            const ImVec2 tp(s_pos.x - ts.x * 0.5f, s_pos.y - ts.y * 0.5f);
            dl->AddText(ImVec2(tp.x + 1.f, tp.y + 1.f), IM_COL32(0, 0, 0, 180), txt);
            dl->AddText(tp, hovered ? style::AccentGlow : style::Text, txt);
        }

        ImGui::End();
    }
    ImGui::PopStyleVar(2);
}

/* ---------------------------------------------------------------------------
 * pages
 * ------------------------------------------------------------------------ */
namespace vars
{
struct EspColours
{
    float box[4]      = { 1.00f, 0.35f, 0.35f, 1.00f };
    float line[4]     = { 1.00f, 1.00f, 1.00f, 1.00f };
    float health[4]   = { 0.20f, 0.90f, 0.30f, 1.00f };
    float skeleton[4] = { 1.00f, 0.85f, 0.20f, 1.00f };
    float name[4]     = { 1.00f, 1.00f, 1.00f, 1.00f };
};

inline EspColours espColours;

inline bool  skinChanger  = false;
inline int   skinIndex    = 0;
inline bool  skinRandom   = false;
inline bool  showSkeleton = false;
inline bool  box3d        = false;
inline float fovChanger   = 120.f;
inline int   renderDist   = 300;
inline bool  worldGrid    = false;
inline bool  radarHack    = false;
inline bool  btEnabled    = false;
inline bool  btMode       = false;
inline int   exploitSel   = 0;
inline float lzSpeed      = 1.0f;
inline bool  localAntiAim = false;
inline bool  antiSpin     = false;
inline bool  fakeLag      = false;
inline bool  desync       = false;
inline bool  tpOnKey     = false;
inline int   tpKey        = ImGuiKey_Insert;
inline bool  useAccent    = true;
inline float accentHsv[4] = { 0.72f, 1.0f, 1.0f, 1.0f };
}

inline const char* kExploits[] = { "Level 2", "Level 6", "Level 10", "Level 30", "Level 60", "Level 100" };
inline const char* kSkins[]    = { "Default", "Classic", "Warrior", "Night", "Gold", "Toxic" };
inline const char* kPresets[]  = { "Legit", "Rage", "Streamer", "Nuke" };

inline char s_nameBuf[32]   = "Player";
inline char s_clipBuf[256]  = "";
inline int  s_accentSel     = 0;

inline char* BuildItems(const char* const* arr, int count, char* buf, size_t cap)
{
    buf[0] = '\0';
    size_t used = 0;
    for (int i = 0; i < count; ++i)
    {
        const size_t n = strlen(arr[i]);
        if (used + n + 2 >= cap) { break; }
        memcpy(buf + used, arr[i], n);
        used += n;
        buf[used++] = '\x01';
    }
    buf[used] = '\0';
    return buf;
}

inline void PageAimbot()
{
    BeginGroup("Aimbot", 0);
    Checkbox("Aimbot", &Aimbot);
    if (Aimbot)
    {
        Indent();
        Checkbox("Silent Aim", &SilentAimv222);
        Checkbox("Headshot", &Headshot);
        Checkbox("Ignore Knocked", &ignoreKnocked);
        Checkbox("Visible Check", &checkVisible2);
        Size        = Slider("Size", Size, 1.f, 500.f, "%.0f");
        aimSmoothness = Slider("Smoothness", aimSmoothness, 0.f, 1.f, "%.2f");
        Unindent();
    }
    EndGroup();

    BeginGroup("Recoil", 1);
    Checkbox("No Recoil", &norecoil);
    if (norecoil)
    {
        Indent();
        Label("Recoil control is always full");
        Unindent();
    }
    EndGroup();

    SetColumns(2);
}

inline void PageVisuals()
{
    static char itemBuf[512];

    BeginGroup("ESP", 0);
    Checkbox("Enabled", &esp);
    if (esp)
    {
        Indent();
        Checkbox("Box", &Config.ESP.Box);
        Checkbox("Line", &Config.ESP.Line);
        Checkbox("Health", &Config.ESP.Health);
        Checkbox("Skeleton", &Config.ESP.Skeleton);
        Checkbox("Distance", &Config.ESP.Distancia);
        Checkbox("Name", &Config.ESP.IsName);
        Checkbox("Target", &Config.ESP.Target);
        Unindent();
    }
    EndGroup();

    BeginGroup("Colours", 1);
    ColorEdit4("Box",      vars::espColours.box,      true);
    ColorEdit4("Line",     vars::espColours.line,     true);
    ColorEdit4("Health",   vars::espColours.health,   true);
    ColorEdit4("Skeleton", vars::espColours.skeleton, true);
    ColorEdit4("Name",     vars::espColours.name,     true);
    EndGroup();

    (void)itemBuf;
    SetColumns(2);
}

inline void PageWorld()
{
    BeginGroup("Environment", 0);
    Checkbox("World Grid", &vars::worldGrid);
    Checkbox("Radar Hack", &vars::radarHack);
    vars::renderDist = (int)Slider("Render Distance", (float)vars::renderDist, 50.f, 1000.f, "%.0f");
    EndGroup();

    BeginGroup("Exploits", 1);
    static char expBuf[256];
    const char* pick = Combo("Level", vars::exploitSel, BuildItems(kExploits, 6, expBuf, sizeof(expBuf)), 6);
    if (pick)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (strcmp(pick, kExploits[i]) == 0) { vars::exploitSel = i; }
        }
    }
    Checkbox("Back Teleport", &vars::tpOnKey);
    if (vars::tpOnKey)
    {
        Indent();
        Keybind("Bind", &vars::tpKey);
        Unindent();
    }
    EndGroup();

    SetColumns(2);
}

inline void PageSkins()
{
    static char skinBuf[128];

    BeginGroup("Skin Changer", 0);
    Checkbox("Enabled", &vars::skinChanger);
    if (vars::skinChanger)
    {
        Indent();
        const char* pick = Combo("Skin", vars::skinIndex, BuildItems(kSkins, 6, skinBuf, sizeof(skinBuf)), 6);
        if (pick)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (strcmp(pick, kSkins[i]) == 0) { vars::skinIndex = i; }
            }
        }
        Checkbox("Random", &vars::skinRandom);
        Unindent();
    }
    EndGroup();

    BeginGroup("Paint", 1);
    Checkbox("Use Accent", &vars::useAccent);
    if (vars::useAccent)
    {
        Indent();
        ColorEdit3("Accent", vars::accentHsv, true);
        vars::useAccent = true;
        Unindent();
    }
    SetAccent(vars::accentHsv, vars::useAccent);
    EndGroup();

    SetColumns(2);
}

inline void PagePlayers()
{
    BeginGroup("Players", 0);
    InputText("Name", s_nameBuf, sizeof(s_nameBuf), false);
    Checkbox("Anti Aim", &vars::localAntiAim);
    Checkbox("Anti Spin", &vars::antiSpin);
    Checkbox("Fake Lag", &vars::fakeLag);
    Checkbox("Desync", &vars::desync);
    EndGroup();

    BeginGroup("Local", 1);
    Checkbox("Enemy Count", &EnemyCount);
    Checkbox("Game Timer", &GameTimer);
    Checkbox("High FPS", &HighFps);
    vars::fovChanger = Slider("FOV", vars::fovChanger, 60.f, 140.f, "%.0f");
    ProgressBar("Health", 0.75f);
    EndGroup();

    SetColumns(2);
}

inline void PageMisc()
{
    BeginGroup("Misc", 0);
    Checkbox("Speed Hack", &SpeedHack);
    Checkbox("Teleport Marker", &TeleportMark1);
    if (TeleportMark1) { HasMapMarker = true; }
    else               { HasMapMarker = false; }
    Checkbox("Show Skeleton", &vars::showSkeleton);
    Checkbox("3D Box", &vars::box3d);
    EndGroup();

    BeginGroup("Config", 1);
    static char presetBuf[128];
    s_accentSel = ListBox(BuildItems(kPresets, 4, presetBuf, sizeof(presetBuf)), 4, s_accentSel, 0, "presets");
    if (Button("Save Preset"))
    {
        ShowInfo("Saved", "Preset saved to disk.");
    }
    if (Button("Load Preset"))
    {
        ShowInfo("Loaded", "Preset loaded from disk.");
    }
    EndGroup();

    SetColumns(2);
}

inline void PageConfig()
{
    BeginGroup("Presets", 0);
    if (Button("Reset All"))
    {
        ShowYesNo("Reset", "Reset every setting to default?");
    }
    if (Button("Save Config"))
    {
        ShowInfo("Saved", "Configuration saved.");
    }
    if (Button("Load Config"))
    {
        ShowInfo("Loaded", "Configuration loaded.");
    }
    EndGroup();

    BeginGroup("About", 1);
    {
        char line[96];
        std::snprintf(line, sizeof(line), "%s %s", ShadowBrand::BrandName, ShadowBrand::Version);
        Label(line);
        Label(ShadowBrand::BuildTag);

        // Telegram: rendered as a dead violet button reading "COMING SOON"
        // until ShadowBrand::TelegramUrl is filled and TelegramLive is true.
        char tg[96];
        std::snprintf(tg, sizeof(tg), "%s - %s", ShadowBrand::TelegramLabel, ShadowBrand::TelegramText);
        if (AccentButton(tg, ShadowBrand::TelegramLive) && ShadowBrand::TelegramUrl[0] != '\0')
        {
            OpenUrl(ShadowBrand::TelegramUrl);
        }
    }
    EndGroup();

    SetColumns(2);
}
/* ---------------------------------------------------------------------------
 * tabs.cpp / gui.cpp - top level
 * ------------------------------------------------------------------------ */
inline void Render()
{
    // Insert is the keyboard way back to the floating icon.
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, true))
    {
        ToggleLauncher();
    }

    if (s_launcherOpen)
    {
        DrawLauncher();
        return;
    }

    ApplyStyle();
    PushFont();

    DrawMsgBox();

    const bool open = !s_launcherVisible;
    if (!open || !IsMsgBoxOpen())
    {
        PushModal();
    }

    // One floating panel, every feature stacked in scrollable sections.
    BeginPanel();

    PageAimbot();
    PageVisuals();
    PageWorld();
    PageSkins();
    PagePlayers();
    PageMisc();
    PageConfig();

    EndPanel();

    MenuEnd();

    PopModal();

    if (CloseRequested())
    {
        ToggleLauncher();
    }

    PopFont();
}
/* ---------------------------------------------------------------------------
 * public entry points (called from Main.cpp / Gui.hpp)
 * ------------------------------------------------------------------------ */
inline void Win95BuildFonts()   { ApplyStyle(); BuildFonts(); }
inline void Win95InitTextures() { BuildIcons(); }
inline void Win95Menu()         { Render(); }

}

namespace Win95UI = ::W95;
