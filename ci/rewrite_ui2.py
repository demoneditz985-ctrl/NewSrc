#!/usr/bin/env python3
"""Second pass: strip the leftover XP chrome (title buttons, menu bar,
toolbar, tab strip, resize grip, XP scrollbar, XP message box) and restyle the
remaining popups so no window frame survives anywhere.

Run from the repo root:  python3 ci/rewrite_ui2.py
"""
import re
import sys

PATH = "ShadowModz/app/src/main/jni/NikuMods/Win95UI.h"
src = open(PATH, encoding="utf-8").read()


def swap(old, new, label):
    global src
    n = src.count(old)
    if n != 1:
        sys.exit("FAIL %s: found %d copies" % (label, n))
    src = src.replace(old, new)
    print("ok   %s" % label)


def drop_line(pattern, label):
    """Delete every line matching a regex (whole line, incl. newline)."""
    global src
    new, n = re.subn(pattern, "", src, flags=re.M)
    if n == 0:
        sys.exit("FAIL %s: no line matched" % label)
    src = new
    print("ok   %s (%d line(s))" % (label, n))


def drop_span(start_regex, end_regex, label):
    """Delete everything from the start match up to (excluding) the end match."""
    global src
    a = re.search(start_regex, src, re.M)
    b = re.search(end_regex, src, re.M)
    if not a or not b or b.start() < a.start():
        sys.exit("FAIL %s: span not found" % label)
    src = src[:a.start()] + src[b.start():]
    print("ok   %s" % label)


def drop_func(sig_regex, label):
    """Delete a whole top-level function/block matched by its first line."""
    global src
    m = re.search(sig_regex, src, re.M)
    if not m:
        sys.exit("FAIL %s: not found" % label)
    end = src.index("\n}\n", m.start()) + len("\n}\n")
    src = src[:m.start()] + src[end:]
    print("ok   %s" % label)


# ------------------------------------------------------------- banner -----
swap("""/* ===========================================================================
 * SHADOW MODZ shell  (Windows-XP chrome, dark black + violet skin)
 * ---------------------------------------------------------------------------
 *   font: NikuMods/Win95Assets/MicrosFont.hpp   (MS Sans Serif, 16px)
 *   ico : NikuMods/Win95Assets/Win95Icon.hpp    (SHADOW MODZ title-bar icon)
 *   brand: NikuMods/ShadowBrand.h               (every string + colour)
 *
 * Original upstream Win95 desktop shell, rebranded and reskinned:
 *   - HostMode::Standalone / HWND / win32 SetWindowPos removed. The window is
 *     always embedded, so dragging uses ImGui::SetWindowPos("##win95") and
 *     resizing ImGui::SetWindowSize("##win95"), which is exactly what the
 *     upstream HostMode::Embedded path already did.
 *   - ShellExecuteA replaced by an OpenUrl() no-op (no browser on device).
 *   - WIC/D3D11 icon upload replaced by stb_image + a GLES3 texture.
 *   - Palette: dark black surfaces, violet accents, bevelled XP frames kept.
 *   - Telegram: COMING SOON (see ShadowBrand::TelegramUrl).
 * =========================================================================== */""",
"""/* ===========================================================================
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
 * =========================================================================== */""",
     "file banner")

# -------------------------------------------------- dead XP chrome state ---
drop_line(r"^inline float  s_(tabY|bodyTop|menuBarY|toolbarY) *= *0\.f;\n", "dead layout statics")
drop_line(r"^inline float (BodyTop|MenuBarTop|ToolbarTop)\(\).*\n", "dead layout accessors")

# ------------------------------------------------- dead XP chrome members --
drop_func(r"inline bool TitleButton\(const ImVec2& a, const ImVec2& b, int glyph, bool enabled = true\)\n\{",
          "TitleButton")
drop_func(r"inline void UpdateResize\(const ImVec2& gripMin, const ImVec2& gripMax\)\n\{", "UpdateResize")
drop_func(r"inline bool TabButton\(const char\* label, int index\)\n\{", "TabButton")
drop_func(r"inline void DrawGrip\(const ImVec2& corner\)\n\{", "DrawGrip")
drop_func(r"inline void DrawScrollbar\(float availH, float contentHeight, float maxScroll\)\n\{",
          "XP DrawScrollbar")
drop_func(r"inline bool BeginMenuBar\(\)\n\{", "BeginMenuBar")
drop_line(r"^inline void EndMenuBar\(\) \{ \}\n", "EndMenuBar")
drop_func(r"inline bool ToolbarButton\(const char\* id, int glyph, const char\* tip, bool toggle, bool\* on\)\n\{",
          "ToolbarButton")
drop_line(r"^inline void BeginToolBar\(\) \{ \}\n", "BeginToolBar")
drop_line(r"^inline void EndToolBar\(\) \{ \}\n", "EndToolBar")
drop_func(r"inline void SeparatorToolBar\(\)\n\{", "SeparatorToolBar")
drop_span(r"inline const char\* kTabLabels\[\] =", r"^inline void Render\(\)\n\{",
          "kTabLabels + kTabCount + RunPage")

# --------------------------------------------------------- message box -----
swap("""    style::Raised(s_dl, s_msgMin, s_msgMax);

    const ImVec2 tMin(s_msgMin.x + 3.f, s_msgMin.y + 3.f);
    const ImVec2 tMax(s_msgMax.x - 3.f, tMin.y + style::TitleBarH);
    s_dl->AddRectFilled(tMin, tMax, AccentColor());
    s_dl->AddText(ImVec2(tMin.x + 6.f, tMin.y + (style::TitleBarH - ImGui::GetTextLineHeight()) * 0.5f),
                  style::TitleText, s_msgTitle);

    const float cbx = tMax.x - style::TitleBtnW - 1.f;
    const float cby = tMin.y + (style::TitleBarH - style::TitleBtnH) * 0.5f;
    TitleButton(ImVec2(cbx, cby), ImVec2(cbx + style::TitleBtnW, cby + style::TitleBtnH), 2);

    const ImVec2 iconC(s_msgMin.x + 28.f, s_msgMin.y + style::TitleBarH + 22.f);
    MsgBoxIcon(s_msgType, iconC);

    s_dl->AddText(ImVec2(s_msgMin.x + 52.f, s_msgMin.y + style::TitleBarH + 8.f), style::Text, s_msgText);""",
"""    // rounded glass card - no window frame, just a violet accent strip
    const float r = 16.f * style::S;
    style::GlowRound(s_dl, s_msgMin, s_msgMax, r, 0.8f);
    s_dl->AddRectFilledMultiColorRounded(s_msgMin, s_msgMax, style::BgDeep,
                                         style::Face, style::Face, style::BgDeep, style::BgDeep, r, 0);
    s_dl->AddRect(s_msgMin, s_msgMax, style::AccentDeep, r, 0, 2.f);

    s_dl->AddRectFilled(ImVec2(s_msgMin.x + 14.f, s_msgMin.y + 9.f),
                        ImVec2(s_msgMax.x - 14.f, s_msgMin.y + 10.f), style::Accent);
    s_dl->AddText(ImVec2(s_msgMin.x + 14.f, s_msgMin.y + 14.f), style::AccentGlow, s_msgTitle);

    const ImVec2 iconC(s_msgMin.x + 28.f, s_msgMin.y + 54.f);
    MsgBoxIcon(s_msgType, iconC);

    s_dl->AddText(ImVec2(s_msgMin.x + 52.f, s_msgMin.y + 46.f), style::Text, s_msgText);""",
     "message box -> rounded card")

swap("""        s_dl->AddRect(ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), AccentColor(), 1.f, 0, 1.f);""",
     """        s_dl->AddRect(ImVec2(a.x + 1.f, a.y + 1.f), ImVec2(b.x - 1.f, b.y - 1.f), AccentColor(),
                      (b.y - a.y) * 0.5f, 0, 1.f);""",
     "message box default-button ring")

# --------------------------------------------------------- combo + menu ----
swap("""    style::FillFace(s_dl, a, b);
    if (held) { style::Pressed(s_dl, a, b); }
    else      { style::Raised(s_dl, a, b); }

    s_dl->AddText(ImVec2(a.x + 5.f, a.y + (style::ComboH - ImGui::GetTextLineHeight()) * 0.5f),
                  style::Text, preview);
    DropDownArrows(a, b);""",
"""    // SHADOW MODZ combo: rounded field with a violet edge, no bevel.
    s_dl->AddRectFilled(a, b, hovered ? style::Field : style::BgDeep, style::ComboH * 0.5f);
    s_dl->AddRect(a, b, hovered ? style::Accent : style::AccentDeep, style::ComboH * 0.5f, 0, 1.5f);

    s_dl->AddText(ImVec2(a.x + 10.f, a.y + (style::ComboH - ImGui::GetTextLineHeight()) * 0.5f),
                  style::Text, preview);
    DropDownArrows(a, b);""",
     "combo field")

swap("""        s_dl = ImGui::GetWindowDrawList();
        style::Sunken(s_dl, ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + listW,
                                                          ImGui::GetWindowPos().y + maxListH + 4.f));""",
"""        s_dl = ImGui::GetWindowDrawList();
        const ImVec2 lp = ImGui::GetWindowPos();
        style::GlowRound(s_dl, lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), 10.f, 0.6f);
        s_dl->AddRectFilled(lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), style::Face, 10.f);
        s_dl->AddRect(lp, ImVec2(lp.x + listW, lp.y + maxListH + 4.f), style::AccentDeep, 10.f, 0, 1.5f);""",
     "combo drop list")

swap("""inline void EndDropDown()
{
    const ImVec2 a(s_menuOpenX, s_menuOpenY);
    const ImVec2 b(s_menuOpenX + (s_ctxOpen ? 110.f : 150.f), s_cursor.y);
    style::Sunken(s_dl, a, b);
    s_cursor.y = s_menuBarY + style::MenuBarH;
    s_cursor.x = s_winMin.x + style::FrameBorder + 4.f;
}""",
"""inline void EndDropDown()
{
    const ImVec2 a(s_menuOpenX, s_menuOpenY);
    const ImVec2 b(s_menuOpenX + (s_ctxOpen ? 110.f : 150.f), s_cursor.y);
    // Items are already drawn, so only the rounded frame goes on top.
    style::GlowRound(s_dl, a, b, 10.f, 0.55f);
    s_dl->AddRect(a, b, style::AccentDeep, 10.f, 0, 1.5f);
    s_cursor.y = s_winMin.y + style::HeaderH + 20.f;
    s_cursor.x = s_contentMin.x;
}""",
     "drop-down frame")

open(PATH, "w", encoding="utf-8").write(src)
print("\ncleaned", PATH)
