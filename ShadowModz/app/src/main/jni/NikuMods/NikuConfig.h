#pragma once
// ===========================================================================
// NikuMods - backing state for the cloned menu
// Folder: NikuMods  |  Credits: @ur_nikuu
// ---------------------------------------------------------------------------
// Every control of the cloned A-to-Z layout has a real variable here, so
// nothing is a dead toggle.  Defaults match the original CloneThisUi menu.
// Where the target already owns a working variable (Config.ESP.*, Enable,
// IgnoreKnocked, norecoil, HighFps, SpeedWalking, visual_esp_boxth ...) the
// UI binds straight to that instead, and those hooks act on it immediately.
// ===========================================================================

struct NikuCfg
{
    // ---- menu state ------------------------------------------------------
    int    Tab               = 1;        // 1..5 sidebar selection
    bool   OpenMenu          = true;     // main window visible
    int    Theme             = 5;        // 1..7 -> BACKGROUND1..7

    // ---- Players / ESP ---------------------------------------------------
    bool   EspShowFps        = false;
    bool   EspTeamId         = false;
    bool   EspDrawFov        = false;    // FOV circle helper on the overlay
    float  EspBoneThickness  = 2.0f;     // -> visual_esp_skel when applied
    float  EspGrenadeRange   = 45.0f;    // metres
    float  EspFovSize        = 120.0f;   // px, FOV circle radius

    // Colour panel (Players tab) - 3 floats each, like the source "menu" slot.
    float  BotVisible[3]     = { 1.00f, 0.40f, 0.17f };
    float  BotNonVisible[3]  = { 1.00f, 0.17f, 0.60f };
    float  PlayerVisible[3]  = { 0.10f, 0.90f, 0.55f };
    float  PlayerNonVisible[3]= { 0.95f, 0.95f, 0.20f };

    // ---- Ragebot ---------------------------------------------------------
    bool   RbIgnoreAimSilent   = false;
    bool   RbVisibleTargetOnly = false;
    bool   RbIgnoreAimShoot    = false;
    int    RbLootDistance      = 150;
    bool   RbVbadge            = false;
    bool   RbJoystick          = false;
    bool   RbWallHack          = false;
    bool   RbUnliAmmo          = false;
    bool   RbRecoilComp        = false;
    float  RbRecoil            = 1.0f;
    float  RbAimbotSmooth      = 2.0f;

    // ---- World -----------------------------------------------------------
    bool   WldVehicleName      = false;
    bool   WldVehicleHealth    = false;
    bool   WldVehicleFuel      = false;
    bool   WldAirDropBox       = false;
    bool   WldAirPlane         = false;
    bool   WldLootBox          = false;
    int    WldLootDistance     = 150;
    bool   WldRadarMap         = false;
    int    WldRadarX           = 100;
    int    WldRadarY           = 100;

    // ---- Memory ----------------------------------------------------------
    bool   MemNoShake          = false;
    bool   MemNoFog            = false;
    bool   MemFastScope        = false;
    bool   MemFastShoot        = false;
    bool   MemXhitEffect       = false;
    float  MemXhit             = 2.0f;
    float  MemIpadRange        = 2.0f;
};

// Single shared instance.
inline NikuCfg Niku;
