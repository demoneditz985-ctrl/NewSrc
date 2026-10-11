


bool EnableAll;
bool AimFire;
bool AimScope;
bool AimAuto;
bool SpeedHack;
bool AimbotLegit;
bool EspLine;
bool EspBox;
bool EspFill;
bool EspHealth;
bool EspTeammate;
bool EspFov;
bool EspSkeleton;
bool ZaruSilent;
bool AutoFireKill;

bool EnemyPull;
bool EspFire;
bool EspFireBlue;
bool EspGrenade;
bool Headshot;
bool CountDih;
bool TeleportProCs;
bool TeleportProBr;
bool FlyMap;

bool main_enableall = false;

bool rage_auto_aim = false;
bool rage_silent_aim = false;
bool rage_no_recoil = false;
bool rage_no_spread = false;
bool rage_triggerbot = false;
bool rage_auto_fire = false;
bool rage_fake_lag = false;
bool rage_jitter = false;
bool rage_spinbot = false;
bool rage_desync = false;
bool rage_pitch = false;
bool rage_yaw = false;
bool rage_testing = false;
bool rage_legit_bot = false;
bool rage_crosshair_pull = false;
bool rage_visible_chk = false;

bool visuals_player_esp = false;
bool visuals_box_esp = false;
bool visuals_skeleton_esp = false;
bool visuals_health_esp = false;
bool visuals_name_esp = false;
bool visual_distance_esp = false;
bool visuals_radar = false;
bool visuals_crosshair = false;
bool visuals_hit_marker = false;
bool visuals_no_flash = false;
bool visuals_night_mode = false;
bool visuals_full_bright = false;
bool visuals_testing = false;
bool visual_espfire = false;
bool visual_espbluefire = false;
bool visual_show_fov = false;

bool misc_Highfps = false;
bool misc_reset_acc = false;
bool misc_bunny_hop = false;
bool misc_speed_hack = false;
bool misc_fly_hack = false;
bool misc_no_clip = false;
bool misc_teleport = false;
bool misc_god_mode = false;
bool misc_infinite_ammo = false;
bool misc_rapid_fire = false;
bool misc_no_reload = false;
bool misc_gravity_hack = false;
bool misc_jump_hack = false;
bool misc_stamina_hack = false;
bool misc_testing = false;
bool misc_fast_reload = false;

Vector3 SetMap = Vector3(0, 0, 0);
int HighVp     = 0;


float autoFireTimer = 0.0f;
bool autoIsFiring = false;
float rapidFireTimer = 0.0f;
bool rapidIsFiring = false;

std::map<void*, Vector3> magnetSavedPos;
void* magnetLastTarget = nullptr;
float lockedDistance = 0.0f;


void* originalSkybox = nullptr;
bool skyboxSaved = false;

float visual_circle_size,visual_circle_stroke,visual_lined_size,visual_lined_stroke,visual_esp_line = 1,visual_esp_box = 1,visual_esp_boxth = 1,visual_esp_box_filedth = 1,visual_esp_box_filed = 1,visual_esp_skel = 1;


bool AutoFire = false;
float AutoFireCD = 0.05f;
float fire = 1.2f;
bool isFiring = false;



float EspNameSize = 0.65f;
float LineSize = 0.1f;
float Fov_Aim = 360;
float active = 0.0f;
bool saved = false;
float desactive = 0.0f;
bool ignoreEnemyBot    = false;
bool ignoreKnocked     = true;
static int aimPosition = 0;
const float NECK_OFFSET = -0.15f;
bool checkVisible2 = true;
bool AimVisible = false;
bool aimVisible = false;
bool AimSilentZaru;
bool Distance;
bool AimbotRage;


ImColor TeamColor  = ImColor(255, 255, 255, 255);
ImColor EnemyColor = ImColor(255, 255, 255, 255);
ImColor KnockColor = ImColor(255, 0, 0, 255);
ImColor NameColor  = ImColor(255, 255, 255, 255);
ImColor DistColor  = ImColor(255, 255, 255, 255);
ImColor FovColor   = ImColor(255, 0, 0, 255);



bool TestSafeSilent = true;
static std::map<void*, std::chrono::steady_clock::time_point> lastShotTimePerTarget;
static std::mutex silentLock;
static void* g_HitObjInfo = nullptr;
static Vector3 g_TargetDir = {0, 0, 0};
static bool g_HasData = false;


