#pragma once
/* ============================================================================
 * SHADOW MODZ - brand constants
 * ----------------------------------------------------------------------------
 * Every string / colour that carries the SHADOW MODZ identity lives here, so a
 * rebrand is a one-file edit instead of a hunt through the shell.
 *
 * The shell itself (Win95UI.h) is a Windows-XP look-alike; the palette below
 * repaints it in dark black + violet while keeping the XP chrome: bevelled
 * frames, title bar, menu bar, toolbar, tabs, groups and status bar.
 * ==========================================================================*/

// Literal form of KeyUrl: OBFUSCATE() in Main.cpp needs a string literal, the
// rest of the code uses the constexpr below. Change both together.
#define SHADOW_KEY_URL_LITERAL "https://nikumods.site/GetFreeKey.php"

namespace ShadowBrand
{
    // ---- identity --------------------------------------------------------
    constexpr const char* Name         = "SHADOW MODZ";
    constexpr const char* Version      = "v4.0";
    constexpr const char* BuildTag     = "Dark XP shell";
    constexpr const char* WindowTitle  = "Shadow Modz - Control Panel - WinXP";
    constexpr const char* StatusUser   = "SHADOW MODZ";
    constexpr const char* StatusUid    = "UID: 1337";
    constexpr const char* LauncherText = "SM";    // glyph drawn on the floating icon

    // ---- Telegram --------------------------------------------------------
    // Not public yet: the shell renders a disabled "COMING SOON" entry until
    // Url is filled in and Live is flipped to true. Nothing else has to change.
    constexpr const char* TelegramLabel = "Telegram";
    constexpr const char* TelegramUrl   = "";     // e.g. "https://t.me/shadowmodz"
    constexpr const char* TelegramText  = "COMING SOON";
    constexpr bool        TelegramLive  = false;

    // ---- login / key server ---------------------------------------------
    // Left exactly as the upstream project ships: this is the endpoint the
    // bundled auth client talks to. Point it at your own key server when you
    // run your own licensing - nothing else in the UI needs touching.
    constexpr const char* KeyUrl = SHADOW_KEY_URL_LITERAL;

    // ---- palette: dark black + violet ------------------------------------
    struct RGB { unsigned char r, g, b; };

    constexpr RGB BgDeep       {  10,   7,  16 };  // #0A0710 deepest background
    constexpr RGB Face         {  26,  21,  38 };  // #1A1526 panel / window face
    constexpr RGB Light        {  43,  35,  64 };  // #2B2340 inner bevel
    constexpr RGB Highlight    {  60,  49,  89 };  // #3C3159 outer bevel
    constexpr RGB Shadow       {   7,   5,  12 };  // #07050C
    constexpr RGB DarkShadow   {   0,   0,   0 };  // #000000
    constexpr RGB Field        {  11,   8,  18 };  // #0B0812 input fields
    constexpr RGB Text         { 243, 239, 255 };  // #F3EFFF
    constexpr RGB TextDisabled { 110, 102, 133 };  // #6E6685

    constexpr RGB Accent       { 168,  85, 247 };  // #A855F7 primary violet
    constexpr RGB AccentDeep   {  76,  29, 149 };  // #4C1D95
    constexpr RGB AccentGlow   { 216, 180, 254 };  // #D8B4FE hover / glow
    constexpr RGB TitleLeft    {  91,  33, 182 };  // #5B21B6 title-bar gradient
    constexpr RGB TitleRight   { 168,  85, 247 };  // #A855F7
    constexpr RGB TitleText    { 255, 255, 255 };

    constexpr RGB Selection    { 124,  58, 237 };  // #7C3AED menus / selection
    constexpr RGB SelText      { 255, 255, 255 };
    constexpr RGB SliderFill   { 168,  85, 247 };

    constexpr RGB BtnTop       {  58,  42,  92 };  // #3A2A5C button gradient
    constexpr RGB BtnBot       {  21,  15,  36 };  // #150F24
    constexpr RGB BtnTopHover  { 109,  40, 217 };  // #6D28D9
    constexpr RGB BtnBotHover  {  42,  22,  80 };  // #2A1650

    constexpr RGB GroupBg      {  18,  14,  28 };  // #120E1C group box fill
    constexpr RGB StatusBg     {  13,  10,  21 };  // #0D0A15 status bar
}
