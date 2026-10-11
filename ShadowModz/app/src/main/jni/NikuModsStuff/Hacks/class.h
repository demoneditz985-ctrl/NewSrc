
#pragma once
#include "Unity/Vector3.hpp"
#include "Unity/Quaternion.h"
#include "Include.h"

#define getRealOffset(offset) AgetAbsoluteAddress("libil2cpp.so",offset)
long AfindLibrary(const char *library) {
    char filename[0xFF] = {0},
    buffer[1024] = {0};
    FILE *fp = nullptr;
    long address = 0;

    sprintf(filename, "/proc/self/maps");

    fp = fopen( filename, "rt" );
    if( fp == nullptr ){
        perror("fopen");
        goto done;
    }

    while( fgets( buffer, sizeof(buffer), fp ) ) {
        if(strstr( buffer, library ) ){
            address = (long)strtoul( buffer, NULL, 16 );
            goto done;
        }
    }

    done:

    if(fp){
        fclose(fp);
    }
    return address;
}

long ClibBase;

long AgetAbsoluteAddress(const char* libraryName, long relativeAddr) {
    if (ClibBase == 0) {
        ClibBase = AfindLibrary(libraryName);
        if (ClibBase == 0) {
            ClibBase = 0;
        }
    }
    return ClibBase + relativeAddr;
}

class Vvector3 {
public:
    float X;
    float Y;
    float Z;
    Vvector3() : X(0), Y(0), Z(0) {}
    Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
    Vvector3(const Vvector3 &v);
    ~Vvector3();
};

Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}



std::string UTF16ToUTF8(const std::u16string& src) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(src);
}





template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};

static Vector3 VecAdd(Vector3 a, Vector3 b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
static Vector3 VecMul(Vector3 a, float s) {
    return { a.x * s, a.y * s, a.z * s };
}
static bool valid(const Vector3& v) {
    return !isnan(v.x) && !isnan(v.y) && !isnan(v.z) && v.z > 0.0f;
}

#define Class_Camera__get_main (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_main"))





#define Class_Input__get_touchCount (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"))
#define Class_Input__GetTouch (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1)
#define Class_Input__get_mousePosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_mousePosition"))

//Class Screen -> UnityEngine.dll
#define Class_Screen__get_width (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_width"))
#define Class_Screen__get_height (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_height"))
#define Class_Screen__get_density (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_dpi"))


#define ForWard (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_forward"), 0)

#define Class_Transform__GetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)

#define Class_Transform__SetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1)

#define Class_Camera__get_main (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_main"))
#define Class_Camera__WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1)

#define Class_Component__get_transform (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
#define Class_Transform__get_position (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0)


#define Class_Transform__Rotation Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_rotation"), 0)


#define Class_Compent__Transform Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
//Class Player

 #define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "get_fieldOfView")
#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset("UnityEngine.dll", "UnityEngine", "Camera", "set_fieldOfView", 1)

void *get_main() {
    return reinterpret_cast<void *(__fastcall *)()>(Class_Camera__get_main)();
}

std::string GetNameFromPlayer(void* player) {
    if (!player) return "Player";
    auto get_NickName = (void* (*)(void*))Il2CppGetMethodOffset("Assembly-CSharp.dll","COW.GamePlay","Player","get_NickName",0);
    if (!get_NickName) return "Player";
    void* nickNamePtr = get_NickName(player);
    if (!nickNamePtr) return "Player";
    int length = *(int*)((uintptr_t)nickNamePtr + 0x10);
    if (length <= 0) return "Player";
    std::u16string str16((char16_t*)((uintptr_t)nickNamePtr + 0x14), length);
    return UTF16ToUTF8(str16);
}


float get_fieldOfView() {
    return reinterpret_cast<float(__fastcall *)(void *)>(Camera_get_fieldOfView)(get_main());
}

void *set_fieldOfView(float value) {
    return reinterpret_cast<void *(__fastcall *)(void *, float)>(Camera_set_fieldOfView)(get_main(), value);
}






static void Transform_INTERNAL_SetPosition(void *player, Vvector3 inn) {
    void (*Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))(Class_Transform__SetPosition);
    Transform_INTERNAL_SetPosition(player, inn);
}




#define offset_isGod (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsGod"), 0)
static bool get_God(void *player) {
    bool (*_get_God)(void *players) = (bool (*)(void *))(offset_isGod);
    return _get_God(player);
}


#define LAimCollider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("AttackableEntity"), OBFUSCATE("set_LockedAimingCollider"), 1)
static void SetAimCollider(void* enemy, void* collider) {
    using fnSetCollider = void(*)(void*, void*);
    auto _set_collider = reinterpret_cast<fnSetCollider>(LAimCollider);
    _set_collider(enemy, collider);
}



#define Class_Camera__WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1)

#define Class_Component__get_transform (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
#define Class_Transform__get_position (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0)

#define Class_Transform__Position (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0)

#define Class_Compent__Transform Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
//Class Player
#define ListPlayer (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("NFJPHMKKEBF"), OBFUSCATE("NIEBEGJADLC"))


#define EnemyUpdate (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("LateUpdate"), 0)


#define m_Timer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameSimulationTimer"), 0)
static void* GetSimulationTimer() {
    void* (*_GetSimulationTimer) () = reinterpret_cast<void* (*)()>(m_Timer);
    return _GetSimulationTimer();
}

#define m_GetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("get_FixedDeltaTime"), 0)
static float GetTimer(void* timeServiceInstance) {
    using fnGetFixedDelta = float(*)(void*);
    auto _GetTimer = reinterpret_cast<fnGetFixedDelta>(m_GetTimer);
    return _GetTimer(timeServiceInstance);
}

#define m_SetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("UseFixedDeltaTime"), 1)
static void SetTimer(void* timeServiceInstance, float fixedDelta) {
    using fnSetFixed = void(*)(void*, float);
    auto _set_fixed = reinterpret_cast<fnSetFixed>(m_SetTimer);
    _set_fixed(timeServiceInstance, fixedDelta);
}




#define MainCam (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MainCameraTransform"))


#define Match (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0)

#define Local (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudDetectorController"), OBFUSCATE("GetLocalPlayer"), 0)


#define Visible (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsVisible"), 0)

#define Team (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalTeammate"), 1)

#define Die (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)

#define CurHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CurHP"), 0)


#define MaxHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_MaxHP"), 0)

#define Name (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_NickName"), 0)

#define Aim (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("JPNJCAONHME"), 1)
#define Scope (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsSighting"),0 )

#define Fire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsFiring"), 0)

#define LocalBool (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalPlayer"), 0)

#define CarLocal (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("VehicleIAmIn"), 0)

#define GetCar (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDriver"), 0)

#define Head (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0)

#define Hip (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHipTF"), 0)

#define CurrenIScene (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentUIScene"), 0)
static void *CurrentUIScene() {
    void *(*_CurrentUIScene)(void *nuls) = (void *(*)(void *))(CurrenIScene);
    return _CurrentUIScene(NULL);
}

#define m_currentUi Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentInGameUIScene"), 0)
static void* CurrentInGameUIScene() {
    using fnCurrentUIScene = void* (*)();
    auto _CurrentUIScene = reinterpret_cast<fnCurrentUIScene>(m_currentUi);
    return _CurrentUIScene();
}


#define Imo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetActiveWeapon"))
static void* get_imo(void* player) {
    void* (*_GetImo)(void* players) = (void* (*)(void*))Imo;
    return _GetImo(player);
}

#define Esp2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelMiniSentry"), OBFUSCATE("CPBCGAKODII"), 2)
static void set_esp2(void* imo, Vector3 x, Vector3 y) {
    void (*_SetEsp2)(void* imo, Vector3 X, Vector3 Y) = (void (*)(void*, Vector3, Vector3))Esp2;
    _SetEsp2(imo, x, y);
}



#define offset_BodyPart (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("ELMGJKHIIAA"), OBFUSCATE("KENBMOOEHBG"))

#define CenterWS (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetAttackableCenterWS"))

static Vector3 GetAttackableCenterWS(void* player) {
    Vector3 (*_GetAttackableCenterWS)(void*) = (Vector3 (*)(void*))CenterWS;
    return _GetAttackableCenterWS(player);
}





#define ShowDynamicupMessage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowDynamicPopupMessage"), 2)
static void ShowDynamicPopupMessage(monoString *nick) {
    void (*_ShowDynamicPopupMessage)(void *players, monoString * nicks, float duration) = (void (*)(void *, monoString *, float))(ShowDynamicupMessage);
    void *ui = CurrentUIScene();
    if (ui != NULL) {
        _ShowDynamicPopupMessage(ui, nick, 0.1f);
    }
}

#define offset_ShowTeammateTips (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowCenterUpTeammateTips"), 2)
static void ShowCenterUpTeammateTips(monoString *message, float duration = 3.0f) {
    if (offset_ShowTeammateTips == 0) return;    
    void (*_Show)(void *, monoString *, float) = (void (*)(void *, monoString *, float))(offset_ShowTeammateTips);    
    void *ui = CurrentInGameUIScene(); 
    if (ui != nullptr) {
        _Show(ui, message, duration);
    }
}

#define offset_ShowPopupNoAnim (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowPopupMessageNoAnim"), 2)
static void ShowPopupMessageNoAnim(monoString *message, float duration = 2.0f) {
    if (offset_ShowPopupNoAnim == 0) return;
    void (*_Show)(void *, monoString *, float) = (void (*)(void *, monoString *, float))(offset_ShowPopupNoAnim);
    void *ui = CurrentInGameUIScene(); 
    if (ui) _Show(ui, message, duration);
}

#define offset_ShowLowPopupMessage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowLowPopupMessage"), 2)
static void ShowLowPopupMessage(monoString *message, float duration = 3.0f) {
    if (offset_ShowLowPopupMessage == 0) return;    
    void (*_ShowLowPopupMessage)(void *_this, monoString *msg, float dur) = 
        (void (*)(void *, monoString *, float))(offset_ShowLowPopupMessage);    
    void *ui = CurrentInGameUIScene();     
    if (ui != nullptr) {
        _ShowLowPopupMessage(ui, message, duration);
    }
}


#define Esp1 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("CPBCGAKODII"), 4)
static void set_esp1(void* imo, Vector3 x, Vector3 y, void* obj) {
    void (*_SetEsp1)(void*, Vector3, Vector3, void*) = (void (*)(void*, Vector3, Vector3, void*))Esp1;
    _SetEsp1(imo, x, y, obj);
}

#define PlayGunTrace (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelMiniSentry"), OBFUSCATE("CPBCGAKODII"), 2)
static void set_esp3(void* imo, Vector3 x, Vector3 y) {
    void (*_set_esp3)(void *imo, Vector3 X, Vector3 Y) = (void (*)(void *, Vector3, Vector3))(PlayGunTrace);
    _set_esp3(imo, x, y);
}

#define m_addTeamHud Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIInGameScene"), OBFUSCATE("ShowAssistantText"), 2)
static void ShowAssistantText(void* uiInstance, monoString* playerName, monoString* line) {
    using fnShowAssistantText = void(*)(void*, monoString*, monoString*);
    auto _ShowAssistantText = reinterpret_cast<fnShowAssistantText>(m_addTeamHud);
    if (uiInstance != nullptr) {
        _ShowAssistantText(uiInstance, playerName, line);
    }
}




#define m_HawkerStartPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("BOGOIAMJFDN"))
#define m_HawkrtEndPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("NHKKHPLFMNG"))
#define m_HawkerHitObject (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GEGFCFDGGGP"))

#define offset_set_positionCount (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_positionCount"), 1)
#define offset_SetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("SetPosition"), 2)

#define offset_DrawLine2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("DrawLine2"), 3)
#define offset_GrenadeLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_GrenadeLine"))
#define offset_ShowGrenadeLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_ShowGrenadeLine"))
#define offset_ShowThrowSkillLine (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GrenadeLine"), OBFUSCATE("m_ShowThrowSkillLine"))
#define offset_set_startColor (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_startColor"), 1)
#define offset_set_endColor (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("LineRenderer"), OBFUSCATE("set_endColor"), 1)

struct UnityColor {
    float r, g, b, a;
};

static void LineRenderer_SetPosition(void *Render, int value, Vector3 Location){
    void (*_LineRenderer_SetPosition)(void *Render, int value, Vector3 Location) = (void (*)(void*, int, Vector3))(offset_SetPosition);
    return _LineRenderer_SetPosition(Render, value, Location);
}

static void LineRenderer_Set_PositionCount(void *Render, int value){
    void (*_LineRenderer_Set_PositionCount)(void *Render, int value) = (void (*)(void*, int))(offset_set_positionCount);
    return _LineRenderer_Set_PositionCount(Render, value);
}

static void LineRenderer_SetColor(void *Render, UnityColor color){
    void (*_set_startColor)(void *Render, UnityColor color) = (void (*)(void*, UnityColor))(offset_set_startColor);
    void (*_set_endColor)(void *Render, UnityColor color) = (void (*)(void*, UnityColor))(offset_set_endColor);
    _set_startColor(Render, color);
    _set_endColor(Render, color);
}

static void GrenadeLine_DrawLine(void *instance, Vector3 start, Vector3 end, Vector3 position) {
    void (*_GrenadeLine_DrawLine)(void *clz, Vector3 throwPos, Vector3 throwVel, Vector3 gravity) = (void (*)(void*, Vector3, Vector3,Vector3)) (offset_DrawLine2);
    return _GrenadeLine_DrawLine(instance, start, end, position);
}

void *LineGrenade = nullptr;
void *RenderLine = nullptr;

void (*UpdateGranada)(void* _this);
void _UpdateGranada(void* _this) {
    if (_this) {
        LineGrenade = _this;
        *(bool*)((uintptr_t)_this + 0x30) = true;   
        RenderLine = *(void**)((uintptr_t)_this + 0x38); 
    }
    UpdateGranada(_this);
}



#define ForWard (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_forward"), 0)

#define Aim (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("JPNJCAONHME"), 1)


#define TranGetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)

#define CharGet (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"), OBFUSCATE("String"), OBFUSCATE("get_Chars"), 1)

#define Team (uintptr_t) Il2CppGetMethodOffset("Assembly-CSharp.dll", "COW.GamePlay", "Player", "IsLocalTeammate", 1)
#define m_ShortIDToPlayers (uintptr_t) Il2CppGetFieldOffset("Assembly-CSharp.dll", "COW.GamePlay", "NFJPHMKKEBF", "NIEBEGJADLC")

#define Raycast (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JEAGCMACNNC"), OBFUSCATE("PLDCHDBCOBF"), 4)

#define Hip (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHipTF"), 0)

#define HeadColider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"), 0)

#define Time (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Time"), OBFUSCATE("get_deltaTime"), 0)
static void *GetHipPositions(void *player) {
    void *(*_GetHipPositions)(void *players) = (void*(*)(void *))(Hip);
     return _GetHipPositions(player);
}
 Vector3 get_position(void *player) {
    Vector3 (*_get_position)(void *players) = (Vector3 (*)(void *))(Class_Transform__Position);
    return _get_position(player);
}

static Vector3 GetHipPosition(void* player) {
    return get_position(GetHipPositions(player));
}

static void *Player_GetHeadCollider(void *player) {
    void *(*_Player_GetHeadCollider)(void *players) = (void *(*)(void *))(HeadColider);
    return _Player_GetHeadCollider(player);
}


static void *GetLocalPlayer(void* Game) {
    void *(*_GetLocalPlayer)(void *match) = (void *(*)(void *))(Local);
    return _GetLocalPlayer(Game);
}

static Vector3 Transform_GetPosition(void *player) {
    Vector3 out = Vector3::zero();
    void (*_Transform_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))(Class_Transform__GetPosition);
    _Transform_GetPosition(player, &out);
    return out;
}


static void *Curent_Match() {
    void *(*_Curent_Match) (void *nuls) = (void *(*)(void *))(Match);
    return _Curent_Match(NULL);
}


static void *GetHeadPositions(void *player) {
    void *(*_GetHeadPositions)(void *players) = (void*(*)(void *))(Head);
     return _GetHeadPositions(player);
}

static void *Component_GetTransform(void *player) {
    void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))(Class_Compent__Transform);
    return _Component_GetTransform(player);
}

static Vector3 WorldToScreenPoint(void *WorldCam, Vector3 WorldPos) {
    Vector3 (*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector3 (*)(void*, Vector3)) (Class_Camera__WorldToScreenPoint);
    return _WorldToScreenScene(WorldCam, WorldPos);
}


static void *Camera_main() {
    void *(*_Camera_main)(void *nuls) = (void *(*)(void *))(Class_Camera__get_main);
    return _Camera_main(nullptr);
}


static bool get_isVisible(void *player) {
    bool (*_get_isVisible)(void *players) = (bool (*)(void *))(Visible);
    return _get_isVisible(player);
}

static bool get_isLocalTeam(void *player) {
    bool (*_get_isLocalTeam)(void *players) = (bool (*)(void *))(Team);
    return _get_isLocalTeam(player);
}

static bool get_IsDieing(void *player) {
    bool (*_get_die)(void *players) = (bool (*)(void *))(Die);
    return _get_die(player);
}

static int GetHp(void* player) {
    int (*_GetHp)(void* players) = (int(*)(void *))(CurHP);
    return _GetHp(player);
}

static int get_MaxHP(void* enemy) {
    int (*_get_MaxHP)(void* player) = (int(*)(void *))(MaxHP);
    return _get_MaxHP(enemy);
}

static monoString* get_NickName(void *player) {
    monoString* (*_get_NickName)(void *players) = (monoString * (*)(void *))(Name);
    return _get_NickName(player);
}

static Vector3 GetForward(void *player) {
    Vector3 (*_GetForward)(void *players) = (Vector3 (*)(void *))(ForWard);
    return _GetForward(player);
}





static bool IsLocal(void *player) {
    bool (*_isMe)(void *players) = (bool (*)(void *))(LocalBool);
    return _isMe(player);
}
   
 static void *GetLocalCar(void *playerCar) {
    void *(*_Player_get_local)(void *Player) = (void *(*)(void *))(CarLocal);
    return _Player_get_local(playerCar);
    
    
}
static bool IsDriver(void *player) {
    bool (*_IsDriver)(void *players) = (bool (*)(void *))(GetCar);
    return _IsDriver(player);
}        
           




char get_Chars(monoString *str, int index){
    char (*_get_Chars)(monoString *str, int index) = (char (*)(monoString *, int))(CharGet);
    return _get_Chars(str, index);
}

static bool get_IsSighting(void *player) {
    bool (*_get_IsSighting)(void *players) = (bool (*)(void *))(Scope);
    return _get_IsSighting(player);
}

static bool get_IsFiring(void *player) {
    bool (*_get_IsFiring)(void *players) = (bool (*)(void *))(Fire);
    return _get_IsFiring(player);
}


static bool get_isLocalTeam(void *player, bool isCheckSocial3pEffect) {
    bool (*_get_isLocalTeam)(void *, bool) = (bool (*)(void *, bool))(Team);
    return _get_isLocalTeam(player, isCheckSocial3pEffect);
}

static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
    bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool(*)(Vector3, Vector3, unsigned int, void*))(Raycast);
    return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

Vector3 getPosition(void *transform) {
    return get_position(Component_GetTransform(transform));
}   

static Vector3 GetHeadPosition(void* player) {
    return get_position(GetHeadPositions(player));
}

static Vector3 CameraMain(void* player){
    return get_position(*(void**) ((uint64_t) player + MainCam));
}

static void set_aim(void *player, Quaternion look) {
    void (*_set_aim)(void *players, Quaternion lock) = (void (*)(void *, Quaternion))(Aim);
    _set_aim(player, look);
}


Vector3 get_mousePosition(void *_this) {
	Vector3 (*_get_mousePosition)(void *_this) = (Vector3 (*)(void *)) (Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_mousePosition"), 0));
	return _get_mousePosition(_this);
}

int get_width() {
    return reinterpret_cast<int(__fastcall *)()>(Class_Screen__get_width)();
}

int get_height() {
    return reinterpret_cast<int(__fastcall *)()>(Class_Screen__get_height)();
}

float get_density() {
    return reinterpret_cast<float(__fastcall *)()>(Class_Screen__get_density)();
}

#define m_HawkerStartPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("BOGOIAMJFDN"))
#define m_HawkrtEndPos (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("MADMMIICBNN"), OBFUSCATE("NHKKHPLFMNG"))
#define m_HawkerHitObject (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GEGFCFDGGGP"))

#define class_get_Damage (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UGCWeaponRepItem"), OBFUSCATE("get_Damage"), 0)
static int get_Damage(void* weaponItem) {
    int (*_get_Damage)(void* weaponItem) = (int(*)(void *))(class_get_Damage);
    return _get_Damage(weaponItem);
}
#define class_get_WeaponData (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIEvoGunMainPickItemController"), OBFUSCATE("get_WeaponData"), 0)
static void* get_WeaponData(void* weaponItem) {
    void* (*_get_WeaponData)(void* weaponItem) = (void* (*)(void *))(class_get_WeaponData);
    return _get_WeaponData(weaponItem);
}
#define class_get_WeaponType (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UGCWeaponGeneratorRepItem"), OBFUSCATE("get_WeaponType"), 0)
static int get_WeaponType(void* weaponItem) {
    int (*_get_WeaponType)(void* weaponItem) = (int(*)(void *))(class_get_WeaponType);
    return _get_WeaponType(weaponItem);
}
#define class_get_PlayerID (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_PlayerID"), 0)
static void* get_PlayerID(void* player) {
    void* (*_get_PlayerID)(void* player) = (void* (*)(void *))(class_get_PlayerID);
    return _get_PlayerID(player);
}

#define offset_IsClientBot (uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsClientBot"))
bool get_IsClientBot(void* player) {
    if (!player) return false;
    return *(bool*)((uint64_t)player + offset_IsClientBot);
}
#define offset_get_Range (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("JDGGIFMKIKF"), 0)
static float get_Range(void *weapon) {
    if (!weapon) return 0.0f;
    auto _get_Range = (float (*)(void *))(offset_get_Range);
    return _get_Range(weapon);
}


#define offset_GetWeaponOnHand (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetWeaponOnHand"), 0)
static void* GetWeaponOnHand(void *local) {
    void *(*_GetWeaponOnHand)(void *local) = (void *(*)(void *))(offset_GetWeaponOnHand);
    return _GetWeaponOnHand(local);
}

#define GameObject Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_gameObject"), 0)
void *get_gameObject(void *player) {
    return ((void *(*)(void *))(GameObject))(player);
}

static auto get_position11(void *player) {
    auto (*_get_position11)(void *player) = (Vector3 (*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
    return _get_position11(player);
}

#define GetTransformAddr (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
void* GetTransform(void* instance) {
    void* (*_get_transform)(void* instance) = (void* (*)(void*))GetTransformAddr;
    return _get_transform(instance);
}
static auto get_transform(void *player) {
    auto (*_get_transform)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0);
    return _get_transform(player);
}

static auto set_position_Injected(void *player, Vvector3 position) {
    auto (*_set_position_Injected)(void *player, Vvector3 position) = (void (*)(void *, Vvector3))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1);
    return _set_position_Injected(player, position);
}


static auto set_position_Injected113(void *player, Vvector3 position) {
    auto (*_set_position_Injected113)(void *player, Vvector3 position) = (void (*)(void *, Vvector3))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1);
    return _set_position_Injected113(player, position);
}



static auto get_transformm(void *player) {
    auto (*_get_transformm)(void *player) = (void *(*)(void *))(uintptr_t)  Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TransformNode"), OBFUSCATE("get_transform"), 0);
    return _get_transformm(player);
}
static auto get_positionn(void *obj) {
    auto (*_get_positionn)(void *obj) = (Vector3 (*)(void *))(uintptr_t)  Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0);
    return _get_positionn(obj);
}
static Vector3 GetHeaPosition1(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("OLCJOGDHJJJ")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRootPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("MPJBGDJJJMJ") ));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetSpinePosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("HCLMADAFLPD")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetHiPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("OLJBCONDGLO")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetLeftShoulderPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("LIBEIIIAGIK")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRightShoulderPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"), OBFUSCATE("HDEPJIBNIIK")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetLeftHandPosition1(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("JHIBMHEMJOL")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRightHandPosition1(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("NJDDAPKPILB")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetLeftToePosition1(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("FDMBKCKMODA")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRightToePosition1(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("CKABHDJDMAP")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetLeftForeArmPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("FGECMMJKFNC")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRightForeArmPosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("JBACCHNMGNJ")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetLeftAnklePosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("BMGCHFGEDDA")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}
static Vector3 GetRightAnklePosition(void* player) {
    void* node = *(void**)((uintptr_t)player + Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("AGHJLIMNPJA")));
    if (!node) return { NAN, NAN, NAN };
    void* xform = get_transformm(node);
    if (!xform) return { NAN, NAN, NAN };
    return get_positionn(xform);
}






#define offset_GetDelay (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("LMGGNHPNMNI"), 0)
static float get_Delay(void *pthis) {
    return ((float (*)(void *))(offset_GetDelay))(pthis);
}

#define offset_PlayerNetwork_StartFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartFiring"), 1)
#define offset_PlayerNetwork_StopFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1)
#define offset_StartWholeBodyFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartWholeBodyFiring"), 1)
#define offset_StartFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartFiring"), 1)
#define offset_StopFire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1)
#define offset_Weapon_StartFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("MPGJOKFGLGK"), 0)
#define offset_Weapon_StopFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("EJONAOOEOJH"), 0)


#define offset_StopFiring (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("EJONAOOEOJH"), 0)
#define offset_StartFiringer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GPBDEDFKJNA"), OBFUSCATE("MPGJOKFGLGK"), 0)

static void *StopFiring55(void *weapon) {
    void *(*_StopFiring)(void *Weapon) = (void *(*)(void *))(offset_StopFiring);
    return _StopFiring(weapon);
}
static void *StartFiring55(void *weapon) {
    void *(*_StartFiring)(void *Weapon) = (void *(*)(void *))(offset_StartFiringer);
    return _StartFiring(weapon);
}





static void StartFiringZaru(void* player, void* weapon) {
    if (!player || !weapon) return;
    auto fn = reinterpret_cast<void(*)(void*, void*)>(offset_PlayerNetwork_StartFiring);
    fn(player, weapon);
}

static void StopFireZaru(void* player, void* weapon) {
    if (!player || !weapon) return;
    auto fn = reinterpret_cast<void(*)(void*, void*)>(offset_PlayerNetwork_StopFire);
    fn(player, weapon);
}

static void StartWholeBodyFiringZaru(void* player, void* weapon) {
    if (!player || !weapon) return;
    auto fn = reinterpret_cast<void(*)(void*, void*)>(offset_StartWholeBodyFiring);
    fn(player, weapon);
}

static void StartFiring2Zaru(void* weapon) {
    if (!weapon) return;
    auto fn = reinterpret_cast<void(*)(void*)>(offset_Weapon_StartFiring);
    fn(weapon);
}

static void StopFiring2Zaru(void* weapon) {
    if (!weapon) return;
    auto fn = reinterpret_cast<void(*)(void*)>(offset_Weapon_StopFiring);
    fn(weapon);
}



static void StartonFiringZaru(void *LocalPlayer, void *WeaponHand) {
    StartFiringZaru(LocalPlayer, WeaponHand);
    StartWholeBodyFiringZaru(LocalPlayer, WeaponHand);
    StopFireZaru(LocalPlayer, WeaponHand);
}


static auto get_transform113(void *player) {
    auto (*_get_transform113)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0);
    return _get_transform113(player);
}



static auto get_Match() {
    auto (*_get_Match)(void *player) = (void *(*)(void *))Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0);
    return _get_Match(nullptr);
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




static void set_skybox_Null(void* material) {
    void (*fn)(void*) = (void (*)(void*))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("set_skybox"), 1);
    fn(material);
}
static void* get_skybox_Original() {
    void* (*fn)() = (void* (*)())Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("get_skybox"), 0);
    return fn();
}
static void set_ambientMode_Black(int mode) {
    void (*fn)(int) = (void (*)(int))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("RenderSettings"), OBFUSCATE("set_ambientMode"), 1);
    fn(mode);
}