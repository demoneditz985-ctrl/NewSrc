#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <Il2Cpp.h>
#include <SubstrateHook.h>
#include <CydiaSubstrate.h>
#include "Struct/Gui.hpp"
#include <Struct/main.h>
#include "KittyMemory/MemoryPatch.h"
#include "ModConfig.h"
#include "ZaruPatch.h"
#include "CurlAuth.h"
#include "NikuMods/Win95UI.h"
#include "NikuModsPreviewPlugin.h"

#define ZPATCH(offset, hex) do{ \
    uintptr_t _addr = (uintptr_t)(offset); \
    if(_addr && _addr > 0x1000) ZaruPatch(_addr, hex).Modify(); \
}while(0)

#define PATCH_RET(addr) MemoryPatch::createWithHex((uintptr_t)(addr), OBFUSCATE("C0 03 5F D6")).Modify()
#define PATCH_RET_TRUE(addr) MemoryPatch::createWithHex((uintptr_t)(addr), OBFUSCATE("20 00 80 D2 C0 03 5F D6")).Modify()
#define PATCH_RET_FALSE(addr) MemoryPatch::createWithHex((uintptr_t)(addr), OBFUSCATE("00 00 80 D2 C0 03 5F D6")).Modify()
#define FAL "00 00 80 52 C0 03 5F D6"
#define TRU "20 00 80 52 C0 03 5F D6"
#define MULL "00 00 80 D2 C0 03 5F D6"
#define RET "C0 03 5F D6"


static int currentTab = 0;
bool ImGuiOK = true;
bool clearMousePos = true;
bool initImGui = false;

uintptr_t il2cpp_base = 0;
void *getRealAddr(ulong offset) {
    return reinterpret_cast<void*>(il2cpp_base + offset);
}

namespace Settings {
    static int Tab = 1;
}
#define Application_OpenURL(url) ((void(*)(Il2CppString*))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Application"), OBFUSCATE("OpenURL"), 1))(url)

static CurlAuth auth;
static char inputKeyBuffer[256] = {0};
static std::string authMessage = "";
static bool authMessageError = false;
static bool gettingKey = false;
static bool showAuthWindow = true;
static bool authSuccess = false;

void SetJavaVM(JavaVM* vm) {
    jvm = vm;
}

std::string getClipboard() {
    if (!jvm) return "";
    
    std::string result;
    JNIEnv *env;
    
    jvm->AttachCurrentThread(&env, NULL);
    
    auto looperClass = env->FindClass("android/os/Looper");
    auto prepareMethod = env->GetStaticMethodID(looperClass, "prepare", "()V");
    env->CallStaticVoidMethod(looperClass, prepareMethod);
    
    jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
    jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, "sCurrentActivityThread", "Landroid/app/ActivityThread;");
    jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
    
    jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, "mInitialApplication", "Landroid/app/Application;");
    jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
    
    auto contextClass = env->FindClass("android/content/Context");
    auto getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    
    auto str = env->NewStringUTF("clipboard");
    auto clipboardManager = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, str);
    env->DeleteLocalRef(str);
    
    jclass ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
    auto getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");
    
    jclass CharSequenceClass = env->FindClass("java/lang/CharSequence");
    auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString", "()Ljava/lang/String;");
    
    auto text = env->CallObjectMethod(clipboardManager, getText);
    if (text) {
        str = (jstring) env->CallObjectMethod(text, toStringMethod);
        result = env->GetStringUTFChars(str, 0);
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }
    
    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    jvm->DetachCurrentThread();
    
    return result;
}

#define Application_OpenURL(url) ((void(*)(Il2CppString*))Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Application"), OBFUSCATE("OpenURL"), 1))(url)

void OpenKeyGeneratorURL() {
    // Endpoint lives in NikuMods/ShadowBrand.h (SHADOW_KEY_URL_LITERAL).
    std::string url = OBFUSCATE(SHADOW_KEY_URL_LITERAL);
    Application_OpenURL(Il2CppString::Create(url.c_str()));
}
//Auth System ( line 118 - 270 ) 🫦
void ShowAuthWindow() {
    if(!showAuthWindow || authSuccess) return;
    
    ImGui::SetNextWindowSize(ImVec2(720, 600), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(g_GlWidth/2 - 260, g_GlHeight/2 - 225), ImGuiCond_Always);
    
    // SHADOW MODZ login: dark black glass with a violet seam across the top.
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(11/255.0f, 8/255.0f, 18/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(168/255.0f, 85/255.0f, 247/255.0f, 0.85f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    
    ImGui::Begin(OBFUSCATE("SHADOW MODZ LOGIN"), &showAuthWindow, 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    
    float windowWidth = ImGui::GetWindowWidth();
    
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + 4), IM_COL32(124, 58, 237, 255));
    draw->AddRectFilled(ImVec2(pos.x, pos.y + 4), ImVec2(pos.x + size.x, pos.y + 5),
                        IM_COL32(216, 180, 254, 130));
    
    ImGui::SetCursorPosY(30);
    ImGui::SetCursorPosX((windowWidth - 120) / 2);
    ImGui::SetWindowFontScale(1.4f);
    ImGui::TextColored(ImVec4(216/255.0f, 180/255.0f, 254/255.0f, 1.0f), OBFUSCATE("SHADOW MODZ"));
    ImGui::SetWindowFontScale(1.0f);
    
    ImGui::SetCursorPosY(70);
    ImGui::SetCursorPosX((windowWidth - 150) / 2);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), OBFUSCATE("LOGIN REQUIRED"));
    
    float inputWidth = 400;
    float inputX = (windowWidth - inputWidth) / 2;
    ImGui::SetCursorPosY(120);
    ImGui::SetCursorPosX(inputX);
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(18/255.0f, 14/255.0f, 28/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(26/255.0f, 21/255.0f, 38/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(34/255.0f, 27/255.0f, 54/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 10));
    
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputText(OBFUSCATE("##KEY"), inputKeyBuffer, sizeof(inputKeyBuffer), 
        ImGuiInputTextFlags_CharsNoBlank);    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);    
    ImGui::SetCursorPosY(185);    
    float buttonWidth = 110;
    float spacing = 15;
    float totalButtonsWidth = (buttonWidth * 3) + (spacing * 2);
    float buttonsStartX = (windowWidth - totalButtonsWidth) / 2;   
    ImGui::SetCursorPosX(buttonsStartX);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(26/255.0f, 21/255.0f, 38/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(58/255.0f, 42/255.0f, 92/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(88/255.0f, 52/255.0f, 140/255.0f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8));    
    if(ImGui::Button(OBFUSCATE("GET KEY"), ImVec2(buttonWidth, 40))) {
        OpenKeyGeneratorURL();
    }    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);    
    ImGui::SameLine(0, spacing);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(26/255.0f, 21/255.0f, 38/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(58/255.0f, 42/255.0f, 92/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(88/255.0f, 52/255.0f, 140/255.0f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8));    
    if(ImGui::Button(OBFUSCATE("PASTE"), ImVec2(buttonWidth, 40))) {
        std::string clipboard = getClipboard();
        if(!clipboard.empty()) {
            strncpy(inputKeyBuffer, clipboard.c_str(), sizeof(inputKeyBuffer) - 1);
            inputKeyBuffer[sizeof(inputKeyBuffer) - 1] = '\0';
            authMessage = "Key pasted!";
            authMessageError = false;
        } else {
            authMessage = "Clipboard empty!";
            authMessageError = true;
        }
    }    
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);    
    // LOGIN Button
    ImGui::SameLine(0, spacing);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.49f, 0.23f, 0.93f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.66f, 0.33f, 0.97f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.36f, 0.15f, 0.78f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8));    
    if(ImGui::Button(OBFUSCATE("LOGIN"), ImVec2(buttonWidth, 40))) {
        if(strlen(inputKeyBuffer) > 0) {
            std::thread([&]() {
                std::string key(inputKeyBuffer);
                if(auth.Login(key)) {
                    authMessage = "Login successful! Welcome!";
                    authMessageError = false;
                    authSuccess = true;
                    showAuthWindow = false;
                } else {
                    authMessage = "Invalid key or HWID locked!";
                    authMessageError = true;
                }
            }).detach();
        } else {
            authMessage = "Please enter or paste a key first!";
            authMessageError = true;
        }
    }   
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::SetCursorPosY(250);
    if(!authMessage.empty()) {
        float msgWidth = ImGui::CalcTextSize(authMessage.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - msgWidth) / 2);
        ImVec4 textColor = authMessageError ? ImVec4(1.0f, 0.4f, 0.4f, 1.0f) : ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
        ImGui::TextColored(textColor, "%s", authMessage.c_str());
    }
    ImGui::SetCursorPosY(300);
    std::string telegramText = std::string(ShadowBrand::TelegramLabel) + ": " + ShadowBrand::TelegramText;
    float telegramWidth = ImGui::CalcTextSize(telegramText.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - telegramWidth) / 2);
    ImGui::TextColored(ImVec4(0.78f, 0.62f, 0.95f, 1.0f), "%s", telegramText.c_str());
    ImGui::SetCursorPosY(340);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.58f, 0.72f, 1.0f));

    float maxInstructionWidth = 0;
    const char* instructions[] = {
        OBFUSCATE("1. Click GET KEY Button"),
        OBFUSCATE("2. Generate Key"), 
        OBFUSCATE("3. Complete Shortner"),
        OBFUSCATE("4. Paste Key & Login Enjoy")
    };    
    for(const char* instr : instructions) {
        float w = ImGui::CalcTextSize(instr).x;
        if(w > maxInstructionWidth) maxInstructionWidth = w;
    }    
    float instructionsStartX = (windowWidth - maxInstructionWidth) / 2;
    ImGui::SetCursorPosX(instructionsStartX);
    
    for(const char* instr : instructions) {
        ImGui::Text("%s", instr);
        if(instr != instructions[3]) {
            ImGui::SetCursorPosX(instructionsStartX);
        }
    }    
    ImGui::PopStyleColor();    
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}

void ShadowMenu07() {//uncomment this gay's 🏳️‍🌈 if u need login 
/*if (!authSuccess) { 
    ShowAuthWindow();
    return;
}*/

// Layout call here bitches 💔
Win95UI::Win95Menu();
//Ui All Codes here NikuMods/Win95UI.h 
}

inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
        prevWidth = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();   // textures are built inside SetupImgui
        g_IsSetup = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    
    static bool isTouching = false;
    static float touchStartTime = 0.0f;

    if (ImGuiOK) {
    int touchCount = ((int (*)())(Class_Input__get_touchCount))();
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Class_Input__GetTouch))(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
        io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
        if (!isTouching) {
            touchStartTime = ImGui::GetTime();
        }
        io.MouseDown[0] = true;
        isTouching = true;
    }
    else {
        if (isTouching && (ImGui::GetTime() - touchStartTime < 0.06f)) {
            io.MouseDown[0] = true;
        } else {
            io.MouseDown[0] = false;
            isTouching = false;
        }
    }
    }
                    
	UpdateResetGuest();
    DrawESP(g_GlWidth, g_GlHeight);
    ShadowMenu07();
   
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

inline void StartGUI() {
    void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (ptr_eglSwapBuffer != nullptr) {
        DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
        LOGD("GUI started successfully");
    }
}



#define offset_UpdateLocalMark Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIMapContentController"), OBFUSCATE("UpdateLocalMark"), 4)
bool libLoaded = false;
uintptr_t findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    uintptr_t address = 0;
    sprintf(filename, OBFUSCATE("/proc/self/maps"));
    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (uintptr_t)strtoul(buffer, NULL, 16);
            goto done;
        }
    }
    done:
    if (fp) {
        fclose(fp);
    }
    return address;
}

uintptr_t libBase = 0; 
uintptr_t getAbsoluteAddress(const char *libraryName, uintptr_t relativeAddr) {
    libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (libBase + relativeAddr);
}
void hack_thread() {
    sleep(10);
    pid_t pid = getpid();
    LOGD("Hack Thread initiated for PID: %i", pid);
    do {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        sleep(3);
    } while (il2cpp_base == 0);
    LOGD("libil2cpp.so found at address: 0x%" PRIxPTR, il2cpp_base);
    Il2CppAttach();
	// SHADOW MODZ - Telegram: COMING SOON
	
    DobbyHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"),OBFUSCATE("Player"),OBFUSCATE("get_InSwapWeaponCD"),0),(void*)_get_InSwapWeaponCD,(void**)&get_InSwapWeaponCD);
	DobbyHook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("UpdateBehavior"), 2), (void*)_LateUpdate, (void**)&LateUpdate);
    DobbyHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9), (void *) &hook_PlayerNetwork_TakeDamage, (void **) &orig_PlayerNetwork_TakeDamage);
 	DobbyHook((void *) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"),OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"),OBFUSCATE("IsWeaponInAutoCharge"), 0),(void *) IsWeaponInAutoCharge_Hook, (void **) &old_IsWeaponInAutoCharge);
	DobbyHook((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_ShowDamageNum"), 0), (void*)hook_get_ShowDamageNum, (void**)&orig_get_ShowDamageNum);                          
    dobbyhooks();
    StartGUI();
    ImGuiOK = true;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    std::thread(hack_thread).detach();
    return JNI_VERSION_1_6;
}
