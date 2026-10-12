# SHADOW MODZ — rebrand of `NIKUMODS.WINDOWS.XP.GUI`

The release asset (`NIKUMODS.WINDOWS.XP.GUI.zip`, 41 MB) was unpacked and reskinned:
same Windows‑XP shell, new **dark black + violet** skin, new **OP buttons**, new
**floating icon**, and every visible string now reads **SHADOW MODZ**.

```
ShadowModz/            <- the Android/JNI project (source of the release zip)
  app/src/main/jni/
    NikuMods/ShadowBrand.h   <- ALL brand strings + colours live here
    NikuMods/Win95UI.h       <- the XP shell (palette, buttons, tabs, launcher)
    NikuMods/Win95Assets/    <- Micross font + SHADOW MODZ icon texture
    Main.cpp                 <- login window
  app/src/main/res/          <- app name, launcher icons, dark+violet theme
preview/                     <- browser preview of the new UI (see below)
```

## What changed

**Theme — dark black + violet**
- `W95::style` palette rewritten: near‑black surfaces (`#0A0710` → `#1A1526`),
  violet accents `#A855F7` / `#7C3AED` / `#D8B4FE`.
- Window body, group boxes, menu bar, toolbar, tabs, status bar and scrollbars
  all repainted; XP bevels (raised / sunken / etched) kept.
- ImGui colour map in `ApplyStyle()` follows the same palette, so popups,
  inputs and combo boxes match.

**OP buttons**
- `Button()` — vertical violet gradient face, XP bevel, gloss line, halo glow
  that intensifies on hover/press, drop‑shadowed label.
- New `AccentButton(label[, size][, enabled])` — full‑violet primary button,
  dims itself and refuses clicks when `enabled == false`.
- Tabs light up violet with an accent underline; toolbar buttons glow when
  toggled on; checkboxes get a violet tick plus a halo.

**Floating icon — rebuilt**
- Was a flat grey 96×26 rectangle labelled “Niku Mods”.
- Now a **draggable, pulsing violet disc**: breathing halo rings, lit top half,
  violet rim, SHADOW MODZ icon texture in the middle (falls back to an “SM”
  glyph). Tap opens the menu, drag moves it, and it clamps to the screen edges.

**Rebrand**
- Window title `Shadow Modz - Control Panel - WinXP`, status bar
  `SHADOW MODZ / UID: 1337`, About page `SHADOW MODZ v4.0 — Dark XP shell`.
- Login window retitled `SHADOW MODZ LOGIN` and repainted (dark glass, violet
  seam, violet primary button).
- App name `Shadow Modz`, dark+violet `colors.xml` / `styles.xml`, new adaptive
  launcher icon + legacy mipmaps generated from the SHADOW MODZ mark.
- Title‑bar / floating‑icon texture (`Win95Icon.hpp`) regenerated with the new
  mark; internal `NikuMods07()` renamed `ShadowMenu07()`.

**Telegram**
- Shows a dead violet button reading **“Telegram - COMING SOON”** (Config ▸
  About) and the login window prints `Telegram: COMING SOON`.
- To go live, edit **two lines** in `NikuMods/ShadowBrand.h`:

```cpp
constexpr const char* TelegramUrl = "https://t.me/your_channel";
constexpr bool        TelegramLive = true;
```

## Deliberately left alone

| Thing | Why |
| --- | --- |
| `LOCAL_MODULE := NikuMods` (Android.mk) | sets `libNikuMods.so`; renaming breaks the loader that injects it. Rename the module **and** the loader string together if you want it. |
| `NikuMods/` and `NikuModsStuff/` folders | referenced throughout `Android.mk` and every `#include`; folder rename is a separate, purely cosmetic commit. |
| `ShadowBrand::KeyUrl` (`nikumods.site/...`) | the endpoint the bundled auth client talks to. Point it at your own key server when you run your own licensing. |
| `NikuModsStuff/login.h` | duplicate login UI that nothing includes — not compiled, never shown. |

## Preview

`preview/index.html` renders the new shell in the browser (tabs, sliders and
buttons are clickable; the floating icon drags and toggles the window). Serve
the folder and open it, e.g. `python3 -m http.server 8080 --directory preview`.

## Build

Unchanged from upstream — Android Studio / AIDE with the NDK:

```
app/src/main/jni/Android.mk   (ndk-build, armeabi-v7a + arm64-v8a)
```

The vendored curl/openssl `.a` blobs and the multi‑MB font headers were not
imported into git (see below); copy them back from the release zip before
building, or keep using your original project tree with these files dropped in.

## APK

`apk/ShadowModz-v4.0.apk` (4.6 MB, debug-signed, `armeabi-v7a` + `arm64-v8a`)
was produced by `.github/workflows/build-apk.yml`. Because this sandbox has no
JDK / Android SDK / NDK (and `dl.google.com` is unreachable), the build runs on
a GitHub Actions runner: it downloads the release archive to restore the
vendored Dobby / curl / OpenSSL prebuilts, overlays the rebranded sources, then
runs `gradle assembleDebug`.

> The app shell (`src/main/java/com/akn/MainActivity.java`, `MenuService.java`)
> is a **placeholder** - the release archive contained no Java sources at all,
> even though the manifest declares both classes. It loads the native module and
> shows the branded screen. Replace those two files with your real activity and
> the same workflow builds the full app.

Rebuild any time: push to `arena/**`, or **Actions → Build APK → Run workflow**.
The run commits `apk/`, `native/` and `ci-out/build.log` back into the branch.

Two build fixes were needed for a modern NDK (kept in the tree):
- `ci/fix-include-case.py` repairs `#include` directives whose letter case does
  not match the files on disk (`dobby/dobby.h`, `Il2cpp.h`), a leftover from
  authoring on a case-insensitive filesystem.
- `jni/sync_shim.c` supplies `__sync_add_and_fetch_4`, still referenced by the
  32-bit OpenSSL prebuilt but no longer exported by current NDKs.
- `minSdkVersion` is 21 (matches `APP_PLATFORM := android-21` in
  `Application.mk`); AGP would otherwise force `android-19` and the 32-bit
  OpenSSL prebuilt fails to link.
