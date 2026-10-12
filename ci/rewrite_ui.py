#!/usr/bin/env python3
"""One-shot rewrite of the Win95UI.h layout layer.

Replaces the XP window chrome (title bar / menu bar / toolbar / tabs / status
bar / group boxes) with a modern floating glass panel: rounded card, drag
header, collapsible-free section list holding every real feature, and restyled
widgets (pill switches, slim sliders, rounded buttons).

Run from the repo root:  python3 ci/rewrite_ui.py
"""
import re
import sys

PATH = "ShadowModz/app/src/main/jni/NikuMods/Win95UI.h"
src = open(PATH, encoding="utf-8").read()


def swap(old, new, label):
    """Replace an exact block, asserting it appears exactly once."""
    global src
    n = src.count(old)
    if n != 1:
        sys.exit("FAIL %s: found %d copies" % (label, n))
    src = src.replace(old, new)
    print("ok   %s" % label)


def swap_func(sig_regex, new, label):
    """Replace a whole top-level function matched by its signature line."""
    global src
    m = re.search(sig_regex, src, re.M)
    if not m:
        sys.exit("FAIL %s: signature not found" % label)
    start = m.start()
    end = src.index("\n}\n", start) + len("\n}\n")
    src = src[:start] + new + src[end:]
    print("ok   %s" % label)


# ---------------------------------------------------------------- sizes ----
swap("""constexpr float ButtonH        = 23. * S;""",
"""constexpr float ButtonH        = 23. * S;

/* SHADOW MODZ floating-panel metrics (no window chrome any more) */
constexpr float PanelW        = 340.f * S;
constexpr float PanelH        = 470.f * S;
constexpr float PanelRadius   = 18.f * S;
constexpr float HeaderH       = 44.f * S;   // logo + name + close, drag handle
constexpr float FooterH       = 28.f * S;   // live dot + uid
constexpr float SectionHeadH  = 26.f * S;   // feature section title
constexpr float SwitchW       = 34.f * S;   // pill switch
constexpr float SwitchH       = 18.f * S;""",
     "panel size constants")

# ------------------------------------------------------------ glow round ---
swap("""// Triangle wave 0..1 on the frame clock, for the launcher pulse.
inline float Pulse(float speed = 1.6f)
{
    const float t = (float)ImGui::GetTime() * speed;
    const float f = t - std::floor(t);
    return (f < 0.5f) ? (f * 2.f) : ((1.f - f) * 2.f);
}""",
"""// Triangle wave 0..1 on the frame clock, for the launcher pulse.
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
}""",
     "GlowRound helper")

# ------------------------------------------------------------- columns -----
swap_func(r"inline void SetupColumns\(\)\n\{",
"""inline void SetupColumns()
{
    // Single scrolling column - the floating panel stacks its sections.
    s_colX[0] = s_contentMin.x;
    s_colW[0] = s_contentMax.x - s_contentMin.x;
    s_colY[0] = s_contentMin.y - s_scrollY;
}""",
     "SetupColumns")

swap_func(r"inline void SetColumns\(int count\)\n\{",
"""inline void SetColumns(int count)
{
    (void)count;
    // Everything is one column now; kept so the pages stay untouched.
}""",
     "SetColumns")

# --------------------------------------------------------------- groups ----
swap_func(r"inline bool BeginGroup\(const char\* label, int column\)\n\{",
"""inline bool BeginGroup(const char* label, int column)
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
}""",
     "BeginGroup -> section header")

swap_func(r"inline void EndGroup\(\)\n\{",
"""inline void EndGroup()
{
    // No box to close: sections are separated by their header + divider.
    s_maxColBottom = s_cursor.y + style::GroupGap + 6.f;
}""",
     "EndGroup")

# ---------------------------------------------------------------- panel ----
swap_func(r"inline void BeginMain\(const char\* title\)\n\{",
r"""/* SHADOW MODZ floating panel: a rounded glass card with no window chrome.
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
                                         style::Face, style::Face, style::BgDeep, style::BgDeep, r, 0);
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
                                         12.f, 0);
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
}""",
     "BeginMain -> BeginPanel")

swap_func(r"inline void EndMain\(\)\n\{",
r"""inline void EndPanel()
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
        const float thumbH = ImMax(30.f, trackH * (availH / contentHeight));
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
}""",
     "EndMain -> EndPanel")

# -------------------------------------------------------------- widgets ----
swap_func(r"inline bool Checkbox\(const char\* label, bool\* value\)\n\{",
r"""inline bool Checkbox(const char* label, bool* value)
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
}""",
     "Checkbox -> pill switch")

swap_func(r"inline float Slider\(const char\* label, float value, float vmin, float vmax, const char\* fmt\)\n\{",
r"""inline float Slider(const char* label, float value, float vmin, float vmax, const char* fmt)
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
}""",
     "Slider -> slim slider")

swap_func(r"inline bool Button\(const char\* label, const ImVec2& size\)\n\{",
r"""inline bool Button(const char* label, const ImVec2& size)
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
                                         hovered ? style::BtnBotHover : style::BtnBot, r, 0);
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
}""",
     "Button -> rounded pill")

swap_func(r"inline bool AccentButton\(const char\* label, const ImVec2& size, bool enabled = true\)\n\{",
r"""inline bool AccentButton(const char* label, const ImVec2& size, bool enabled = true)
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
                                             hovered ? style::Accent : style::AccentDeep, r, 0);
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
}""",
     "AccentButton -> rounded pill")

# ---------------------------------------------------------------- render ---
swap_func(r"inline void Render\(\)\n\{",
r"""inline void Render()
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
}""",
     "Render -> floating panel with all sections")

open(PATH, "w", encoding="utf-8").write(src)
print("\nrewrote", PATH)
