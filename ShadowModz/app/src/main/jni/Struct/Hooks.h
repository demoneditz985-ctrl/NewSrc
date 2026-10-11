#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <imgui.h>
#include <mutex>
#include <deque>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include <Struct/main.h>
#include "obfuscate.h"
#include "Unity/unity.h"
#include "Class.h"
#include "Rect.h"
inline static int g_GlHeight, g_GlWidth;
#include "Bools.h"
#include <math.h>
#include <stdio.h>
#include "KittyMemory/MemoryPatch.h"

typedef char PACKAGENAME;


bool Distance;
bool AimSilentZaru = false;
bool TestSafeSilent = false;

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



#define m_HawkerStartPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("BOGOIAMJFDN"))
#define m_HawkrtEndPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("NHKKHPLFMNG"))
#define m_HawkerHitObject (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GEGFCFDGGGP"))
bool UnlockTraining;

bool AimbotRage = false;
int AimTarget = 1;
bool AimbotLegit;

struct Vector2 {
    float x, y;
};



struct sColor {
        
        float line[4] = {45 / 255.0f, 180 / 255.0f, 45 / 255.0f, 255 / 255.0f};
        float box[4] = {45 / 255.0f, 180 / 255.0f, 45 / 255.0f, 255 / 255.0f};
        
    };
    
    sColor Color;
   
   
bool isOutsideScreen(ImVec2 pos, ImVec2 screen) {
    if (pos.y < 0) {
        return true;
    }
    if (pos.x > screen.x) {
        return true;
    }
    if (pos.y > screen.y) {
        return true;
    }
    return pos.x < 0;
}

ImVec2 pushToScreenBorder(ImVec2 Pos, ImVec2 screen, int offset) {
    int x = (int) Pos.x;
    int y = (int) Pos.y;
    
    if (Pos.y < 0) {
        y = -offset;
    }
    
    if (Pos.x > screen.x) {
        x = (int) screen.x + offset;
    }
    
    if (Pos.y > screen.y) {
        y = (int) screen.y + offset;
    }
    
    if (Pos.x < 0) {
        x = -offset;
    }
    return ImVec2(x, y);
}

static float tm = 127 / 255.f;

void DrawCircleHealth(ImVec2 position, int health, int max_health, float radius) {
    float a_max = ((3.14159265359f * 2.0f));
    ImU32 healthColor = IM_COL32(45, 180, 45, 255);
    if (health <= (max_health * 0.6)) {
        healthColor = IM_COL32(180, 180, 45, 255);
    }
    if (health < (max_health * 0.3)) {
        healthColor = IM_COL32(180, 45, 45, 255);
    }
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / max_health) * (max_health - health), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 4);
}



void drawText2(int x, int y, long color, const char *str, float size) {
        ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), color, str);
    }


void DrawRectFilled(int x, int y, int w, int h, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               color, 0, 0);
}


void DrawRectFilledHealth(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}


void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int T) {
    ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                                                   color);
}
bool IsVisible(void * player){
    if(player != NULL) {
        void *hitObj = NULL;
        

        
        Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(Camera_main()));
        Vector3 headLocation = Transform_GetPosition(Component_GetTransform(Player_GetHeadCollider(player)));
            return !Physics_Raycast(cameraLocation, headLocation, 12, &hitObj);
    }
    return false;
}


void DrawHealths(Rect box, float entityHealth, float maxHealth, int TeamID, const char* name, long color) {
    float x = box.x - (140 - box.w) / 2;
    float y = box.y;
    char dis[20];
    // sprintf(dis, "%0.fM", d);

    char TeamText[20];
    sprintf(TeamText, "%d", (int)TeamID);
    ImVec4 HPColor = ImVec4{1, 1, 0, 127 / 255.f}; // Màu mặc định cho máu

    if (entityHealth < maxHealth) {
        float healthPercentage = entityHealth / maxHealth;

        if (healthPercentage < 0.3f) {
            HPColor = ImVec4{0.5f, 0.0f, 0.0f, 127 / 255.f}; // Màu đỏ cho máu thấp
        } else if (healthPercentage < 0.6f) {
            HPColor = ImVec4{1, 0, 0, 127 / 255.f}; // Màu cam cho máu trung bình
        }
    }

    tm = 100.f / 255.f;
    DrawRectFilled(x - strlen(name), y - 41, 120 + strlen(name) * 2, 20, color); // Đám mây nền cho tên người chơi
    tm = 255.f / 255.f;
    DrawRectFilled(x - strlen(name), box.y - 41, 25, 20, color);
    if (strlen(TeamText) < 2) {
        drawText2(x + 6 - strlen(name), box.y - 42, ImColor(255,255,255),
                       TeamText, 18.943);//队伍ID
    } else {
        drawText2(x - strlen(name), box.y - 42, ImColor(255,255,255),
                       TeamText, 18.943);//队伍ID
    }
    drawText2(x + 28 - strlen(name), y - 43,
                   ImColor(255,255,255), name, 18.943);//玩家名称
    float maxWidth = 120; // Độ dài tối đa của thanh máu
    float healthBarWidth = entityHealth * maxWidth / maxHealth; // Độ dài thanh máu dựa trên tỷ lệ máu hiện tại và máu tối đa
    DrawRectFilledHealth(x - maxWidth / 2, y - 18, healthBarWidth, 8, HPColor);// cập nhật vị trí và kích thước của thanh máu
    drawText2(x + 125 + strlen(name), y - 43, ImColor(255,255,255), dis,18.943);//距离显示
    DrawTriangle(box.x + box.w / 2 - 10, y - 8, box.x + box.w / 2 + 15 - 10, y - 8,
                 box.x + box.w / 2 - 2, y, ImColor(255,255,255), 1);//第一个左边 第二个右边 第三个底边
}



static float SetFieldOfView = 0, GetFieldOfView = 0;

void DroneView() {
    if (GetFieldOfView == 0) {
        GetFieldOfView = get_fieldOfView();
    }
    if (SetFieldOfView > 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView + SetFieldOfView);
    }
    if (SetFieldOfView == 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView);
    }
}

Quaternion GetRotationToTheLocation(Vector3 Target, float Height, Vector3 MyEnemy) {
return Quaternion::LookRotation((Target + Vector3(0, Height, 0)) - MyEnemy, Vector3(0, 1, 0));
}

void *GetClosestEnemy() {
    float shortestDistance = 9999;
    void* closestEnemy = NULL;   
     
    void* get_MatchGame = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);

    if (LocalPlayer != NULL && get_MatchGame != NULL && Enable) {
        MyDictionary* dict = *(MyDictionary**)((uintptr_t)get_MatchGame + ListPlayer);
        
        if (dict != nullptr && dict->entries != nullptr) {
            void* camera = Camera_main();
            int total = dict->count;

            for (int u = 0; u < total; u++) {
                void* Player = dict->entries->vector[u].value; 
                
                if (Player != NULL && Player != LocalPlayer && !get_isLocalTeam(Player) && !get_IsDieing(Player) && get_isVisible(Player) && get_MaxHP(Player)) {    
                    Vector3 PlayerPos = getPosition(Player);
                    Vector3 LocalPlayerPos = getPosition(LocalPlayer);
                    Vector3 pos2 = WorldToScreenPoint(camera, PlayerPos);
                    
                    bool isFov1 = isFov(Vector3(pos2.x, pos2.y), Vector3(g_GlWidth / 2, g_GlHeight / 2), Fov_Aim);
                    
                    float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
                    if (distance < Aimdis) {
                        Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                        float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(camera))) * 100.0;             
                
                        if (angle <= Fov_Aim && isFov1 && angle < shortestDistance) {
                            shortestDistance = angle;
                            closestEnemy = Player;
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;
}

float (*get_deltaTime)();
float smoothSpeedAIM = 5; 
void *MyPlayer = NULL;
bool Drone;
const float RADAR_SIZE = 200.0f; 
const ImVec2 RADAR_CENTER(100.0f, 100.0f); 
float x,y;
float enemy1 = 2.0f;
bool RAMA_MODZ = true;
bool RamaModz = true;

bool block_connections = true;
#include <string>
ImVec4 colorTextBackground = ImVec4(0, 0, 0, 255); 
ImVec4 colorBox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);          
ImVec4 colorHealth = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); 
ImVec4 colorDistance = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
ImVec4 colorLine = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);        
ImVec4 colorHexagono = ImVec4(150 / 255.0f, 0, 255 / 255.0f, 1.0f);
ImVec4 colorMenuText = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
void* CurrentMatch = nullptr;
void* closestEnemy = nullptr;
void* LocalPlayer = nullptr;


float aimSmoothness = 0.1f;

inline void DrawAIMBOT(float screenWidth, float screenHeight) {
     
}


void AimbotLegitVoid() {
    if (AimbotLegit) {
        void* current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);
        void* closestEnemy = GetClosestEnemy();
        if (closestEnemy != NULL && local_player != NULL && current_Match != NULL) {
            void* Current_Collider = *(void**)((uintptr_t)closestEnemy + 0x78);
            void* Head_Collider = Player_GetHeadCollider(closestEnemy);
            if (Current_Collider != Head_Collider) SetAimCollider(closestEnemy, Head_Collider);
        }
    }
}


bool HighFps = false;

void UpdateHighFps() {
    void* addr = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameSettingData"), OBFUSCATE("CachedSmoothHighFrame"), 0);
    if (addr != 0) {
        if (HighFps) {
            MemoryPatch::createWithHex((uintptr_t)addr, OBFUSCATE("20 00 80 52 C0 03 5F D6")).Modify();
        } else {
            MemoryPatch::createWithHex((uintptr_t)addr, OBFUSCATE("00 00 80 52 C0 03 5F D6")).Modify();
        }
    }
}

bool GuestResetZ = false;

void UpdateResetGuest() {
    void* addr = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameConfig"), OBFUSCATE("get_ResetGuest"), 0);
    if (addr == 0) return;
    
    if (GuestResetZ) {
        MemoryPatch::createWithHex((uintptr_t)addr, OBFUSCATE("20 00 80 52 C0 03 5F D6")).Modify();
    } else {
        MemoryPatch::createWithHex((uintptr_t)addr, OBFUSCATE("00 00 80 52 C0 03 5F D6")).Modify();
    }
}

#define offset_GetWeaponOnHand1 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetWeaponOnHand"), 0)
static void *GetWeaponOnHand1(void *localPlayer) {
    if (!localPlayer) return nullptr;
    auto _GetWeapon = (void *(*)(void *))(offset_GetWeaponOnHand1);
    return _GetWeapon(localPlayer);
}

#define offset_StopFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("FDAEPHMIEPC"), OBFUSCATE("KCKLGPLBMEN"), 0)

static void StopFire(void *weapon_instance) {
    if (!weapon_instance) return;
    auto _StopFire = (void (*)(void *))(offset_StopFire);
    _StopFire(weapon_instance);
}

#define offset_StartFiringer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("FDAEPHMIEPC"), OBFUSCATE("AMBIBKLKGNB"), 0)
static void StartFiring(void *weapon_instance) {
    if (!weapon_instance) return;
    auto _StartFire = (void (*)(void *))(offset_StartFiringer);
    _StartFire(weapon_instance);
}

bool SilentAimv222 = false;
bool ignoreEnemyBot    = false;
bool ignoreKnocked     = true;
bool checkVisible2 = true;
static int aimPosition = 0;
const float NECK_OFFSET = -0.15f;
bool Headshot = false;

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


static bool IsValid(void* obj) {
    return obj != nullptr; 
}

float AutoFire_Rate = 0.15f; 
bool AutoFire_IsFiringState = false; 
bool Autofire; // تم إضافتها لتجنب خطأ Undeclared

void autofireupdate(void* local) {
    if (!local || get_IsDieing(local)) {
        return;
    }

    void* weapon = GetWeaponOnHand1(local);
    if (!weapon) {
        return;
    }

    void* target = GetClosestEnemy();

    if (!target || !IsValid(target) || get_IsDieing(target)) {
        if (AutoFire_IsFiringState) {
            StopFire(weapon);
            AutoFire_IsFiringState = false;
        }
        return;
    }

    bool isVisible = IsVisible(target);  // assume function exists

    if (Autofire && isEnemyInRangeWeapon(local, target, weapon) && isVisible) {
        static float AutoFire_Timer = 0.0f;
        AutoFire_Timer += ImGui::GetIO().DeltaTime;
        if (AutoFire_Timer >= AutoFire_Rate) {
            StartFiring(weapon);
            StopFire(weapon);
            AutoFire_Timer = 0.0f;
        }
    }
}



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

int SilentDamage = 100;

void* EnemySlientAim() {
    float shortestDistance = 200.0f;
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

#define offset_BodyPart (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GLLLEDKLLDA"), OBFUSCATE("DNFNNJMHNEK"))


int (*orig_PlayerNetwork_TakeDamage)(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID);
int hook_PlayerNetwork_TakeDamage(void *ClosestEnemy, int baseDamage, PlayerID damager, DamageInfo2_o *damageInfo, int weaponDataID, Vector3 firePos, Vector3 hitPos, monoList<float> *checkParams, void *damagerWeaponDynamicInfo, int damagerVehicleID) {
    
    void* CurrentMatch = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(CurrentMatch);
    
    if (LocalPlayer && ClosestEnemy) {
        firePos = GetHeadPosition(LocalPlayer);
        hitPos = GetHeadPosition(ClosestEnemy);
    }
    if (ClosestEnemy != NULL && Headshot) {
        if (damageInfo != NULL) {
            *(int *)((long) damageInfo + offset_BodyPart) = 1;
        }
    }
    return orig_PlayerNetwork_TakeDamage(ClosestEnemy, baseDamage, damager, damageInfo, weaponDataID, firePos, hitPos, checkParams, damagerWeaponDynamicInfo, damagerVehicleID);
}

int (*old_KOODEKEKFEK1)(void *, HitObjectInfo *);
int KOODEKEKFEK1(void *ist, HitObjectInfo *HitObject) {
    if ((SilentAimv222 || Autofire) && HitObject != nullptr) {
        void *current_match = Curent_Match();
        if (current_match != NULL) {
            void* local_player = GetLocalPlayer(current_match);
            if (local_player != NULL) {
                void* WeaponHand = GetWeaponOnHand1(local_player);
                auto *ClosestEnemy = EnemySlientAim();
                
                if (ClosestEnemy != nullptr && isEnemyInRangeWeapon(local_player, ClosestEnemy, WeaponHand)) {
                    Vector3 EnemyLocation = (aimPosition == 0) ? GetHeadPosition(ClosestEnemy) : GetHipPosition(ClosestEnemy);
                    Vector3 PlayerLocation = CameraMain(local_player);
                    HitObject->HitObject = get_gameObject(Player_GetHeadCollider(ClosestEnemy));
                    HitObject->HitCollider = Player_GetHeadCollider(ClosestEnemy);
                    
                    HitObject->HitLocation = EnemyLocation;
                    HitObject->HitNormal = EnemyLocation;
                    HitObject->RayDir = Vector3::Normalized(EnemyLocation - PlayerLocation);
                    HitObject->StartPosition = PlayerLocation;
                    HitObject->OrigStartPosition = PlayerLocation;
                    HitObject->Damage = SilentDamage;
                    HitObject->SpecialHitType = 0;
                    HitObject->HitGroup = 1;
                    HitObject->IgnoreHappens = false;
                    HitObject->ViewBlocked = false;
                }
            }
        }
    }
    return old_KOODEKEKFEK1(ist, HitObject);
}

bool tapAutoFire = false;
bool AutofireV2 = false;
bool (*old_IsWeaponInAutoCharge)(void *_this);
bool IsWeaponInAutoCharge_Hook(void *_this) {
    if (_this != nullptr && AutofireV2) {
        void *LocalPlayer = GetLocalPlayer(Curent_Match());
        if (LocalPlayer != nullptr) {
            void* CurrentMatch = Curent_Match();
 void* ClosestEnemy = GetClosestEnemy();
        if (ClosestEnemy != nullptr) {
             void *Weapon = GetWeaponOnHand1(LocalPlayer);
                if (Weapon != nullptr) {
                    if (isEnemyInRangeWeapon(LocalPlayer, ClosestEnemy, Weapon)) {
                     if (IsVisible(ClosestEnemy)) {
                            if (tapAutoFire) {
                                static bool fireToggle = false;
                                static auto lastFireTick =
                                    std::chrono::steady_clock::now();
                                auto currentTick =
                                    std::chrono::steady_clock::now();
                                auto diff =
                                    std::chrono::duration_cast
                                    <std::chrono::milliseconds>(
                                        currentTick - lastFireTick
                                    ).count();
                                if (diff > 35) {
                                    fireToggle = !fireToggle;
                                    lastFireTick = currentTick;
                                }
                                return fireToggle;
                            }
                            return true;
                        }
                    }
                }
            }
        }
    }

    return old_IsWeaponInAutoCharge(_this);
}

bool Startgs = true;
bool AimVisible = true;

void* EnemyVisible(void* match) {
    if (!match || !Startgs) return nullptr;

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
        if (ignoreEnemyBot && get_IsClientBot(Player)) continue;

        if (!get_isLocalTeam(Player)
            && (!ignoreKnocked || !get_IsDieing(Player))
            && get_isVisible(Player)
            && get_MaxHP(Player) > 0) {

            Vector3 PlayerPos = GetAdjustedPosition(Player);
            Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
            Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);

            float angle = Vector3::Angle(
                targetDir,
                GetForward(Component_GetTransform(Camera_main()))
            ) * 100.0f;

            if (!get_God(Player)) {
                if (AimVisible) {
                    if (Visible_Check(Player) &&
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
bool CoverKill = false;
static int currentStep = 0;
static float coverKillTimer = 0.0f;

void CoverKillVoid(float deltaTime)
{
    if (!Enable || !CoverKill)
        return;

    void* match = Curent_Match();
    if (!match)
        return;
    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer)
        return;

    MyDictionary* players = *(MyDictionary**)((uintptr_t)match + ListPlayer);  
    if (!players || !players->entries) 
        return;  


    coverKillTimer += deltaTime;
    if (coverKillTimer >= 0.18f)
    {
        currentStep = (currentStep + 1) % 4;
        coverKillTimer = 0.0f;
    }

    for (int i = 0; i < players->count && i < 64; i++) 
    {
        void* enemy = players->entries->vector[i].value;  
        if (!enemy || enemy == localPlayer) 
            continue; 

        if (get_isLocalTeam(enemy))
            continue;

        if (get_MaxHP(enemy) <= 0.1f)
            continue;

        if (get_IsDieing(enemy))
            continue;

        if (!IsInDistance(localPlayer, enemy))
            continue;

        void* enemyTransform = Component_GetTransform(enemy);
        if (!enemyTransform)
            continue;


        Vector3 basePos = GetPosition(enemyTransform);
        Vector3 targetPos = basePos;


        if (currentStep == 0) {
            targetPos.x += 2.0f;
            targetPos.z += 2.0f;
        }
        else if (currentStep == 1) {
            targetPos.x -= 2.0f;
            targetPos.z += 2.0f;
        }
        else if (currentStep == 2) {
            targetPos.x -= 2.0f;
            targetPos.z -= 2.0f;
        }
        else if (currentStep == 3) {
            targetPos.x += 2.0f;
            targetPos.z -= 2.0f;
        }


        SetPosition(enemyTransform, targetPos); 
    }
}	

static void ShowPaymentCreditDialog(monoString* message, monoString* title, int buttonStyle = 2) {
    if (!offset_ShowPaymentMessageBoxNotUseSpecialDepth) return;
    void (*_Show)(monoString*, monoString*, monoString*, void*, void*, monoString*, int) =
        (void (*)(monoString*, monoString*, monoString*, void*, void*, monoString*, int))(offset_ShowPaymentMessageBoxNotUseSpecialDepth);
    _Show(message, title, nullptr, nullptr, nullptr, nullptr, buttonStyle);
}

static void ShowCheatOussamaDialog2(monoString* message, monoString* title) {
    ShowPaymentCreditDialog(message, title, 2);
}


static void ShowCreditDialog1() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Owner - @ur_nikuu[00ff00]NIKUMODS\n[ffffff]Enhance your gaming experience with our updated mod.\n")),
        U3DStr(OBFUSCATE("INFORMATION")), 2);
}
static void TryShowEnableAllDialogOnce(bool enabled) {
    static bool dialogShown = false;
    if (!enabled || dialogShown) return;
    if (!GetUIInGameScene()) return;

    ShowCreditDialog1();
    dialogShown = true;
}
static void AddTeammateHud(monoString* nick, monoString* info) {
    void* off = Il2CppGetMethodOffset(
        OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"),
        OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowAssistantText"), 2);
    if (!off) return;
    void (*_Add)(void*, monoString*, monoString*) = (void(*)(void*, monoString*, monoString*))(off);
    void* ui = GetUIInGameScene();
    if (ui) _Add(ui, nick, info);
}

static void ShowCreditDialog2() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Official Channel:\n[00ff00]t.me/ur_nikuu\n[ffffff]Join us for updates & support.")),
        U3DStr(OBFUSCATE("TELEGRAM")), 2);
}

static void ShowCreditDialog3() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Developed By: [00ff00]@ur_nikuu\n[ffffff]Copyright @2026\n[ffffff]All Rights Reserved.")),
        U3DStr(OBFUSCATE("DEVELOPER")), 1);
}

static void ShowCreditDialog4() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Version: [00ff00]FreeFire\n[ffffff]Mod Status: [00ff00]Online\n[ffffff]Build: Max Update")),
        U3DStr(OBFUSCATE("VERSION")), 2);
}

static void ShowCreditDialog5() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Features:\n[00ff00]- Aimkill\n[00ff00]- @ur_nikuu\n[00ff00]- Fly Hack\n[00ff00]- ESP")),
        U3DStr(OBFUSCATE("FEATURES")), 2);
}

static void ShowCreditDialog6() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]New Update Available!\n[00ff00]@ur_nikuu\n[ffffff]Check Telegram for latest build.")),
        U3DStr(OBFUSCATE("UPDATE")), 2);
}

static void ShowCreditDialog7() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Premium Mod Menu\n[00ff00]Best Performance\n[ffffff]Optimized for all device")),
        U3DStr(OBFUSCATE("PREMIUM")), 1);
}

static void ShowCreditDialog8() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffff00]Warning:\n[ffffff]Use at your own risk.\n[ffffff]Play smart, stay safe.")),
        U3DStr(OBFUSCATE("WARNING")), 2);
}

static void ShowCreditDialog9() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Thank you for using our mod!\n[00ff00]Support: t.me/ur_nikuu\n[ffffff]Share with friends.")),
        U3DStr(OBFUSCATE("THANK YOU")), 3);
}

static void ShowCreditDialog10() {
    ShowPaymentCreditDialog(
        U3DStr(OBFUSCATE("[ff0000]NIKUMODS\n[ffffff]Powered By @ur_nikuu\n[00ff00]Community\n[ffffff]Stay tuned for more!")),
        U3DStr(OBFUSCATE("COMMUNITY")), 2);
}

static void ShowCreditDialogByIndex(int index) {
    switch (index % 10) {
        case 0: ShowCreditDialog1(); break;
        case 1: ShowCreditDialog2(); break;
        case 2: ShowCreditDialog3(); break;
        case 3: ShowCreditDialog4(); break;
        case 4: ShowCreditDialog5(); break;
        case 5: ShowCreditDialog6(); break;
        case 6: ShowCreditDialog7(); break;
        case 7: ShowCreditDialog8(); break;
        case 8: ShowCreditDialog9(); break;
        default: ShowCreditDialog10(); break;
    }
}

static void RxmGetNickName(void* targetEnemy) {
    if (!targetEnemy) return;
    if (get_IsDieing(targetEnemy)) return;
    if (GetHp(targetEnemy) <= 0) return;

    void* match = Curent_Match();
    if (!match) return;
    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer) return;

    void* camera = Camera_main();
    if (!camera) return;
    void* camTF = Component_GetTransform(camera);
    if (!camTF) return;

    Vector3 camPos = getPosition(camTF);
    Vector3 enemyHeadPos = GetHeadPosition(targetEnemy);
    if (enemyHeadPos.x == 0 && enemyHeadPos.y == 0 && enemyHeadPos.z == 0) return;

    float distance = sqrtf(
        (camPos.x - enemyHeadPos.x) * (camPos.x - enemyHeadPos.x) +
        (camPos.y - enemyHeadPos.y) * (camPos.y - enemyHeadPos.y) +
        (camPos.z - enemyHeadPos.z) * (camPos.z - enemyHeadPos.z)
    );

    monoString* nick = get_NickName(targetEnemy);
    if (!nick) return;
    int enemyHp = GetHp(targetEnemy);
    monoString* distances = U3DStrFormat(distance, enemyHp);
    if (!distances) return;
    AddTeammateHud(nick, distances);
}
void* g_ClosestEnemy = nullptr;

bool autoglid      = false;
bool glidertp      = false;
bool infiniteFlyer = false;
bool gliderbac     = false;
bool gliderhold    = false;
bool flyMove       = false;
bool sequenceTP    = false;
int  gliderHeight  = 80;

// internal helpers (not exposed in menu)
bool autojump       = false;
bool flyJump        = false;
bool forceGlideTP   = false;

#define Offset_StartSAPJumpBeforeGlid (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("StartSAPJumpBeforeGlid"), 0)
#define Offset_StartInSAPFlyJumping   Offset_StartSAPJumpBeforeGlid
#define Offset_Startgliderbac         (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("StartSAPGlid"), 0)
static void* getRealOffset(uintptr_t offset) {
    extern uintptr_t il2cpp_base;
    if (offset < 0x10000000)
        return reinterpret_cast<void*>(il2cpp_base + offset);
    return reinterpret_cast<void*>(offset);
}

// --- HELPER 1: OPEN GLIDER INSTANTLY (gliderbac) ---


void hilll_gliderbac(void* localPlayer) {
    if (!localPlayer) return;
    uintptr_t fnOffset = Offset_Startgliderbac;
    if (!fnOffset) return;
    typedef void (*gliderbac_t)(void*);
    gliderbac_t fn = (gliderbac_t)getRealOffset(fnOffset);
    if (fn) fn(localPlayer);
}

void TriggerInfiniteGlide(void* localPlayer) {
    if (!localPlayer) return;
    uintptr_t fnOffset = Offset_StartSAPJumpBeforeGlid;
    if (!fnOffset) return;
    typedef void (*StartSAPJump_t)(void*);
    StartSAPJump_t fn = (StartSAPJump_t)getRealOffset(fnOffset);
    if (fn) fn(localPlayer);
}

void ApplyAutoGlider(void* localPlayer) {
    if (!localPlayer || !autoglid) return;
    TriggerInfiniteGlide(localPlayer);
}

void ApplyInfiniteFlyer(void* localPlayer) {
    if (!localPlayer || !infiniteFlyer) return;
    TriggerInfiniteGlide(localPlayer);
    hilll_gliderbac(localPlayer);
}

void ApplyFlyMove(void* localPlayer) {
    static bool wasLaunched = false;
    static std::chrono::steady_clock::time_point flyStartTime{};

    if (!localPlayer || get_IsDieing(localPlayer) || !flyMove) {
        wasLaunched = false;
        flyStartTime = {};
        return;
    }

    void* transform = Component_GetTransform(localPlayer);
    if (!transform) return;

    Vector3 pos = GetPosition(transform);

    auto now = std::chrono::steady_clock::now();

    if (!wasLaunched) {
        TriggerInfiniteGlide(localPlayer);
        wasLaunched = true;
        flyStartTime = now;
    }

    Vector3 forwardDir = {0, 0, 0};
    void* mainCam = get_main();
    if (mainCam) {
        void* camTf = Component_GetTransform(mainCam);
        if (camTf) forwardDir = GetForward(camTf);
    }
    if (forwardDir.x == 0 && forwardDir.y == 0 && forwardDir.z == 0) {
        forwardDir = GetForward(transform);
    }

    float speed = 0.6f;
    pos.x += forwardDir.x * speed;
    pos.y += forwardDir.y * speed;
    pos.z += forwardDir.z * speed;

    SetPosition(transform, pos);

    if (flyStartTime.time_since_epoch().count() != 0) {
        long long elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - flyStartTime).count();
        if (elapsedMs >= 3000) {
            TriggerInfiniteGlide(localPlayer);
            hilll_gliderbac(localPlayer);
            flyStartTime = now;
        }
    }
}

// --- FEATURE 4: GLIDER HOLD (WITH GLIDER HEIGHT) ---
void ApplyGliderHold(void* localPlayer) {
    static int gliderHoldStep = 0;
    static std::chrono::steady_clock::time_point step1StartTime{};

    if (!localPlayer || get_IsDieing(localPlayer) || !gliderhold) {
        if (gliderHoldStep == 1) autojump = false;
        gliderHoldStep = 0;
        return;
    }

    auto now = std::chrono::steady_clock::now();

    if (gliderHoldStep == 0) {
        autojump = true;
        step1StartTime = now;
        gliderHoldStep = 1;
    } else if (gliderHoldStep == 1) {
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - step1StartTime).count();
        if (ms >= 300) {
            autojump = false;

            void* transform = Component_GetTransform(localPlayer);
            if (transform) {
                Vector3 pos = GetPosition(transform);
                int targetFeet = gliderHeight;
                if (targetFeet < 1)   targetFeet = 1;
                if (targetFeet > 100) targetFeet = 100;
                float meters = targetFeet * 0.3048f;

                pos.y += meters;
                SetPosition(transform, pos);

                TriggerInfiniteGlide(localPlayer);
                hilll_gliderbac(localPlayer);
            }
            gliderHoldStep = 2;
        }
    }
    // step 2 = done, stays until feature toggled off
}

bool AutoSwap1   = false;
float SwapDelay = 150;
bool AutoChangeWeaponbet = false;
bool isSwapWeapon = false;
bool sID = false; 
static clock_t lastSwapTime = 0;
static int SwapDelayMs = 1;
static int SwapDelayMs2 = 2;
int tS = 1000;

#define SwapWeapon11234555 Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SwapWeapon"), 2)

static void SwapWeapon111200(void* player, int POFFNNMOOBM, bool GDKLMFLNNGM) {
    void (*_SwapWeapon111200)(void* player, int POFFNNMOOBM, bool GDKLMFLNNGM) = (void (*)(void *, int, bool))(SwapWeapon11234555);
     _SwapWeapon111200(player, POFFNNMOOBM, GDKLMFLNNGM);
}    

Vector3 SavedMapMarker = { 0, 0, 0 };
bool HasMapMarker = false;
bool TeleportMark1 = false;

void (*orig_UpdateLocalMark)(void* _this, Vector3 p, uint32_t posType, Vector3 realPos, int bigMapType);
void hook_UpdateLocalMark(void* _this, Vector3 p, uint32_t posType, Vector3 realPos, int bigMapType) {
    
    SavedMapMarker = realPos;
    HasMapMarker = true;

    if (orig_UpdateLocalMark) {
        orig_UpdateLocalMark(_this, p, posType, realPos, bigMapType);
    }
}

static auto get_position11(void *player) {
    auto (*_get_position11)(void *player) = (Vector3 (*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
    return _get_position11(player);
}
static auto set_position_Injected(void *player, Vvector3 position) {
    auto (*_set_position_Injected)(void *player, Vvector3 position) = (void (*)(void *, Vvector3))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1);
    return _set_position_Injected(player, position);
}
static auto set_position_Injected113(void *player, Vvector3 position) {
    auto (*_set_position_Injected113)(void *player, Vvector3 position) = (void (*)(void *, Vvector3))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1);
    return _set_position_Injected113(player, position);
}
static void set_position_Injected114(void* transform, Vector3 position) {
    auto _set_position_Injected114 =(void (*)(void*, Vector3))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1);
    _set_position_Injected114(transform, position);
}
static auto get_transform(void *player) {
    auto (*_get_transform)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0);
    return _get_transform(player);
}
void TeleportMarkVoid(void* localPlayer) {
    if (TeleportMark1 && HasMapMarker && localPlayer != nullptr) {        
        void* tf = get_transform(localPlayer);
        if (!tf) return;
        Vector3 currentPos = get_position11(tf);
        Vector3 targetPos;
        targetPos.x = SavedMapMarker.x;
        targetPos.y = currentPos.y; 
        targetPos.z = SavedMapMarker.z;
        set_position_Injected(tf, Vvector3(targetPos.x, targetPos.y, targetPos.z));
        TeleportMark1 = false;
    }
}

void (*LateUpdate)(void *Player, float gameTime, float deltaTime);
void _LateUpdate(void *Player, float gameTime, float deltaTime) {
    if (Player != NULL) {
		CoverKillVoid(deltaTime);
		void* CurrentMatch = Curent_Match();
void* closestEnemy = GetClosestEnemy();
void* closestEnemyVisible = EnemyVisible(CurrentMatch);
void* LocalPlayer = GetLocalPlayer(CurrentMatch);
ApplyAutoGlider(LocalPlayer);
    ApplyInfiniteFlyer(LocalPlayer);
    ApplyFlyMove(LocalPlayer);
    ApplyGliderHold(LocalPlayer);
    if (gliderbac) hilll_gliderbac(LocalPlayer);
AimbotLegitVoid();
UpdateSpeedhack();
TryShowEnableAllDialogOnce(Enable);
TeleportMarkVoid(LocalPlayer);
if (Autofire) autofireupdate(Player);
		
		if (Aimbot) {
void* CurrentMatch = Curent_Match();
void* closestEnemy = GetClosestEnemy();
void* LocalPlayer = GetLocalPlayer(CurrentMatch);

if (closestEnemy != NULL && LocalPlayer != NULL && CurrentMatch != NULL) {
    Vector3 EnemyLocation = GetHeadPosition(closestEnemy);
    Vector3 PlayerLocation = CameraMain(LocalPlayer);
    Quaternion PlayerLook = GetRotationToLocation(EnemyLocation, 0.1f, PlayerLocation);
    bool IsScopeOn = get_IsSighting(LocalPlayer);
    bool IsFiring = get_IsFiring(LocalPlayer);
  
    if (AimWhen == 0) {
        set_aim(LocalPlayer, PlayerLook);
    }
    else if (AimWhen == 1 && IsFiring) {
        set_aim(LocalPlayer, PlayerLook);
    }
    else if (AimWhen == 2 && IsScopeOn) {
        set_aim(LocalPlayer, PlayerLook);
    }
}
}

static auto lastSwapTime = std::chrono::steady_clock::now();
    static int currentWeaponIndex = 0;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSwapTime).count();
          
    if (AutoSwap1 && elapsed >= SwapDelay) {
        currentWeaponIndex = (currentWeaponIndex + 1) % 3;
        SwapWeapon111200(LocalPlayer, currentWeaponIndex, true);
        lastSwapTime = currentTime;
    }
	
static float lastCreditTime = 0.0f;
        static float lastPopupTime = 0.0f;
        static float lastNickTime = 0.0f;
        static bool tipsShown = false;

        void* match = Curent_Match();
        if (match) {
            float now = get_realtimeSinceStartup();

            void* ui = GetUIInGameScene();
            if (ui) {
                if (!tipsShown) {
                    ShowCenterUpTeammateTips(U3DStr(OBFUSCATE("Copyright NIKUMODS | Developed @ur_nikuu")), 99999.0f);
                    tipsShown = true;
                }

                if (now - lastCreditTime >= 3.0f) {
                    ShowCreditPopup(U3DStr(OBFUSCATE("NIKUMODS | t.me/ur_nikuu")));
                    lastCreditTime = now;
                }

                if (now - lastPopupTime >= 10.0f) {
                    ShowPopupMessageNoAnim(U3DStr(OBFUSCATE("[ff0000] NIKUMODS | [ffffff] Owner - @ur_nikuu")), 1.5f);
                    lastPopupTime = now;
                }
            }

            if (now - lastNickTime >= 2.0f) {
                void* closest = GetClosestEnemy();
                if (closest) {
                    RxmGetNickName(closest);
                }
                lastNickTime = now;
            }
        }
   

		    }
    LateUpdate(Player, gameTime, deltaTime);
}

void dobbyhooks() {
	DobbyHook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("LHNJLFOINDO"), 1), (void *) KOODEKEKFEK1,(void **) &old_KOODEKEKFEK1);
}
bool EnemyCount = false;
static int g_CachedEnemyCount = 0;
static float g_LastUpdateTime = 0.0f;

inline void DrawEnemiesCount(float screenWidth)
{
    if (!EnemyCount)
        return;

    float time = ImGui::GetTime();

    if (time - g_LastUpdateTime >= 0.2f)
    {
        g_LastUpdateTime = time;

        void* match = Curent_Match();
        if (!match)
            return;

        void* localPlayer = GetLocalPlayer(match);
        if (!localPlayer)
            return;

        MyDictionary* players = *(MyDictionary**)((uintptr_t)match + ListPlayer);
        if (!players || !players->entries)
            return;

        int count = players->count;
        if (count > 100) count = 100;

        int enemyCount = 0;

        for (int i = 0; i < count; i++)
        {
            void* enemy = players->entries->vector[i].value;

            if (!enemy || enemy == localPlayer)
                continue;

            if (get_isLocalTeam(enemy))
                continue;

            if (get_MaxHP(enemy) <= 0.1f)
                continue;

            if (get_IsDieing(enemy))
                continue;

            if (!IsInDistance(localPlayer, enemy))
                continue;

            enemyCount++;
        }

        g_CachedEnemyCount = enemyCount;
    }

    std::string text = "Enemies: " + std::to_string(g_CachedEnemyCount);

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    draw->AddText(
    ImGui::GetFont(),
    41.0f,
    ImVec2(screenWidth / 2 - 60, 80),
    g_CachedEnemyCount == 0
        ? IM_COL32(0, 255, 0, 255)
        : IM_COL32(255, 0, 0, 255),
    text.c_str()
);
}
bool GameTimer = false;
static std::chrono::steady_clock::time_point timerStartTime;
static bool wasTimerRunning = false;
static float elapsedTimerSeconds = 0.0f;
static bool timerAutoOffTriggered = false; // New flag for auto-off

inline void DrawGameTimer(ImDrawList* drawList, float screenWidth, float screenHeight) {
    if (!GameTimer || !drawList) return;

    // تشغيل وإيقاف العداد
    if (Enable) {
        if (!wasTimerRunning) {
            timerStartTime = std::chrono::steady_clock::now();
            wasTimerRunning = true;
            timerAutoOffTriggered = false; // Reset auto-off flag
        }

        auto now = std::chrono::steady_clock::now();
        elapsedTimerSeconds =
            std::chrono::duration<float>(now - timerStartTime).count();

        // Auto-off after 3 minutes (180 seconds)
        if (elapsedTimerSeconds >= 181.0f && !timerAutoOffTriggered) {
            GameTimer = false; // Turn off timer automatically
            timerAutoOffTriggered = true; // Prevent multiple triggers
            elapsedTimerSeconds = 0.0f; // Reset time
            wasTimerRunning = false;
            
            // Optional: You can add a notification or sound here
            // Example: ShowMessage("Timer finished!");
        }
    } else {
        // Only reset if it wasn't auto-off triggered
        if (!timerAutoOffTriggered) {
            elapsedTimerSeconds = 0.0f;
            wasTimerRunning = false;
        } else {
            // Keep elapsed time at 0 when auto-off
            elapsedTimerSeconds = 0.0f;
            wasTimerRunning = false;
        }
    }

    // تحديد لون الوقت
    float elapsedMinutes = elapsedTimerSeconds / 60.0f;
    ImColor valueColor;

    if (!Enable) {
        valueColor = ImColor(0, 255, 0, 255);
    }
    else if (elapsedMinutes < 1.0f) {
        valueColor = ImColor(255, 0, 0, 255);
    }
    else if (elapsedMinutes < 2.0f) {
        valueColor = ImColor(255, 140, 0, 255);
    }
    else if (elapsedMinutes < 3.0f) {
        valueColor = ImColor(255, 255, 0, 255);
    }
    else {
        valueColor = ImColor(0, 255, 0, 255);
    }

    // مكان العداد (تم رفعه للأعلى)
    ImVec2 rectPos(30.0f, 30.0f);
    ImVec2 rectSize(160.0f, 35.0f);
    ImVec2 rectMax(rectPos.x + rectSize.x, rectPos.y + rectSize.y);

    float rounding = 6.0f;

    drawList->AddRect(
        rectPos,
        rectMax,
        ImColor(0, 255, 0, 255),
        rounding,
        0,
        1.5f
    );

    // نص الوقت
    int totalSeconds = static_cast<int>(elapsedTimerSeconds);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    char timerBuffer[32];
    snprintf(timerBuffer, sizeof(timerBuffer), "TIMER : %d:%02d", minutes, seconds);

    std::string labelText = "TIMER : ";
    std::string timeText = std::string(timerBuffer).substr(labelText.length());

    float textHeight = ImGui::GetTextLineHeight();
    ImVec2 textPos(
        rectPos.x + 12.0f,
        rectPos.y + (rectSize.y - textHeight) * 0.5f
    );

    float labelWidth = ImGui::CalcTextSize(labelText.c_str()).x;

    // ===== ظل النص =====

    // ظل TIMER :
    drawList->AddText(
        ImVec2(textPos.x + 2.0f, textPos.y + 2.0f),
        IM_COL32(0, 0, 0, 200),
        labelText.c_str()
    );

    // ظل الوقت
    drawList->AddText(
        ImVec2(textPos.x + labelWidth + 2.0f, textPos.y + 2.0f),
        IM_COL32(0, 0, 0, 200),
        timeText.c_str()
    );

    // ===== النص الحقيقي =====

    drawList->AddText(
        textPos,
        ImColor(255, 255, 255, 255),
        labelText.c_str()
    );

    drawList->AddText(
        ImVec2(textPos.x + labelWidth, textPos.y),
        valueColor,
        timeText.c_str()
    );
}

inline void DrawESP(float screenWidth, float screenHeight) {
 ImDrawList*draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;
   
    
if (Enable) {
	DrawEnemiesCount(screenWidth);
	DrawGameTimer(draw, screenWidth, screenHeight);
    void* current_Match = Curent_Match();
    void* local_player = GetLocalPlayer(current_Match);
         
    if (local_player && current_Match) {
        MyDictionary* players = *(MyDictionary**)((uintptr_t)current_Match + ListPlayer);
        void* camera = Camera_main();

        if (players && players->entries && camera) {
            int total = players->count;
            for (int u = 0; u < total; u++) {
                void* closestEnemy = players->entries->vector[u].value;

                if (closestEnemy && closestEnemy != local_player && get_isVisible(closestEnemy) && !get_isLocalTeam(closestEnemy)) {
                    Vector3 Toepos = getPosition(closestEnemy);
                    Vector3 Toeposi = WorldToScreenPoint(camera, Toepos);
                    if (Toeposi.z < 1) continue;
						
                    Vector3 HeadPos = Toepos + Vector3(0, 1.9f, 0);
                    Vector3 HeadPosition = WorldToScreenPoint(camera, HeadPos);
                    if (HeadPosition.z < 1) continue;

                     float distance = Vector3::Distance(getPosition(local_player), Toepos);                     				 					 
					 float Height = abs(HeadPosition.y - Toeposi.y) * 1.09f;
                     float Width = Height * 0.55f;                                                    
				     Rect rect = Rect(HeadPosition.x - Width / 2.f, screenHeight - HeadPosition.y, Width, Height);																																			
					
if (Config.ESP.Health) {           
             long clr = ImColor(0,255,0,255);               
                                
              if (GetHp(closestEnemy) <= (get_MaxHP(closestEnemy) * 0.6)) {
                clr = ImColor(255, 255, 0, 255);
          }
              
              if (GetHp(closestEnemy) < (get_MaxHP(closestEnemy) * 0.3)) {
                clr = ImColor(255, 0, 0, 255);
        }
             int xx = rect.x + rect.w + 2;
             int yy = rect.y;
            draw->AddRectFilled(ImVec2(xx, yy), ImVec2(xx + 5, yy + rect.h), ImColor(0, 0, 0, 255));        
            draw->AddRectFilled(ImVec2(xx + 1, yy + rect.h - (rect.h * ((float)GetHp(closestEnemy) / get_MaxHP(closestEnemy)))), ImVec2(xx + 4, yy + rect.h),clr);                                        
        }
        
        
        if (Config.ESP.Line) {
    // Top Center position (Screen ke bilkul upar beech mein)
    ImVec2 startPos = ImVec2(screenWidth / 2, 0); 

    // Enemy ke head position tak line
    ImVec2 endPos = ImVec2(rect.x + rect.w / 2, rect.y + rect.h / 35);

    // Line ki thickness (Patla karne ke liye 1.0f ya 0.8f use karein)
    float lineThickness = 1.0f; 

    if (get_IsDieing(closestEnemy)) {
        // Agar enemy mar raha hai toh Red line (Thoda patla)
        draw->AddLine(startPos, endPos, ImColor(255, 255, 255), lineThickness);
    } else {
        // Alive enemy ke liye Pure White color
        ImColor whiteColor = ImColor(255, 255, 255); // R=255, G=255, B=255 (White)
        draw->AddLine(startPos, endPos, whiteColor, lineThickness);
    }
}

        
       if (Config.ESP.Box) {
    int x = rect.x;
    int y = rect.y;
    int _width = rect.w;
    int _height = rect.h;

    // --- Static White Field (Sirf White Color) ---
    // 0.25f ka matlab hai thoda transparent white
    ImColor fieldColorWhite = ImColor(255, 255, 255, 60); // (R, G, B, Alpha)

    if (get_IsDieing(closestEnemy)) {
        // --- DYING ENEMY (Solid Red) ---
        draw->AddRect(ImVec2(x, y), ImVec2(x + _width, y + _height), ImColor(255, 0, 0), visual_esp_box, 0, visual_esp_boxth);
    } else {
        // --- ALIVE ENEMY (White Field + Cyan Border) ---
        
        // 1. Andar ka color (White with Transparency)
        draw->AddRectFilled(
            ImVec2(x, y), 
            ImVec2(x + _width, y + _height), 
            fieldColorWhite, 
            visual_esp_box
        );

        // 2. Border (Solid Cyan)
        draw->AddRect(
            ImVec2(x, y),
            ImVec2(x + _width, y + _height),
            ImColor(0, 255, 255), 
            visual_esp_box,
            0,
            visual_esp_boxth
        );
    }
}


if (Config.ESP.RamaMods) {
    monoString* Nick = get_NickName(closestEnemy);
    std::string name;
    if (Nick != nullptr) {
        int len = Nick->getLength();
        name.reserve(len);
        for (int i = 0; i < len; i++) {
            char c = get_Chars(Nick, i);
            name += (isascii(c) && isprint(c)) ? c : '?';
        }
    }
    
    float textScale = std::clamp(1.0f - (distance / 1000.0f), 0.6f, 1.2f);
    float textX = rect.x + rect.w * 0.5f;
    float topY = rect.y - 25.0f;
    float bottomY = rect.y + rect.h + 15.0f;
    
    ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());
    draw->AddText(nullptr, 14.0f * textScale, ImVec2(textX - nameSize.x * 0.5f, topY), ImColor(255, 255, 255), name.c_str());
    
    char distText[32];
    snprintf(distText, sizeof(distText), "%.1fm", distance);
    ImVec2 distSize = ImGui::CalcTextSize(distText);
    draw->AddText(nullptr, 20.0f * textScale, ImVec2(textX - distSize.x * 0.5f, bottomY), ImColor(0, 255, 0), distText);
}}}}}}
    
}

bool Guest;
bool (*ResetGuest)(void* _this);
bool _ResetGuest(void* _this) {
if (Guest) {return true;}
return ResetGuest(_this);}
bool camxa;
float (*GetCameraHeightRateValue)(void *player);
float _GetCameraHeightRateValue(void *player) {
if (player != nullptr) {
if (camxa) {return 4.0f;}}
return GetCameraHeightRateValue(player);}
bool(*Bypass)(void* bypass); 
bool _Bypass(void* bypass) {return false;}

bool(*get_ResetGuest)(bool* instance);
bool _get_ResetGuest(bool* instance) {
    if (Convidado) {
        return true;
    }
    return get_ResetGuest(instance);
}

float (*old_GetCurrentDashSpeed)(void *instance);
float hook_GetCurrentDashSpeed(void *instance) {
    void* localPlayer = GetLocalPlayer(Curent_Match());
    if (instance != nullptr && instance == localPlayer) {
        if (SpeedHack) {
            //10f Is Limit BY SYNTHAX (Gay)
            return 9.0; 
        }
    }
    return old_GetCurrentDashSpeed(instance);
}
bool FastSwitch = false;
bool (*get_InSwapWeaponCD)(void* thiz);
bool _get_InSwapWeaponCD(void* thiz) {
    if (FastSwitch) {
        return false;  // bypass swap cooldown
    }
    return get_InSwapWeaponCD(thiz);
}

#define offset_Component_GetTransform (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
static void *Component_get_transform(void *player)
{
    void *(*_Component_GetTransform)(void *nullo) = (void *(*)(void *))offset_Component_GetTransform;
    return _Component_GetTransform(player);
}

#define offset_TransformNode (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TransformNode"), OBFUSCATE("get_transform"), 0)
void* TransformNode(void *_this)
{
    return ((void* (*)(void *))offset_TransformNode)(_this);
}
#define offset_get_IsDieing (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)
static bool IsDieing(void *player) {
    if (!player) return false;
    bool (*_IsDieing)(void *players) = (bool (*)(void *))offset_get_IsDieing;
    return _IsDieing ? _IsDieing(player) : false;
}
#define offset_LocalPlayer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentLocalPlayer"), 0)
static void *Current_Local_Player() {
    void *(*_Local_Player)(void *players) = (void *(*)(void *))(offset_LocalPlayer);
    return _Local_Player(NULL);
}
#define offset_HeadTF (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("OLCJOGDHJJJ"))


#define offset_Transform_INTERNAL_GetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
static Vector3 Transform_INTERNAL_GetPosition(void *player)
{
    Vector3 out = Vector3::zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))offset_Transform_INTERNAL_GetPosition;
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}
static Vector3 CameraPosition(void *player)
{
    return Transform_INTERNAL_GetPosition(Component_get_transform(get_main()));
}
bool isVisible_Aimbot(void * player)
{
    if (!IsPullTargetValid(player)) return false;

    void *hitObj = NULL;
    void *HeadTF = GetPlayerHeadTransform(player);
    if (!HeadTF) return false;

    void *local = Current_Local_Player();
    if (!local) return false;

    Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
    Vector3 LocalPlayerPos = CameraPosition(local);
    return !Physics_Raycast(LocalPlayerPos, EnemyHeadPosition, 12, &hitObj);
}

#define offset_get_HeadCollider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"), 0)
void *get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))offset_get_HeadCollider)(pthis);
}
//END FOR PC AIMSILENT


bool DiscordRajaXModsFireAimkill = false;

inline uintptr_t HeadTF()      { return Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("PEMOFNFCLFB")); }
bool HideDamage;

bool (*orig_get_ShowDamageNum)(void* thiz);
bool hook_get_ShowDamageNum(void* thiz) {
    if (HideDamage) {
        return false;
    }
    return orig_get_ShowDamageNum(thiz);
}

bool SpeedWalking = false;

bool (*IsFoldWingGliding)(void *player);
bool _IsFoldWingGliding(void *player) {
    void* CurrentMatch = Curent_Match();
    if (CurrentMatch != nullptr) {
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (LocalPlayer != nullptr) {
            if (player != NULL) {
                if (SpeedWalking) {
                    return true;
                }
            }
        }
    }
    return IsFoldWingGliding(player);
}

static int (*MBOHNCMOJDE)(...);
static int _MBOHNCMOJDE(void* CBCAJJJCKLH) {
    void* CurrentMatch = Curent_Match();
    if (CurrentMatch != nullptr) {
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);
        if (LocalPlayer != nullptr) {
            if(SpeedWalking) {
                return 1;
            }
        }
    }
    return MBOHNCMOJDE(CBCAJJJCKLH);
}

Vector3 SetMap = Vector3(0, 0, 0);
int HighVp = 0;
bool ghostmapcs = false;


int (*orig_BEV_Jump_onExecute)(int Player, int a);
int hook_BEV_Jump_onExecute(int Player, int a) {
  if (Player && HighVp < 10) {
    HighVp += 2;
  }
  return orig_BEV_Jump_onExecute(Player,a);
}
int (*orig_isground)(void *Player, void* a);
int hook_isground(void *Player, void* a) {
  if (ghostmapcs) {
    return -1;
  }
  return orig_isground(Player,a);
}
static auto get_Match() {
    auto (*_get_Match)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0);
    return _get_Match(nullptr);
}
static auto get_HeadTF(void *player) {
    auto (*_get_HeadTF)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0);
    return _get_HeadTF(player);
}
static auto get_transform113(void *player) {
    auto (*_get_transform113)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0);
    return _get_transform113(player);
}
static auto get_position114(void *player) {
    auto (*_get_position114)(void *player) = (Vector3 (*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
    return _get_position114(player);
}
static auto get_position_Injected(void *player) {
    auto zero = Vector3::zero();
    auto (*_get_position_Injected)(void *player, Vector3 *zero) = (void (*)(void *, Vector3 *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1);
    _get_position_Injected(player, &zero);
    return zero;
}
void* (*old_GarenaMSDKMgr)(void *cmd, void* a);
void* GarenaMSDKMgr(void *cmd, void* a) {
        if (cmd != nullptr) {
        if (ghostmapcs) {
      if (SetMap == Vector3(0, 0, 0)) {      
      void* MatchFC = Curent_Match();
      void* LocalPlayer = GetLocalPlayer(MatchFC);
        SetMap = get_position_Injected(get_transform113(LocalPlayer));
      }
    } else {
      SetMap = Vector3(0, 0, 0);
    }
    if (ghostmapcs) {      
      void *MatchFC = get_Match();
      void *LocalPlayer = GetLocalPlayer(MatchFC);
      set_position_Injected113(get_transform113(LocalPlayer),Vvector3(get_position114(get_transform113(LocalPlayer)).x, SetMap.y + HighVp/5, get_position114(get_transform113(LocalPlayer)).z));
    } else {
      HighVp = 0;
    }
    }
    return old_GarenaMSDKMgr(cmd, a);
}
