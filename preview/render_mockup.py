#!/usr/bin/env python3
"""Render a static mockup of the SHADOW MODZ floating panel as a PNG.

Uses the exact palette from ShadowModz/app/src/main/jni/NikuMods/ShadowBrand.h
and the widget geometry from Win95UI.h (panel metrics, pill switches, slim
sliders, rounded buttons), so the picture matches what the menu draws on
device. Everything is drawn with Pillow - no browser needed.

Run: python3 preview/render_mockup.py
"""
import os
from PIL import Image, ImageDraw, ImageFont

S = 2  # supersample factor; image is downscaled at the end

HERE = os.path.dirname(os.path.abspath(__file__))
FONT_DIR = "/usr/share/fonts/truetype/dejavu"
REG = os.path.join(FONT_DIR, "DejaVuSans.ttf")
BOLD = os.path.join(FONT_DIR, "DejaVuSans-Bold.ttf")

# ---- palette (ShadowBrand.h) ----------------------------------------------
BG_DEEP    = (10, 7, 16)
FACE       = (26, 21, 38)
LIGHT      = (43, 35, 64)
HIGHLIGHT  = (60, 49, 89)
SHADOW     = (7, 5, 12)
DARK       = (0, 0, 0)
FIELD      = (11, 8, 18)
TEXT       = (243, 239, 255)
TEXT_DIM   = (110, 102, 133)
ACCENT     = (168, 85, 247)
ACCENT_DEEP= (76, 29, 149)
ACCENT_GLOW= (216, 180, 254)
TITLE_L    = (91, 33, 182)
TITLE_R    = (168, 85, 247)
SELECTION  = (124, 58, 237)
BTN_TOP    = (58, 42, 92)
BTN_BOT    = (21, 15, 36)
BTN_TOP_H  = (109, 40, 217)
BTN_BOT_H  = (42, 22, 80)
GROUP      = (18, 14, 28)
STATUS     = (13, 10, 21)
GREEN      = (74, 222, 128)


def f(path, size):
    return ImageFont.truetype(path, int(size * S))


class Canvas:
    def __init__(self, w, h):
        self.w, self.h = w, h
        self.img = Image.new("RGB", (w * S, h * S), BG_DEEP)
        self.d = ImageDraw.Draw(self.img)
        self.glow = Image.new("RGBA", self.img.size, (0, 0, 0, 0))
        self.gd = ImageDraw.Draw(self.glow)

    # -- primitives ---------------------------------------------------------
    def rect(self, box, fill=None, outline=None, width=1):
        b = [v * S for v in box]
        self.d.rectangle(b, fill=fill, outline=outline, width=int(width * S))

    def rrect(self, box, r, fill=None, outline=None, width=1):
        b = [v * S for v in box]
        self.d.rounded_rectangle(b, radius=int(r * S), fill=fill,
                                 outline=outline, width=int(width * S))

    def ellipse(self, box, fill=None, outline=None, width=1):
        b = [v * S for v in box]
        self.d.ellipse(b, fill=fill, outline=outline, width=int(width * S))

    def line(self, xy, fill, width=1):
        p = [(v[0] * S, v[1] * S) for v in xy]
        self.d.line(p, fill=fill, width=int(width * S))

    def grad_v(self, box, top, bot):
        x0, y0, x1, y1 = [v * S for v in box]
        if y1 == y0:
            return
        for y in range(y0, y1):
            t = (y - y0) / float(y1 - y0)
            col = tuple(int(top[i] + (bot[i] - top[i]) * t) for i in range(3))
            self.d.line([(x0, y), (x1, y)], fill=col)

    def grad_h(self, box, left, right):
        x0, y0, x1, y1 = [v * S for v in box]
        if x1 == x0:
            return
        for x in range(x0, x1):
            t = (x - x0) / float(x1 - x0)
            col = tuple(int(left[i] + (right[i] - left[i]) * t) for i in range(3))
            self.d.line([(x, y0), (x, y1)], fill=col)

    def bevel(self, box, raised=True):
        x0, y0, x1, y1 = box
        tl, br = (HIGHLIGHT, SHADOW) if raised else (SHADOW, HIGHLIGHT)
        self.rect((x0, y0, x1, y0 + 1), fill=tl)
        self.rect((x0, y0, x0 + 1, y1), fill=tl)
        self.rect((x0, y1 - 1, x1, y1), fill=br)
        self.rect((x1 - 1, y0, x1, y1), fill=br)

    def halo(self, box, color=ACCENT, rings=4, step=3, alpha=16, width=2):
        x0, y0, x1, y1 = box
        for i in range(rings, 0, -1):
            o = i * step
            self.gd.rectangle([(x0 - o) * S, (y0 - o) * S, (x1 + o) * S, (y1 + o) * S],
                              outline=color + (alpha,), width=int(width * S))

    def halo_r(self, box, r, color=ACCENT, rings=5, step=2.5, alpha=14, width=1.5):
        x0, y0, x1, y1 = box
        for i in range(rings, 0, -1):
            o = i * step
            self.gd.rounded_rectangle([(x0 - o) * S, (y0 - o) * S,
                                       (x1 + o) * S, (y1 + o) * S],
                                      radius=int((r + o) * S),
                                      outline=color + (alpha,), width=int(width * S))

    def text(self, xy, s, size=13, color=TEXT, bold=False, shadow=True):
        font = f(BOLD if bold else REG, size)
        x, y = xy
        if shadow:
            self.d.text((x * S, (y + 1) * S), s, font=font, fill=(0, 0, 0))
        self.d.text((x * S, y * S), s, font=font, fill=color)

    def tw(self, s, size=13, bold=False):
        return self.d.textlength(s, font=f(BOLD if bold else REG, size)) / S

    def paste(self, im, cx, cy, size):
        im = im.resize((int(size * S), int(size * S)), Image.LANCZOS)
        x = int(cx * S - im.width / 2)
        y = int(cy * S - im.height / 2)
        self.img.paste(im, (x, y), im if im.mode == "RGBA" else None)

    def finish(self, path):
        out = Image.alpha_composite(self.img.convert("RGBA"), self.glow)
        out = out.convert("RGB").resize((self.w, self.h), Image.LANCZOS)
        out.save(path, optimize=True)
        return out


# ---- widgets (mirrors Win95UI.h) ------------------------------------------
def section(c, x, y, w, label):
    """BeginGroup(): accent bar + uppercase title + hairline divider."""
    h = 26
    c.rect((x, y + h * 0.25, x + 3, y + h * 0.75), fill=ACCENT)
    c.text((x + 12, y + 5), label.upper(), size=11, color=ACCENT_GLOW, bold=True)
    c.rect((x + 12, y + h - 3, x + w, y + h - 2), fill=ACCENT + (45,))


def switch(c, x, y, label, on):
    """Checkbox(): pill switch with a sliding knob."""
    w, h = 42, 22
    r = h / 2
    box = (x, y, x + w, y + h)
    if on:
        c.halo_r(box, r, rings=4, step=2.5, alpha=15, width=1.5)
    c.rrect(box, r, fill=ACCENT_DEEP if on else FIELD)
    c.rrect(box, r, outline=ACCENT if on else LIGHT, width=1)
    kx = box[2] - r if on else box[0] + r
    c.ellipse((kx - r + 3, y + 3, kx + r - 3, y + h - 3),
              fill=ACCENT_GLOW if on else TEXT_DIM)
    c.text((x + w + 8, y + 2), label, size=12, color=TEXT if on else TEXT_DIM)


def slim_slider(c, x, y, w, frac, label, value):
    """Slider(): label + value, then a slim filled track with a round knob."""
    c.text((x, y), label, size=12, color=TEXT)
    c.text((x + w - c.tw(value, size=12), y), value, size=12, color=ACCENT_GLOW)
    cy = y + 21
    track = (x, cy - 3, x + w, cy + 3)
    c.rrect(track, 3, fill=FIELD)
    c.rrect(track, 3, outline=LIGHT, width=1)
    kx = x + w * frac
    c.rrect((x, cy - 3, kx, cy + 3), 3, fill=ACCENT)
    c.halo_r((x, cy - 8, x + w, cy + 8), 8, rings=3, step=2, alpha=13, width=1.5)
    c.ellipse((kx - 7, cy - 7, kx + 7, cy + 7), fill=ACCENT_GLOW)
    c.ellipse((kx - 7, cy - 7, kx + 7, cy + 7), outline=ACCENT_DEEP, width=1.5)


def pill_button(c, box, label, disabled=False, primary=False):
    x0, y0, x1, y1 = box
    r = (y1 - y0) / 2
    if not disabled:
        c.halo_r(box, r, rings=4, step=2.5, alpha=18, width=1.5)
    if disabled:
        c.rrect(box, r, fill=BTN_BOT)
        c.rrect(box, r, outline=LIGHT, width=1)
    elif primary:
        c.grad_v(box, ACCENT_GLOW if False else ACCENT, ACCENT_DEEP)
        c.rrect(box, r, outline=ACCENT_DEEP, width=1.5)
    else:
        c.grad_v(box, BTN_TOP, BTN_BOT)
        c.rrect(box, r, outline=ACCENT_DEEP, width=1.5)
    c.rect((x0 + 6, y0 + 2, x1 - 6, y0 + 3), fill=(255, 255, 255, 46) if not disabled else (255, 255, 255, 14))
    tw = c.tw(label, size=12, bold=True)
    tx = x0 + (x1 - x0 - tw) / 2
    ty = y0 + (y1 - y0) / 2 - 8
    c.text((tx, ty), label, size=12, bold=True,
           color=TEXT_DIM if disabled else ((255, 255, 255) if primary else TEXT))


def floating_icon(c, cx, cy, r, icon):
    """DrawLauncher(): pulsing halo rings around the branded disc."""
    for i in range(4, 0, -1):
        rr = r + i * 5
        c.ellipse((cx - rr, cy - rr, cx + rr, cy + rr), outline=ACCENT + (10 + i * 12,), width=2)
    c.ellipse((cx - r, cy - r, cx + r, cy + r), fill=BG_DEEP)
    c.d.pieslice([(cx - r) * S, (cy - r) * S, (cx + r) * S, (cy + r) * S], 180, 360, fill=BTN_TOP)
    c.ellipse((cx - r + 3, cy - r + 3, cx + r - 3, cy + r - 3), fill=BTN_BOT)
    c.ellipse((cx - r, cy - r, cx + r, cy + r), outline=ACCENT, width=3)
    c.ellipse((cx - r + 5, cy - r + 5, cx + r - 5, cy + r - 5), outline=ACCENT + (70,), width=1)
    c.paste(icon, cx, cy, (r - 11) * 2)


def panel(c, box, icon):
    """BeginPanel()/EndPanel(): the whole floating glass card."""
    x0, y0, x1, y1 = box
    r = 18
    c.halo_r(box, r, rings=5, step=2.5, alpha=13, width=1.5)
    c.rrect(box, r, fill=FACE)
    # top-down darkening so the card reads as glass
    c.grad_v(box, (0, 0, 0), (0, 0, 0)) if False else None
    c.rrect(box, r, outline=ACCENT_DEEP, width=2)
    c.rrect((x0 + 1, y0 + 1, x1 - 1, y1 - 1), r - 1, outline=ACCENT_GLOW + (34,))

    # ---- header ----------------------------------------------------------
    hx0, hy0, hx1 = x0 + 10, y0 + 10, x1 - 10
    hh = 55
    hbox = (hx0, hy0, hx1, hy0 + hh)
    c.halo_r(hbox, 12, rings=4, step=2, alpha=10, width=1.5)
    c.grad_h(hbox, ACCENT_DEEP, ACCENT)
    c.rrect(hbox, 12, outline=ACCENT + (90,), width=1)

    lc = (hx0 + 8 + 25, hy0 + hh / 2)
    c.ellipse((lc[0] - 22, lc[1] - 22, lc[0] + 22, lc[1] + 22), fill=BG_DEEP)
    c.ellipse((lc[0] - 22, lc[1] - 22, lc[0] + 22, lc[1] + 22), outline=ACCENT_GLOW, width=2)
    c.paste(icon, lc[0], lc[1], 34)

    title = "SHADOW MODZ"
    c.text((lc[0] + 34, hy0 + 18), title, size=17, bold=True, color=(255, 255, 255))
    tw = c.tw(title, size=17, bold=True)
    chip_x = lc[0] + 34 + tw + 8
    c.rrect((chip_x, hy0 + 19, chip_x + 36, hy0 + 37), 9, fill=(255, 255, 255, 40))
    c.text((chip_x + 9, hy0 + 21), "v4.0", size=11, bold=True, color=(255, 255, 255))

    bc = (hx1 - 8 - 13, hy0 + hh / 2)
    c.ellipse((bc[0] - 13, bc[1] - 13, bc[0] + 13, bc[1] + 13), fill=(38, 27, 60))
    c.ellipse((bc[0] - 13, bc[1] - 13, bc[0] + 13, bc[1] + 13), outline=LIGHT, width=1.5)
    c.line([(bc[0] - 4, bc[1] - 4), (bc[0] + 4, bc[1] + 4)], TEXT, 2)
    c.line([(bc[0] + 4, bc[1] - 4), (bc[0] - 4, bc[1] + 4)], TEXT, 2)

    return (x0 + 18, hy0 + hh + 8, x1 - 18, y1 - 30)   # body rect


def footer(c, box):
    x0, y0, x1, y1 = box
    c.rect((x0 + 14, y0 - 4, x1 - 14, y0 - 3), fill=ACCENT + (70,))
    dy = (y0 + y1) / 2
    c.ellipse((x0 + 20, dy - 4, x0 + 28, dy + 4), fill=GREEN)
    c.ellipse((x0 + 20, dy - 4, x0 + 28, dy + 4), outline=GREEN + (90,), width=3)
    c.text((x0 + 34, dy - 8), "SHADOW MODZ", size=11, bold=True, color=ACCENT_GLOW)
    uid = "UID: 1337"
    c.text((x1 - 14 - c.tw(uid, size=11), dy - 8), uid, size=11, color=TEXT_DIM)


def scrollbar(c, body, frac=0.0, thumb_frac=0.42):
    x0, y0, x1, y1 = body
    c.rrect((x1 - 3, y0, x1, y1), 1.5, fill=(255, 255, 255, 16))
    th = (y1 - y0) * thumb_frac
    ty = y0 + ((y1 - y0) - th) * frac
    c.rrect((x1 - 3, ty, x1, ty + th), 1.5, fill=ACCENT)


def fade_bottom(c, body, h=48):
    """Fade the last rows so the card reads as scrollable."""
    x0, y0, x1, y1 = body
    y0 = y1 - h
    for y in range(int(y0), int(y1)):
        t = (y - y0) / float(h)
        a = t * t
        for x in range(int(x0), int(x1)):
            cur = c.img.getpixel((x * S, y * S))
            col = tuple(int(cur[i] * (1 - a) + BG_DEEP[i] * a) for i in range(3))
            c.d.point((x * S, y * S), fill=col)
            for sx in range(S):
                for sy in range(S):
                    c.d.point((x * S + sx, y * S + sy), fill=col)


# ---- composition -----------------------------------------------------------
def main():
    icon = Image.open(os.path.join(HERE, "icon.png")).convert("RGBA")

    W, H = 1280, 780
    c = Canvas(W, H)

    # backdrop
    c.grad_v((0, 0, W, H), (22, 17, 38), (4, 3, 8))

    title = "SHADOW MODZ"
    c.text(((W - c.tw(title, size=30, bold=True)) / 2, 20), title, size=30,
           bold=True, color=ACCENT_GLOW)
    sub = "Floating glass panel  ·  v4.0  ·  Telegram: COMING SOON"
    c.text(((W - c.tw(sub, size=13)) / 2, 60), sub, size=13, color=TEXT_DIM)

    # ============================ floating panel ===========================
    PB = (56, 92, 456, 712)
    body = panel(c, PB, icon)
    bx0, by0, bx1, by1 = body
    cx = bx0 + 8
    cw = bx1 - bx0 - 16

    y = by0 + 6
    section(c, cx, y, cw, "Aimbot");            y += 26
    switch(c, cx, y, "Aimbot", True);           y += 24
    switch(c, cx, y, "Silent Aim", True);       y += 24
    switch(c, cx, y, "Headshot", False);        y += 24
    slim_slider(c, cx, y, cw, 0.62, "Size", "310"); y += 38

    section(c, cx, y, cw, "Visuals");           y += 26
    switch(c, cx, y, "Enabled", True);          y += 24
    switch(c, cx, y, "Box", True);              y += 24
    switch(c, cx, y, "Line", False);            y += 24
    switch(c, cx, y, "Name", True);             y += 24

    section(c, cx, y, cw, "World");             y += 26
    slim_slider(c, cx, y, cw, 0.45, "Render Distance", "475"); y += 38

    section(c, cx, y, cw, "Actions");           y += 26
    pill_button(c, (cx, y, cx + (cw - 10) / 2, y + 28), "Save Preset", primary=True)
    pill_button(c, (cx + (cw - 10) / 2 + 10, y, cx + cw, y + 28), "Load Preset")
    y += 36

    section(c, cx, y, cw, "About");             y += 26
    c.text((cx + 2, y), "SHADOW MODZ v4.0", size=12, bold=True, color=TEXT)
    c.text((cx + 2, y + 18), "Floating panel shell", size=11, color=TEXT_DIM)
    pill_button(c, (cx, y + 38, cx + cw, y + 66), "Telegram - COMING SOON", disabled=True)

    scrollbar(c, body, 0.16, 0.40)
    footer(c, (PB[0], PB[3] - 36, PB[2], PB[3] - 10))

    # ============================ floating icon ============================
    FX, FY, FR = 580, 178, 42
    floating_icon(c, FX, FY, FR, icon)
    c.text((FX + 62, FY - 24), "FLOATING ICON", size=11, bold=True, color=ACCENT_GLOW)
    c.text((FX + 62, FY - 6), "draggable · pulsing halo", size=12, color=TEXT_DIM)
    c.text((FX + 62, FY + 14), "tap opens the panel, tap ✕ to", size=12, color=TEXT_DIM)
    c.text((FX + 62, FY + 32), "collapse back to the icon", size=12, color=TEXT_DIM)

    # ============================ what changed =============================
    NX, NY, NW, NH = 520, 268, 330, 152
    c.halo_r((NX, NY, NX + NW, NY + NH), 16, rings=4, step=2.5, alpha=11, width=1.5)
    c.rrect((NX, NY, NX + NW, NY + NH), 16, fill=FACE)
    c.rrect((NX, NY, NX + NW, NY + NH), 16, outline=ACCENT_DEEP, width=2)
    c.text((NX + 16, NY + 12), "NO WINDOW CHROME", size=11, bold=True, color=ACCENT_GLOW)
    for i, t in enumerate([
        "✕  no title bar, no min / max / close row",
        "✕  no menu bar, toolbar or tab strip",
        "✕  no status bar and no resize grip",
        "✓  rounded glass card, drag by header",
        "✓  every feature in one scroll column",
    ]):
        c.text((NX + 16, NY + 38 + i * 21), t, size=11,
               color=GREEN if t.startswith("✓") else TEXT_DIM, shadow=False)

    # ============================ palette card =============================
    PX, PY, PW, PH = 870, 268, 290, 152
    c.rrect((PX, PY, PX + PW, PY + PH), 16, fill=FACE)
    c.rrect((PX, PY, PX + PW, PY + PH), 16, outline=ACCENT_DEEP, width=2)
    c.text((PX + 16, PY + 12), "PALETTE", size=11, bold=True, color=ACCENT_GLOW)
    swatches = [
        ("#0A0710  bg deep", BG_DEEP), ("#1A1526  face", FACE),
        ("#7C3AED  selection", SELECTION), ("#A855F7  accent", ACCENT),
        ("#D8B4FE  glow", ACCENT_GLOW), ("#F3EFFF  text", TEXT),
    ]
    for i, (name, col) in enumerate(swatches):
        sx = PX + 16 + (i % 2) * 138
        sy = PY + 38 + (i // 2) * 40
        c.rrect((sx, sy, sx + 34, sy + 24), 6, fill=col)
        c.rrect((sx, sy, sx + 34, sy + 24), 6, outline=HIGHLIGHT, width=1)
        c.text((sx + 44, sy + 6), name, size=10, color=TEXT_DIM, shadow=False)

    # ============================ switch / slider detail ====================
    DX, DY, DW, DH = 520, 446, 640, 152
    c.rrect((DX, DY, DX + DW, DY + DH), 16, fill=FACE)
    c.rrect((DX, DY, DX + DW, DY + DH), 16, outline=ACCENT_DEEP, width=2)
    c.text((DX + 16, DY + 12), "WIDGETS", size=11, bold=True, color=ACCENT_GLOW)
    switch(c, DX + 20, DY + 40, "off", False)
    switch(c, DX + 20, DY + 74, "on", True)
    slim_slider(c, DX + 240, DY + 38, 200, 0.25, "Size", "120")
    slim_slider(c, DX + 240, DY + 82, 200, 0.75, "Smoothness", "0.75")
    pill_button(c, (DX + 470, DY + 38, DX + 610, DY + 66), "Save Preset", primary=True)
    pill_button(c, (DX + 470, DY + 78, DX + 610, DY + 106), "Telegram", disabled=True)

    c.text((300, 748), "Rendered from ShadowBrand.h · Win95UI.h — same constants the APK draws with",
           size=12, color=TEXT_DIM)

    out = c.finish(os.path.join(HERE, "ui-mockup.png"))
    print("wrote", os.path.join(HERE, "ui-mockup.png"), out.size)


if __name__ == "__main__":
    main()
