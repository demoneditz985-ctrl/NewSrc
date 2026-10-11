
#include <set>
#include <algorithm>
#include <deque>
#include <string>
#include <android/log.h>
#include <string>
#include <vector>
#include <set>
#include <cstdio>
#include <dlfcn.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
// #include "obfuscate.h"
#include "class.h"
#include "Rect.h"

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
bool enableAntiReport = true;
bool EnemyPull;
bool EspFire;
bool EspFireBlue;
bool EspGrenade;
bool Headshot;
bool CountDih;
bool TeleportProCs;
bool TeleportProBr;
bool FlyMap;

std::mutex logMutex;
std::deque<std::string> cmdLogs;

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
bool TestSafeSilent = true;
static std::map<void*, std::chrono::steady_clock::time_point> lastShotTimePerTarget;
static std::mutex silentLock;
static void* g_HitObjInfo = nullptr;
static Vector3 g_TargetDir = {0, 0, 0};
static bool g_HasData = false;


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

#define u3str (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"), OBFUSCATE("String"), OBFUSCATE("CreateString"), 1)
static monoString *U3DStr(const char *str) {
    monoString *(*String_CreateString)(void *_this, const char *str) = (monoString * (*)(void *, const char *))(u3str);
    return String_CreateString(NULL, str);
}

#define Class_Entity__SetRotation Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("Entity"), OBFUSCATE("set_Rotation"), 1)

Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc){
    return Quaternion::LookRotation((targetLocation + Vector3(0, y_bias, 0)) - myLoc, Vector3(0, 1, 0));
}




template <typename T>
struct UnityArray {
    void* klass;
    void* monitor;
    void* bounds;
    int max_length;
    T vector[64]; 
};

struct DictionaryEntry {
    int hashCode;
    int next;
    void* key;
    void* value;
};

struct MyDictionary {
    void* klass;
    void* monitor;
    void* buckets;
    UnityArray<DictionaryEntry>* entries;
    int count;
};



struct COW_GamePlay_IHAAMHPPLMG_o;
struct COW_GamePlay_IHAAMHPPLMG_o {
    uint32_t m_Value;
    uint32_t m_ID;
    uint8_t m_TeamID;
    uint8_t m_ShortID;
    uint64_t m_IDMask;
};

struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t BaseDamage;
    int32_t HitColliderType;
    monoString* HitColliderName;
    bool isBackArea;
    COW_GamePlay_IHAAMHPPLMG_o Damager;
    void* Weapon;
    int32_t WeaponDataID;
    Vector3 FirePos;
    Vector3 HitPos;
    Vector3 HitNormal;
    uint8_t SpecialHitType;
    bool ForceNoHeadshot;
    int32_t ExtraInfo;
    MyDictionary *SpecialHitDic;
};

struct PlayerID_MKFEKBKJCKE_o;
struct PlayerID {
    uint32_t NBPDJAAAFBH;
    uint32_t JEDDPHIHGKL;
    uint8_t IOICFFEKAIL;
    uint8_t PHAFNFOFFDB;
    uint64_t BNFAIDHEHOM;
};

struct HitObjectInfo {
    void *klass;
    void *monitor;
    bool m_IsInPool;
    void *HitObject;
    void *HitCollider;
    Vector3 HitLocation;
    Vector3 HitNormal;
    Vector3 RayDir;
    Vector3 StartPosition;
    int32_t Damage;
    float Distance;
    int32_t ActorLayer;
    int32_t HitGroup;
    void *HitPhysicMaterial;
    bool IgnoreHappens;
    bool ViewBlocked;
    struct Vector3 OrigStartPosition;
    uint8_t SpecialHitType;
    uint32_t SpecialHitLevelObjID;
};




static bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon) {
    if (player != nullptr && enemy != nullptr && weapon != nullptr) {
        Vector3 EnemyHeadPosition = GetHeadPosition(enemy); 
        Vector3 PlayerHeadPosition = GetHeadPosition(player);       
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float weaponRange = get_Range(weapon);
        if (distance <= weaponRange) {
            return true;
        }
    }
    return false;
}

bool Visible_Check(void *enemy)  {
    if(enemy != nullptr)  {
        void *hitObj = nullptr;
        auto Camera = Transform_GetPosition(Component_GetTransform(Camera_main()));
        auto Target = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(enemy)));
        return !Physics_Raycast(Camera, Target, 12, &hitObj);
    }
    return false;
}

Vector3 GetAdjustedPosition(void* closestEnemy) {
    Vector3 headPos = GetHeadPosition(closestEnemy);
    if (aimPosition == 1) {
        headPos.y += NECK_OFFSET;
    } else if (aimPosition == 2) {
        Vector3 hipPos = GetHipPosition(closestEnemy);
        headPos = Vector3::Lerp(headPos, hipPos, 0.5f);
    } else if (aimPosition == 3) {
        headPos = GetHipPosition(closestEnemy);
    }
    return headPos;
}


bool isVisible(void * player){
    if(player != NULL) {
        void *hitObj = NULL;
        

        
        Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(Camera_main()));
        Vector3 headLocation = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(player)));
            return !Physics_Raycast(cameraLocation, headLocation, 12, &hitObj);
    }
    return false;
}

void* EnemySlientAim() {
    float shortestDistance = 9999.0f;
    void* closestEnemy = nullptr;

    void* match = Curent_Match();
    if (!match) return nullptr;

    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer) return nullptr;

    auto players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
    if (!players || !players->entries) return nullptr;

    auto entries = players->entries;
    Vector3 localPos = getPosition(localPlayer);

    for (int i = 0; i < players->count; i++) {
        DictionaryEntry& entry = entries->vector[i];
        if (entry.hashCode < 0) continue;

        void* player = entry.value;
        if (!player) continue;
        if (ignoreEnemyBot && get_IsClientBot(player)) continue;
        if (get_isLocalTeam(player)) continue;
        if (ignoreKnocked && get_IsDieing(player)) continue;
        if (GetHp(player) <= 0) continue;
        if (get_MaxHP(player) <= 0) continue;
        if (!get_isVisible(player)) continue;
        if (checkVisible2 && !Visible_Check(player)) continue;

        Vector3 enemyPos = GetAdjustedPosition(player);
        float dist = Vector3::Distance(localPos, enemyPos);

        if (dist < shortestDistance) {
            shortestDistance = dist;
            closestEnemy = player;
        }
    }

    return closestEnemy;
}

void* EnemyVisible(void* match) {
    if (!match || !main_enableall || !main_enableall) {
        return nullptr;
    }
    
    float shortestDistance = 99999.0f;
    float maxAngle = Fov_Aim;
    void* closestEnemy = nullptr;
    void* LocalPlayer = GetLocalPlayer(match);
    if (!LocalPlayer) return nullptr;

    auto players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
    if (!players || !players->entries) return nullptr;

    auto entries = players->entries;

    for (int u = 0; u < players->count; u++) {
        DictionaryEntry& entry = entries->vector[u];
        if (entry.hashCode < 0) continue;

        void* Player = entry.value;
        if (!Player) continue;
        

        if (!get_isLocalTeam(Player)
            && (!ignoreKnocked || !get_IsDieing(Player))
            && get_isVisible(Player)
            && get_MaxHP(Player) > 0) {

            Vector3 PlayerPos = GetHeadPosition(Player);
            Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
            Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);

            float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * 100.0f;
            if (!get_God(Player)) {
                if (rage_visible_chk) {
                    if (isVisible(Player) &&
                        angle <= maxAngle &&
                        angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                } else {
                    if (angle <= maxAngle &&
                        angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                }
            }
        }
    }
    return closestEnemy;
}



void* hackerrrr() {
    float shortestDistance = 9999;
    void* closestEnemy = nullptr;   
    void* get_MatchGame = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    
    if (LocalPlayer != nullptr && get_MatchGame != nullptr && main_enableall) {
        auto players = *(MyDictionary**)((uintptr_t)get_MatchGame + ListPlayer);
        if (!players || !players->entries) return nullptr;
        
        for (int i = 0; i < players->count; i++) {
            DictionaryEntry& entry = players->entries->vector[i];
            if (entry.hashCode < 0) continue;
            
            void* Player = entry.value;
            if (!Player || Player == LocalPlayer) continue;
            if (get_isLocalTeam(Player) || get_IsDieing(Player) || !get_MaxHP(Player)) continue;
            
            Vector3 PlayerPos = getPosition(Player);
            Vector3 LocalPlayerPos = getPosition(LocalPlayer);
            Vector3 pos2 = WorldToScreenPoint(Camera_main(), PlayerPos);
            

            float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
            
            if (distance < 1000) {
                Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * 100.0;
                
                if (angle <= Fov_Aim && angle < shortestDistance) {
                    shortestDistance = angle;
                    closestEnemy = Player;
                }
            }
        }
    }
    return closestEnemy;
}

static bool IsValid(void* obj) {
    return obj != nullptr; 
}




void BlackSkyUpdate() {
    if (visuals_night_mode) {
        if (!skyboxSaved) {
            originalSkybox = get_skybox_Original();
            skyboxSaved = true;
        }
        set_skybox_Null(nullptr);
        set_ambientMode_Black(3);
    } else {
        if (skyboxSaved && originalSkybox) {
            set_skybox_Null(originalSkybox);
            set_ambientMode_Black(0);
            skyboxSaved = false;
            originalSkybox = nullptr;
        }
    }
}



bool SpinBot = false;
float SpinSpeed = 360.0f;
std::mutex spinLock;
static void* g_SpinBotEntity = nullptr;
static Quaternion g_SpinBotData = {0, 0, 0, 1};
static bool g_SpinBotReady = false;
static float g_SpinBotAngle = 0.0f;
static void* g_LastMatch = nullptr;

static void Entity_SetRotation(void* entity, Quaternion rotation) {
    void (*_SetRotation)(void* entity, Quaternion rotation) = (void(*)(void*, Quaternion))(Class_Entity__SetRotation);    
    _SetRotation(entity, rotation);
}











void (*LateUpdate)(void *Player);
void _LateUpdate(void *Player) {
    
    if (!Player) {
        LateUpdate(Player); return;
    }

    void* CurrentMatch = Curent_Match();
    if (!CurrentMatch) {
        LateUpdate(Player); return;
    }
    
    BlackSkyUpdate();
    
    
    
    void* LocalPlayer = GetLocalPlayer(CurrentMatch);
    if (!LocalPlayer) {
        LateUpdate(Player); return;
    }
    
    void* trLocal = Component_GetTransform(LocalPlayer);
    if (!trLocal) {
        LateUpdate(Player); return;
    }
    

    
    void* closestEnemy = EnemyVisible(CurrentMatch);
    void* dikhtahuaenemy = hackerrrr();
    
if (main_enableall) {
    if (closestEnemy != NULL && LocalPlayer != NULL && CurrentMatch != NULL) {
        Vector3 EnemyLocation = GetAdjustedPosition(closestEnemy);
        Vector3 PlayerLocation = CameraMain(LocalPlayer);
        Quaternion PlayerLook = GetRotationToLocation(EnemyLocation, 0.1f, PlayerLocation);
        bool IsScopeOn = get_IsSighting(LocalPlayer);
        bool IsFiring = get_IsFiring(LocalPlayer);
        if (AimFire) {
            if (IsFiring) {
                set_aim(LocalPlayer, PlayerLook);  
            }
        }
        if (AimScope) {
            if (IsScopeOn) {
                set_aim(LocalPlayer, PlayerLook);  
            }
        }
        if (AimAuto) {
            set_aim(LocalPlayer, PlayerLook);  
        }
        
        if(rage_auto_aim) {
            if(IsScopeOn || IsFiring) {
                set_aim(LocalPlayer, PlayerLook);
            }
        }
    }
}
    
    
    

    if (main_enableall) {
        if (CurrentMatch != nullptr && LocalPlayer != nullptr) {
            MyDictionary* players = *(MyDictionary**)((long)CurrentMatch + ListPlayer);
            if (players && players->entries) {
                int count = players->count;
                for (int i = 0; i < count; i++) {
                    DictionaryEntry entry = players->entries->vector[i];
                    if (entry.hashCode >= 0) {
                        void* enemy = entry.value;
                        if (enemy && !get_isLocalTeam(enemy) && !get_IsDieing(enemy) && GetHp(enemy) > 0 && get_MaxHP(enemy) > 0 && get_isVisible(enemy)) {
                            Vector3 LocalBody = GetAttackableCenterWS(LocalPlayer);
                            Vector3 EnemyBody = GetAttackableCenterWS(enemy);
                            void* imo = get_imo(LocalPlayer);
                            if (imo) {
                                if (visual_espfire) {
                                    set_esp1(imo, LocalBody, EnemyBody, enemy);
                                }
                                if (visual_espbluefire) {
                                    set_esp3(imo, LocalBody, EnemyBody);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (main_enableall && AimSilentZaru && LocalPlayer) {        
        bool readyToGoEpsteinIsland = false;
        if (get_IsFiring(LocalPlayer)) {
            uintptr_t hitObjAddress = (uintptr_t)LocalPlayer + m_HawkerHitObject;
            void* hitObjInfo = *(void**)hitObjAddress;
            if (hitObjInfo != nullptr && closestEnemy != nullptr) {
                readyToGoEpsteinIsland = true;
                Vector3 enemyPos = GetHeadPosition(closestEnemy);
                Vector3 ammoBase = *(Vector3*)((uintptr_t)hitObjInfo + m_HawkerStartPos);
                if (TestSafeSilent) {
                    int hp = GetHp(closestEnemy);                    
                    if (hp < 50) {
                        enemyPos.y -= 0.45f; 
                    }
                }
                if ((ammoBase.x != 0 || ammoBase.y != 0) && (enemyPos.x != 0 || enemyPos.y != 0)) {
                    Vector3 dir;
                    dir.x = enemyPos.x - ammoBase.x;
                    dir.y = enemyPos.y - ammoBase.y;
                    dir.z = enemyPos.z - ammoBase.z;
                    silentLock.lock();                    
                    g_HitObjInfo = hitObjInfo;
                    g_TargetDir = dir;
                    g_HasData = true;
                    silentLock.unlock();
                }
            }
        }
        if (!readyToGoEpsteinIsland) {
            if (g_HasData) {
                silentLock.lock();
                g_HasData = false;
                g_HitObjInfo = nullptr;
                silentLock.unlock();
            }
        }
    }
    
    
    if (dikhtahuaenemy != nullptr && LocalPlayer != nullptr && CurrentMatch != nullptr) {
     
        // Teleport BR
        if (TeleportProBr && LocalPlayer) {
            void* _TeleKillTP = get_transform(dikhtahuaenemy);
            if (_TeleKillTP != NULL) {
                Vector3 TeleKillTP = get_position11(_TeleKillTP) - (GetForward(_TeleKillTP) * 0);
                set_position_Injected(get_transform(LocalPlayer), Vvector3(TeleKillTP.x, TeleKillTP.y, TeleKillTP.z));
                TeleportProBr = false;
            }
        }

        // Teleport CS
        if (misc_teleport) {
            void* enemyTf = get_transform(dikhtahuaenemy);
            Vector3 PosEnemy = get_position11(enemyTf) - (GetForward(enemyTf) * 1.0f);
            set_position_Injected(get_transform(LocalPlayer), Vvector3(PosEnemy.x, PosEnemy.y, PosEnemy.z));
            misc_teleport = false;
        }
    }
    
    


    if ((main_enableall ? (closestEnemy != nullptr ? (EspGrenade ? true : false) : false) : false)) {
        auto _getHead = [](void* ent) -> Vector3 {
            return ent ? GetHeadPosition(ent) : Vector3(0.0f, 0.0f, 0.0f);
        };
        Vector3 _local = _getHead(LocalPlayer);
        Vector3 _enemy = _getHead(closestEnemy);
        auto _offset = []() -> Vector3 {
            Vector3 base = {0.0f, 0.1f, 0.0f};
            return Vector3(base.x * 0.1f, base.y * 0.1f, base.z * 0.1f);
        };
        GrenadeLine_DrawLine(LineGrenade, (_local.x != 0.0f ? _local : Vector3()), _local, _offset());
        if (!!RenderLine) {
            struct {
                float r, g, b, a;
            } _col = {1.0f, 0.0f, 0.0f, 1.0f};
            auto _applyColor = [&](void* lr) {
                if (lr) LineRenderer_SetColor(lr, *(UnityColor*)&_col);
            };
            auto _setPositions = [&](void* lr, Vector3 a, Vector3 b) {
                if (!lr) return;
                LineRenderer_Set_PositionCount(lr, (int)(sizeof(void*) == 8 ? 2 : 0x2));
                LineRenderer_SetPosition(lr, 0, a);
                LineRenderer_SetPosition(lr, 1, b);
            };
            _applyColor(RenderLine);
            _setPositions(RenderLine, _local, _enemy);
        }
    }
    
    if (main_enableall && rage_legit_bot) {
        void* current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);
        void* closestEnemy = hackerrrr();
        if (closestEnemy != NULL && local_player != NULL && current_Match != NULL) {
            void* Current_Collider = *(void**)((uintptr_t)closestEnemy + 0x78);
            void* Head_Collider = Player_GetHeadCollider(closestEnemy);
            if (Current_Collider != Head_Collider) SetAimCollider(closestEnemy, Head_Collider);
        }
    }
    
    if (CurrentMatch != nullptr && LocalPlayer != nullptr) {
        static auto lastTipTime = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTipTime).count();
        if (elapsed >= 1500) {
static monoString* creditStr = (monoString*)il2cpp_string_new("-: * S3 HACKS ADV * :-");
static monoString* popupStr = (monoString*)il2cpp_string_new("S3 HACKS ADV");
            ShowCenterUpTeammateTips(creditStr, 1.9f);
            ShowPopupMessageNoAnim(popupStr, 1.7f);
            lastTipTime = now;
        }
    }
        

    
    if (main_enableall) {
        if (CurrentMatch != nullptr && LocalPlayer != nullptr) {
            if (LocalPlayer) {
                void* weapon = GetWeaponOnHand(LocalPlayer);
                float firecd = 0.0f;
                bool isfire = false;
                firecd += ImGui::GetIO().DeltaTime * fire;
                float delay = get_Delay(weapon);
                if (delay < AutoFireCD) {
                    delay = AutoFireCD;
                }
                if (rage_auto_fire) {
                    if (closestEnemy != nullptr && weapon != nullptr && !get_IsDieing(LocalPlayer)) {
                        bool weaponRange = isEnemyInRangeWeapon(LocalPlayer, closestEnemy, weapon);
                        if (weaponRange) {
                            if (firecd >= delay) {
                                StartFiring55(weapon);
                                firecd = 0.0f;
                                isfire = true;
                            }
                        } else {
                            if (isFiring) {
                                StopFiring55(weapon);
                                isfire = false;
                            }
                        }
                    } else {
                        if (isFiring) {
                            StopFiring55(weapon);
                            isfire = false;
                            firecd = 0.0f;
                        }
                    }
                }
            }
        }
    }
    
        if (main_enableall) {
    if (CurrentMatch != nullptr && LocalPlayer != nullptr) {
        if (LocalPlayer) {
            void* weapon = GetWeaponOnHand(LocalPlayer);
            static float firecd = 0.0f;
            static bool isfire = false;
            
            firecd += ImGui::GetIO().DeltaTime;
            
            float delay = get_Delay(weapon);
            if (delay < AutoFireCD) {
                delay = AutoFireCD;
            }
            
            if (rage_auto_fire) {
                if (closestEnemy != nullptr && weapon != nullptr && !get_IsDieing(LocalPlayer) && isEnemyInRangeWeapon(LocalPlayer, closestEnemy, weapon)) {
                    if (firecd >= delay) {
                        StartFiring55(weapon);
                        firecd = 0.0f;
                        isfire = true;
                    }
                } else {
                    if (isfire) {
                        StopFiring55(weapon);
                        isfire = false;
                        firecd = 0.0f;
                    }
                }
            }
        }
    }
}


if (SpinBot && LocalPlayer) {
    void* CurrentMatch = Curent_Match();
    
    if (g_LastMatch != CurrentMatch) {
        spinLock.lock();
        g_SpinBotReady = false;
        g_SpinBotEntity = nullptr;
        g_SpinBotAngle = 0.0f;
        spinLock.unlock();
        g_LastMatch = CurrentMatch;
    }
    
    if (g_LastMatch == CurrentMatch) {
        g_SpinBotAngle += SpinSpeed * ImGui::GetIO().DeltaTime;
        if (g_SpinBotAngle >= 360.0f) g_SpinBotAngle -= 360.0f;
        
        float rad = g_SpinBotAngle * (M_PI / 180.0f);
        Quaternion newRot;
        newRot.x = 0.0f;
        newRot.y = sin(rad * 0.5f);
        newRot.z = 0.0f;
        newRot.w = cos(rad * 0.5f);
        
        spinLock.lock();        
        g_SpinBotEntity = LocalPlayer;
        g_SpinBotData = newRot;
        g_SpinBotReady = true;
        spinLock.unlock();
    }
} else {
    if (g_SpinBotReady) {
        spinLock.lock();
        g_SpinBotReady = false;
        g_SpinBotEntity = nullptr;
        g_SpinBotAngle = 0.0f;
        spinLock.unlock();
    }
}

    
    
    void *WeaponHand = GetWeaponOnHand(LocalPlayer);
            rapidFireTimer += ImGui::GetIO().DeltaTime;
            bool manualFiring = get_IsFiring(LocalPlayer);
            if (main_enableall && misc_rapid_fire && LocalPlayer && manualFiring && WeaponHand != nullptr) {
                if (rapidFireTimer >= (get_Delay(WeaponHand) * 0.3f)) {
                    StartFiring55(WeaponHand);
                    rapidFireTimer = 0.0f;
                    rapidIsFiring = true;
                }
            } else {
                if (rapidIsFiring) {
                    StopFiring55(WeaponHand);
                    rapidIsFiring = false;
                }
            }
   
    if (main_enableall) {
        void* Simulation = GetSimulationTimer();
        if (Simulation != nullptr) {
            float FixedDeltaTime = GetTimer(Simulation);
            if(!saved) {
                active = FixedDeltaTime * 1.30f;
                desactive = FixedDeltaTime;
                saved = true;
            }
            if (misc_speed_hack) {
                if (FixedDeltaTime != active) {
                    SetTimer(Simulation, active);
                }
            } else {
                if (FixedDeltaTime != desactive) {
                    SetTimer(Simulation, desactive);
                }
            }
        }
    }
    
    LateUpdate(Player);
}

void AimSilentVoid() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        if (!g_HasData) continue;
        silentLock.lock();        
        void* currentHitObj = g_HitObjInfo;
        Vector3 currentDir = g_TargetDir;
        bool valid = g_HasData;
        silentLock.unlock();
        if (valid && currentHitObj != nullptr) {
            *(Vector3*)((uintptr_t)currentHitObj + m_HawkrtEndPos) = currentDir;
        }
    }
}

void SpinBotVoid() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
        if (!g_SpinBotReady) continue;
        
        spinLock.lock();
        void* targetEntity = g_SpinBotEntity;
        Quaternion targetRot = g_SpinBotData;
        bool active = g_SpinBotReady;
        spinLock.unlock();
        
        if (active && targetEntity != nullptr) {
            void* CurrentMatch = Curent_Match();
            if (CurrentMatch != g_LastMatch) {
                spinLock.lock();
                g_SpinBotReady = false;
                g_SpinBotEntity = nullptr;
                spinLock.unlock();
                continue;
            }
            
            if (!get_IsDieing(targetEntity)) {
                Entity_SetRotation(targetEntity, targetRot);
            }
        }
    }
}

int (*orig_PlayerNetwork_TakeDamage)(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID);
int hook_PlayerNetwork_TakeDamage(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) {
    
    void* CurrentMatch = Curent_Match();
    if (CurrentMatch != nullptr) {
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (LocalPlayer != nullptr) {
    
            if (LocalPlayer && ClosestEnemy) {
                firePos = GetHeadPosition(LocalPlayer);
               hitPos = GetHeadPosition(ClosestEnemy);
             }

            if (ClosestEnemy != NULL && Headshot) {
                if (damageInfo != NULL) {
                    *(int *)((long) damageInfo + offset_BodyPart) = 1;
                }
            }
        }
    }
    return orig_PlayerNetwork_TakeDamage(ClosestEnemy, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
}


int (*old_BLAGCMCGEJG1)(void *, HitObjectInfo *);
int BLAGCMCGEJG1(void *ist, HitObjectInfo *HitObject) {
    if (main_enableall && (rage_silent_aim || rage_auto_fire)) {
        if (HitObject != nullptr) {
            void *current_match = Curent_Match();
            if (current_match != nullptr) {
                void* local_player = GetLocalPlayer(current_match);
                if (local_player != nullptr) {
                    void* WeaponHand = GetWeaponOnHand(local_player);
                    
                    auto *ClosestEnemy = EnemyVisible(current_match);
                    
                    if (ClosestEnemy != nullptr) {
                        if (isEnemyInRangeWeapon(local_player, ClosestEnemy, WeaponHand)) {
                            Vector3 EnemyLocation;
                            
                            if (aimPosition == 0) {
                                EnemyLocation = GetHeadPosition(ClosestEnemy);
                            } 
                            else if (aimPosition == 2) {
                                EnemyLocation = GetHipPosition(ClosestEnemy); 
                            }                            
                            Vector3 PlayerLocation = CameraMain(local_player);
                            HitObject->HitObject = get_gameObject(Player_GetHeadCollider(ClosestEnemy));
                            HitObject->HitCollider = Player_GetHeadCollider(ClosestEnemy);
                            
                            HitObject->HitLocation = EnemyLocation;
                            HitObject->HitNormal = EnemyLocation;
                            HitObject->RayDir = Vector3::Normalized(EnemyLocation - PlayerLocation);
                            HitObject->StartPosition = PlayerLocation;
                            HitObject->OrigStartPosition = PlayerLocation;
                            HitObject->SpecialHitType = 0;
                            HitObject->HitGroup = 1;
                            HitObject->IgnoreHappens = false;
                            HitObject->ViewBlocked = false;
                        }
                    }
                }
            }
        }
    }
    return old_BLAGCMCGEJG1(ist, HitObject);
}

static void DrawSkeletonESP(void* closestEnemy, int glHeight) {
    if (!closestEnemy) return;

    void* camera = Camera_main();
    if (!camera) return;

    ImU32 col = ImGui::ColorConvertFloat4ToU32(EnemyColor);
    const float thick = LineSize;

    Vector3 headS  = WorldToScreenPoint(camera, GetHeaPosition1(closestEnemy));
    Vector3 chestS = headS; chestS.y -= 1.5f;
    Vector3 hipS   = WorldToScreenPoint(camera, GetHiPosition(closestEnemy));

    Vector3 lsS = WorldToScreenPoint(camera, GetLeftShoulderPosition(closestEnemy));
    Vector3 leS = WorldToScreenPoint(camera, GetLeftForeArmPosition(closestEnemy));
    Vector3 lhS = WorldToScreenPoint(camera, GetLeftHandPosition1(closestEnemy));
    Vector3 rsS = WorldToScreenPoint(camera, GetRightShoulderPosition(closestEnemy));
    Vector3 reS = WorldToScreenPoint(camera, GetRightForeArmPosition(closestEnemy));
    Vector3 rwS = WorldToScreenPoint(camera, GetRightHandPosition1(closestEnemy));
    Vector3 lcS = WorldToScreenPoint(camera, GetLeftAnklePosition(closestEnemy));
    Vector3 lfS = WorldToScreenPoint(camera, GetLeftToePosition1(closestEnemy));
    Vector3 rcS = WorldToScreenPoint(camera, GetRightAnklePosition(closestEnemy));
    Vector3 rfS = WorldToScreenPoint(camera, GetRightToePosition1(closestEnemy));

    auto toIm = [&](const Vector3& v){
        return ImVec2(v.x, glHeight - v.y);
    };

    ImDrawList* dl = ImGui::GetBackgroundDrawList();

    auto L = [&](Vector3 a, Vector3 b){
        if (valid(a) && valid(b))
        dl->AddLine(toIm(a), toIm(b), col, thick);
    };

    if (valid(headS) && valid(chestS)) {
        ImVec2 neckP = toIm(chestS);
        ImVec2 headP = toIm(headS);

        float dx = headP.x - neckP.x;
        float dy = headP.y - neckP.y;

        ImVec2 extendedHead = ImVec2(neckP.x + (dx * 2.5f), neckP.y + (dy * 2.5f));
        dl->AddLine(neckP, extendedHead, col, thick);
    }

    L(chestS, hipS);

    L(chestS, lsS);
    L(lsS, leS);
    L(leS, lhS);

    L(chestS, rsS);
    L(rsS, reS);
    L(reS, rwS);

    L(hipS, lcS);
    L(lcS, lfS);

    L(hipS, rcS);
    L(rcS, rfS);
}


bool FlyHack = false;
float FlySpeed = 20.0f;
float TargetHeight = 15.0f;
static bool flyActive = false;

void UpdateFlyHack() {
    if (!FlyHack) {
        flyActive = false;
        return;
    }
    
    void* match = Curent_Match();
    void* local = GetLocalPlayer(match);
    if (!match || !local) return;
    
    void* localTr = Component_GetTransform(local);
    if (!localTr) return;
    
    Vector3 currentPos = Transform_GetPosition(localTr);
    Vector3 newPos = currentPos;
    
    if (!flyActive) {
        newPos.y = TargetHeight;
        flyActive = true;
    } else {
        newPos.y = TargetHeight;
    }
    
    void* cam = Camera_main();
    if (cam) {
        void* camTrans = Component_GetTransform(cam);
        Vector3 camFwd = GetForward(camTrans);
        
        float fwdLen = sqrt(camFwd.x * camFwd.x + camFwd.z * camFwd.z);
        if (fwdLen > 0) {
            camFwd.x /= fwdLen;
            camFwd.z /= fwdLen;
        }
        
        newPos.x += camFwd.x * FlySpeed * ImGui::GetIO().DeltaTime;
        newPos.z += camFwd.z * FlySpeed * ImGui::GetIO().DeltaTime;
    }
    
    Transform_INTERNAL_SetPosition(localTr, Vvector3(newPos.x, newPos.y, newPos.z));
}

bool UpPlayer = false;
bool TeleportToEnemy = false;
static float savedY = 0.0f;
static void* lastTargetID = nullptr;
static bool alreadySavedY = false;
static std::map<void*, Vector3> UpPlayerOriginalPos;

void ResetUpPlayerState() {
    savedY = 0.0f;
    alreadySavedY = false;
    lastTargetID = nullptr;
}

void UpdateUpPlayer() {
    if (!UpPlayer && !TeleportToEnemy) {
        for (auto& pair : UpPlayerOriginalPos) {
            void* enemy = pair.first;
            void* tr = Component_GetTransform(enemy);
            if (tr && !get_IsDieing(enemy)) {
                Transform_INTERNAL_SetPosition(tr, Vvector3(
                    pair.second.x, pair.second.y, pair.second.z
                ));
            }
        }
        UpPlayerOriginalPos.clear();
        ResetUpPlayerState();
        return;
    }
    
    void* match = Curent_Match();
    if (!match) return;
    
    void* local = GetLocalPlayer(match);
    if (!local) return;
    
    void* localTr = Component_GetTransform(local);
    if (!localTr) return;
    
    void* closestEnemy = hackerrrr();
    if (!closestEnemy) return;
    
    void* enemyTransform = Component_GetTransform(closestEnemy);
    if (!enemyTransform) return;
    
    bool isFiring = get_IsFiring(local);
    
    if (!isFiring) {
        auto it = UpPlayerOriginalPos.find(closestEnemy);
        if (it != UpPlayerOriginalPos.end()) {
            Transform_INTERNAL_SetPosition(enemyTransform, Vvector3(
                it->second.x, it->second.y, it->second.z
            ));
            UpPlayerOriginalPos.erase(it);
        }
        ResetUpPlayerState();
        return;
    }
    
    Vector3 enemyPosition = getPosition(closestEnemy);
    Vector3 playerPosition = getPosition(local);
    
    // TELEPORT TO ENEMY - moves YOU to enemy
    if (TeleportToEnemy) {
        Transform_INTERNAL_SetPosition(localTr, Vvector3(
            enemyPosition.x,
            enemyPosition.y,
            enemyPosition.z
        ));
        return;
    }
    
    // UP PLAYER - lifts enemy up
    if (UpPlayer) {
        if (UpPlayerOriginalPos.find(closestEnemy) == UpPlayerOriginalPos.end()) {
            UpPlayerOriginalPos[closestEnemy] = enemyPosition;
        }
        
        if (closestEnemy != lastTargetID || !alreadySavedY) {
            savedY = UpPlayerOriginalPos[closestEnemy].y + 1.5f;
            alreadySavedY = true;
            lastTargetID = closestEnemy;
        }
        Transform_INTERNAL_SetPosition(enemyTransform, Vvector3(enemyPosition.x, savedY, enemyPosition.z));
    }
}

bool AimPull = false;
float AIM_MAGNET_RANGE = 15.0f;
float PULL_POWER = 1.0f;  // 1.0 = instant, 0.9 = smooth
static std::map<void*, Vector3> MagnetOriginalPos;
static void* currentTarget = nullptr;

void UpdateAimMagnet() {
    if (!AimPull) return;
    
    void* match = Curent_Match();
    void* local = GetLocalPlayer(match);
    if (!match || !local) return;
    
    bool isFiring = get_IsFiring(local);
    bool isScoping = get_IsSighting(local);
    bool isActive = isFiring || isScoping;
    
    void* cam = Camera_main();
    if (!cam) return;
    
    void* camTrans = Component_GetTransform(cam);
    Vector3 camPos = Transform_GetPosition(camTrans);
    Vector3 camFwd = GetForward(camTrans);
    
    float len = sqrt(camFwd.x*camFwd.x + camFwd.y*camFwd.y + camFwd.z*camFwd.z);
    if (len > 0) {
        camFwd.x /= len;
        camFwd.y /= len;
        camFwd.z /= len;
    }
    
    void* dict = *(void**)((uintptr_t)match + ListPlayer);
    if (!dict) return;
    
    void* entriesArray = *(void**)((uintptr_t)dict + 0x18);
    int count = *(int*)((uintptr_t)dict + 0x20);
    if (!entriesArray || count <= 0) return;
    
    uintptr_t start = (uintptr_t)entriesArray + 0x20;
    
    // Restore all when not active
    if (!isActive) {
        for (int i = 0; i < count; i++) {
            DictionaryEntry* entry = (DictionaryEntry*)(start + i * sizeof(DictionaryEntry));
            if (entry->hashCode < 0) continue;
            
            void* enemy = entry->value;
            auto it = MagnetOriginalPos.find(enemy);
            if (it != MagnetOriginalPos.end()) {
                void* tr = Component_GetTransform(enemy);
                if (tr) {
                    Transform_INTERNAL_SetPosition(tr, Vvector3(
                        it->second.x, it->second.y, it->second.z
                    ));
                    MagnetOriginalPos.erase(enemy);
                }
            }
        }
        currentTarget = nullptr;
        return;
    }
    
    // Find closest enemy to crosshair
    void* closestEnemy = nullptr;
    float closestDist = 999999.0f;
    Vector3 bestCrosshairPoint;
    float bestEnemyHeight = 0.0f;
    
    for (int i = 0; i < count; i++) {
        DictionaryEntry* entry = (DictionaryEntry*)(start + i * sizeof(DictionaryEntry));
        if (entry->hashCode < 0) continue;
        
        void* enemy = entry->value;
        if (!enemy || get_isLocalTeam(enemy) || get_IsDieing(enemy)) continue;
        
        Vector3 enemyRoot = getPosition(enemy);
        Vector3 enemyHead = GetHeadPosition(enemy);
        float enemyHeight = enemyHead.y - enemyRoot.y;
        
        Vector3 toEnemy = {
            enemyHead.x - camPos.x,
            enemyHead.y - camPos.y,
            enemyHead.z - camPos.z
        };
        float distToEnemy = sqrt(toEnemy.x*toEnemy.x + toEnemy.y*toEnemy.y + toEnemy.z*toEnemy.z);
        
        Vector3 crosshairPoint = {
            camPos.x + camFwd.x * distToEnemy,
            camPos.y + camFwd.y * distToEnemy,
            camPos.z + camFwd.z * distToEnemy
        };
        
        float dx = enemyHead.x - crosshairPoint.x;
        float dy = enemyHead.y - crosshairPoint.y;
        float dz = enemyHead.z - crosshairPoint.z;
        float dist3D = sqrt(dx*dx + dy*dy + dz*dz);
        
        if (dist3D <= AIM_MAGNET_RANGE && dist3D < closestDist) {
            closestDist = dist3D;
            closestEnemy = enemy;
            bestCrosshairPoint = crosshairPoint;
            bestEnemyHeight = enemyHeight;
        }
    }
    
    if (!closestEnemy) return;
    
    void* tr = Component_GetTransform(closestEnemy);
    if (!tr) return;
    
    // Store original position
    if (MagnetOriginalPos.find(closestEnemy) == MagnetOriginalPos.end()) {
        MagnetOriginalPos[closestEnemy] = getPosition(closestEnemy);
    }
    
    Vector3 currentPos = getPosition(closestEnemy);
    
    // Calculate target root position
    Vector3 targetRoot = {
        bestCrosshairPoint.x,
        bestCrosshairPoint.y - bestEnemyHeight,
        bestCrosshairPoint.z
    };
    
    // Apply pull (instant if PULL_POWER == 1.0)
    if (PULL_POWER >= 0.99f) {
        // Instant snap
        Transform_INTERNAL_SetPosition(tr, Vvector3(targetRoot.x, targetRoot.y, targetRoot.z));
    } else {
        // Smooth pull
        Vector3 newPos;
        newPos.x = currentPos.x + (targetRoot.x - currentPos.x) * PULL_POWER;
        newPos.y = currentPos.y + (targetRoot.y - currentPos.y) * PULL_POWER;
        newPos.z = currentPos.z + (targetRoot.z - currentPos.z) * PULL_POWER;
        Transform_INTERNAL_SetPosition(tr, Vvector3(newPos.x, newPos.y, newPos.z));
    }
    
    currentTarget = closestEnemy;
}


bool AirCreep = false;
float AirCreepHeight = 15.0f;
static bool airCreepActivated = false;

void UpdateAirCreep() {
    if (!AirCreep) return;
    
    void* match = Curent_Match();
    void* local = GetLocalPlayer(match);
    if (!match || !local) return;
    
    void* localTr = Component_GetTransform(local);
    if (!localTr) return;
    
    Vector3 currentPos = Transform_GetPosition(localTr);
    
    if (!airCreepActivated) {
        Transform_INTERNAL_SetPosition(localTr, Vvector3(
            currentPos.x,
            AirCreepHeight,
            currentPos.z
        ));
        airCreepActivated = true;
    }
}

void ResetAirCreep() {
    if (!AirCreep) {
        airCreepActivated = false;
    }
}

bool SilentAim = false;
static Vector3 g_SilentDirection = {0, 0, 0};
static float g_SilentTimer = 0.0f;

void UpdateSilentAim() {
    if (!SilentAim) return;
    
    void* match = Curent_Match();
    void* local = GetLocalPlayer(match);
    if (!match || !local) return;
    
    bool isFiring = get_IsFiring(local);
    
    if (!isFiring) return;
    
    void* closestEnemy = EnemyVisible(match);
    if (!closestEnemy) return;
    
    uintptr_t hitObjAddress = (uintptr_t)local + m_HawkerHitObject;
    void* hitObjInfo = *(void**)hitObjAddress;
    
    if (!hitObjInfo) return;
    
    Vector3 enemyPos = GetHeadPosition(closestEnemy);
    Vector3 ammoBase = *(Vector3*)((uintptr_t)hitObjInfo + m_HawkerStartPos);
    
    if ((ammoBase.x == 0 && ammoBase.y == 0 && ammoBase.z == 0) || 
        (enemyPos.x == 0 && enemyPos.y == 0 && enemyPos.z == 0)) return;
    
    Vector3 dir;
    dir.x = enemyPos.x - ammoBase.x;
    dir.y = enemyPos.y - ammoBase.y;
    dir.z = enemyPos.z - ammoBase.z;
    
    *(Vector3*)((uintptr_t)hitObjInfo + m_HawkrtEndPos) = dir;
}


void DrawEnemyCount(int totalEnemies) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;
    
    std::string prefix = "ENEMIES : ";
    std::string number = std::to_string(totalEnemies);
    
    float fontSize = 32.0f;
    float thickness = 2.5f;  // Thick text
    
    ImVec2 prefixSize = ImGui::CalcTextSize(prefix.c_str());
    ImVec2 numberSize = ImGui::CalcTextSize(number.c_str());
    
    float totalWidth = prefixSize.x + numberSize.x;
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, 45.0f);
    ImVec2 startPos = ImVec2(center.x - totalWidth / 2.0f, center.y - prefixSize.y / 2.0f);
    
    // White "ENEMIES : " with thick text
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                draw->AddText(nullptr, fontSize, ImVec2(startPos.x + dx, startPos.y + dy), 
                              ImColor(0, 0, 0, 200), prefix.c_str());
            }
        }
    }
    draw->AddText(nullptr, fontSize, startPos, ImColor(255, 255, 255, 255), prefix.c_str());
    
    // Red number with thick text
    ImVec2 numberPos = ImVec2(startPos.x + prefixSize.x, startPos.y);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                draw->AddText(nullptr, fontSize, ImVec2(numberPos.x + dx, numberPos.y + dy), 
                              ImColor(0, 0, 0, 200), number.c_str());
            }
        }
    }
    draw->AddText(nullptr, fontSize, numberPos, ImColor(255, 50, 50, 255), number.c_str());
}

static int totalEnemies = 0;

// Add these globals at the top of Hooks.h with your other ESP bools
int EspLineMode = 0;  // 0 = Top (normal to head), 1 = Bottom (crosshair to head), 2 = Down (down to enemy)

void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw || !main_enableall) return;

    void* current_Match = Curent_Match();    
    if (!current_Match) return;    
        
    void* local_player = GetLocalPlayer(current_Match);    
    if (!local_player) return;    

    MyDictionary* players = *(MyDictionary**)((long)current_Match + ListPlayer);    
    void* camera = Camera_main();    
      
    if (!players || !players->entries || !camera) return;    

    totalEnemies = 0;

    for (int i = 0; i < players->count; ++i) {    
        DictionaryEntry entry = players->entries->vector[i];  
        if (entry.hashCode < 0) continue;   
          
        void* enemy = entry.value;    
        if (!enemy || enemy == local_player) continue;  
        if (get_isLocalTeam(enemy) && !EspTeammate) continue;  
        if (!get_isVisible(enemy)) continue;    

        Vector3 toePos = getPosition(enemy);    
        Vector3 toeScreen = WorldToScreenPoint(camera, toePos);    
        if (toeScreen.z < 1) continue;    

        Vector3 headPos = toePos + Vector3(0, 1.7f, 0);    
        Vector3 headScreen = WorldToScreenPoint(camera, headPos);    
        if (headScreen.z < 1) continue;    

        float distance = Vector3::Distance(getPosition(local_player), headPos);    

        bool isKnocked = get_IsDieing(enemy);    
        bool isTeammate = get_isLocalTeam(enemy);  

        float baseHeight = abs(headScreen.y - toeScreen.y) * 1.2f;
        
        // DYNAMIC SCALE - expands when close, shrinks when far
        float distanceScale = std::clamp(1.8f - (distance / 200.0f), 0.6f, 1.8f);
        
float scale;
if (isKnocked) {
    scale = std::clamp(0.5f + (distance / 800.0f), 0.5f, 1.2f) * distanceScale;
} else {
    scale = std::clamp(0.5f + (distance / 800.0f), 0.5f, 1.2f) * distanceScale;
}
        
        float height = baseHeight * scale;
        float width = height * 0.5f;
        
        float x = headScreen.x - width / 2.f;
        float y = screenHeight - headScreen.y;
        
        float lineThickness = std::clamp(2.5f + (distance / 400.0f), 2.0f, 4.0f);
        float boxThickness = std::clamp(2.0f + (distance / 500.0f), 1.5f, 3.5f);
        
        ImColor EspColor = isTeammate  
            ? ImColor(TeamColor)  
            : (isKnocked ? ImColor(KnockColor) : ImColor(EnemyColor));  

        if (!isTeammate && !get_IsDieing(enemy) && GetHp(enemy) > 0) {
            totalEnemies++;
        }
          
        // ESP LINE with 3 modes
        if (visuals_player_esp) {  
            ImVec2 startPoint;
            ImVec2 hitPoint = ImVec2(x + width * 0.5f, y);
            
            // Mode selection
            if (EspLineMode == 0) {
                // TOP MODE - Normal line from top center of screen to enemy head
                startPoint = ImVec2(screenWidth * 0.5f, 82.5f);
            } else if (EspLineMode == 1) {
                // BOTTOM MODE - Line from crosshair (screen center) to enemy head
                startPoint = ImVec2(screenWidth * 0.5f, screenHeight * 0.5f);
            } else {
                // DOWN MODE - Line from bottom of enemy box to ground/down direction
                startPoint = ImVec2(x + width * 0.5f, y + height);
                hitPoint = ImVec2(x + width * 0.5f, y);
            }
            
            // Glow effect
            for (int g = 1; g <= 3; g++) {
                draw->AddLine(startPoint, hitPoint, 
                    IM_COL32(EspColor.Value.x * 255, EspColor.Value.y * 255, EspColor.Value.z * 255, 80 - g * 20), 
                    lineThickness + g);
            }
            draw->AddLine(startPoint, hitPoint, EspColor, lineThickness);  
        }  
              
        // ESP BOX with glow border
        if (visuals_box_esp) {  
            ImVec2 min(x, y);  
            ImVec2 max(x + width, y + height);  

            if (EspFill) {
                float fillAlpha = std::clamp(0.1f + (distance / 1500.0f), 0.05f, 0.25f);
                draw->AddRectFilled(min, max, IM_COL32(255, 255, 255, (int)(fillAlpha * 255)), 2.0f);
            }
            
            float rounding = std::clamp(2.0f + (distance / 400.0f), 1.5f, 5.0f);
            
            // GLOW BORDER
            for (int g = 1; g <= 3; g++) {
                draw->AddRect(
                    ImVec2(min.x - g, min.y - g), 
                    ImVec2(max.x + g, max.y + g), 
                    IM_COL32(EspColor.Value.x * 255, EspColor.Value.y * 255, EspColor.Value.z * 255, 100 - g * 25), 
                    rounding + g, 0, boxThickness + g);
            }
            
            draw->AddRect(min, max, EspColor, rounding, 0, boxThickness);  
        }
          
        if (visuals_health_esp) {           
            long clr = ImColor(0,255,0,255);               
            if (GetHp(enemy) <= (get_MaxHP(enemy) * 0.6)) clr = ImColor(255, 255, 0, 255);
            if (GetHp(enemy) < (get_MaxHP(enemy) * 0.3)) clr = ImColor(255, 0, 0, 255);
            
            float healthBarWidth = std::clamp(4.0f * scale, 3.0f, 8.0f);
            float healthX = x + width + 2;
            float healthY = y;
            float healthHeight = height;
            
            draw->AddRectFilled(ImVec2(healthX, healthY), ImVec2(healthX + healthBarWidth, healthY + healthHeight), ImColor(0, 0, 0, 200));        
            draw->AddRectFilled(ImVec2(healthX + 1, healthY + healthHeight - (healthHeight * ((float)GetHp(enemy) / get_MaxHP(enemy)))), 
                                ImVec2(healthX + healthBarWidth - 1, healthY + healthHeight), clr);                                        
        }
          
        if (visuals_skeleton_esp) {        
            DrawSkeletonESP(enemy, screenHeight);        
        }  
        
        if (visual_distance_esp) {
            monoString* Nick = get_NickName(enemy);
            std::string names;
            if (Nick != nullptr) {
                int nick_Len = Nick->getLength();
                for (int i = 0; i < nick_Len; i++) {
                    char data = get_Chars(Nick, i);
                    names += isascii(data) ? data : '?';
                }
            }
            
            float textScale = std::clamp(0.8f + (distance / 600.0f), 0.8f, 1.4f);
            float textX = x + (width / 2);
            float nameY = y - 18 * textScale;
            float distY = y + height + 12 * textScale;
            
            if (names.length() > 0) {
                ImVec2 nameSize = ImGui::CalcTextSize(names.c_str());
                draw->AddText(nullptr, 13.0f * textScale, ImVec2(textX - nameSize.x/2, nameY), 
                              IM_COL32(255, 255, 255, 255), names.c_str());
            }
            
            char distText[32];
            snprintf(distText, sizeof(distText), "%.0fm", distance);
            ImVec2 distSize = ImGui::CalcTextSize(distText);
            draw->AddText(nullptr, 11.0f * textScale, ImVec2(textX - distSize.x/2, distY), 
                          ImColor(DistColor), distText);
        }
    }
    
    if (CountDih && totalEnemies > 0) {
        DrawEnemyCount(totalEnemies);
    }
      
    if (visual_show_fov) {        
        ImVec2 screenCenter = ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
        
        for (int g = 1; g <= 3; g++) {
            draw->AddCircle(screenCenter, Fov_Aim + g, 
                IM_COL32(FovColor.Value.x * 255, FovColor.Value.y * 255, FovColor.Value.z * 255, 80 - g * 20), 
                64, LineSize + g);
        }
        draw->AddCircle(screenCenter, Fov_Aim, ImColor(FovColor), 64, LineSize);        
    }
}



int (*orig_get_DomesticVariant)();
int my_get_DomesticVariant() {
    return 2;
}

int (*orig_get_SystemPlatform)();
int my_get_SystemPlatform() {
    return 2;
}

int (*orig_get_ChannelType)();
int my_get_ChannelType() {
    return 0;
}

int (*orig_get_Architecture)();
int my_get_Architecture() {
    return 3;
}

bool (*orig_get_IsHPE)();
bool my_get_IsHPE() {
    return false;
}




bool (*orig_NeedShowBloodOption)();
bool my_NeedShowBloodOption() {
    return true;
}

int (*orig_GetCurrentBloodType)(bool pcIgnoreBloodSwitch);
int my_GetCurrentBloodType(bool pcIgnoreBloodSwitch) {
    return 1;
}

bool (*orig_IsIndiaNoblood)();
bool my_IsIndiaNoblood() {
    return false;
}
