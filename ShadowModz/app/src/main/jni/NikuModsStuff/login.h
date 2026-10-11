#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <jni.h>

// ============================================================
//  MINI JSON  (header only, no nlohmann / no curl dependency)
// ============================================================
namespace miniJson {

struct Value {
    enum Kind { Null, Bool, Number, String, Obj, Arr } kind = Null;
    bool b = false;
    double d = 0;
    long long i = 0;
    std::string s;
    std::vector<std::pair<std::string, Value>> members;

    const Value* find(const char* key) const {
        if (kind != Obj) return nullptr;
        for (const auto& kv : members)
            if (kv.first == key) return &kv.second;
        return nullptr;
    }
    bool getBool(const char* key, bool def = false) const {
        const Value* v = find(key);
        return v && v->kind == Bool ? v->b : def;
    }
    std::string getString(const char* key, const std::string& def = "") const {
        const Value* v = find(key);
        return v && v->kind == String ? v->s : def;
    }
    long long getInt(const char* key, long long def = 0) const {
        const Value* v = find(key);
        return v && v->kind == Number ? v->i : def;
    }
};

static inline void skipWs(const char*& p, const char* end) {
    while (p < end && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) ++p;
}

static bool parseString(const char*& p, const char* end, std::string& out) {
    if (p >= end || *p != '"') return false;
    ++p;
    out.clear();
    while (p < end) {
        char c = *p++;
        if (c == '"') return true;
        if (c == '\\') {
            if (p >= end) return false;
            char e = *p++;
            switch (e) {
                case '"': out += '"'; break;
                case '\\': out += '\\'; break;
                case '/': out += '/'; break;
                case 'b': out += '\b'; break;
                case 'f': out += '\f'; break;
                case 'n': out += '\n'; break;
                case 'r': out += '\r'; break;
                case 't': out += '\t'; break;
                case 'u': {
                    unsigned cp = 0;
                    for (int k = 0; k < 4; ++k) {
                        if (p >= end) return false;
                        char h = *p++;
                        cp <<= 4;
                        if (h >= '0' && h <= '9') cp |= (h - '0');
                        else if (h >= 'a' && h <= 'f') cp |= (h - 'a' + 10);
                        else if (h >= 'A' && h <= 'F') cp |= (h - 'A' + 10);
                        else return false;
                    }
                    if (cp <= 0x7F) out += (char)cp;
                    else if (cp <= 0x7FF) {
                        out += (char)(0xC0 | (cp >> 6));
                        out += (char)(0x80 | (cp & 0x3F));
                    } else {
                        out += (char)(0xE0 | (cp >> 12));
                        out += (char)(0x80 | ((cp >> 6) & 0x3F));
                        out += (char)(0x80 | (cp & 0x3F));
                    }
                    break;
                }
                default: return false;
            }
        } else out += c;
    }
    return false;
}

static bool parseValue(const char*& p, const char* end, Value& out) {
    skipWs(p, end);
    if (p >= end) return false;
    char c = *p;
    if (c == '{') {
        ++p;
        out.kind = Value::Obj;
        skipWs(p, end);
        if (p < end && *p == '}') { ++p; return true; }
        while (p < end) {
            skipWs(p, end);
            std::string key;
            if (!parseString(p, end, key)) return false;
            skipWs(p, end);
            if (p >= end || *p != ':') return false;
            ++p;
            Value val;
            if (!parseValue(p, end, val)) return false;
            out.members.emplace_back(key, std::move(val));
            skipWs(p, end);
            if (p >= end) return false;
            if (*p == ',') { ++p; continue; }
            if (*p == '}') { ++p; return true; }
            return false;
        }
        return false;
    }
    if (c == '"') { out.kind = Value::String; return parseString(p, end, out.s); }
    if (c == 't' && end - p >= 4 && strncmp(p, "true", 4) == 0) {
        out.kind = Value::Bool; out.b = true; p += 4; return true;
    }
    if (c == 'f' && end - p >= 5 && strncmp(p, "false", 5) == 0) {
        out.kind = Value::Bool; out.b = false; p += 5; return true;
    }
    if (c == 'n' && end - p >= 4 && strncmp(p, "null", 4) == 0) {
        p += 4; out.kind = Value::Null; return true;
    }
    if (c == '-' || (c >= '0' && c <= '9')) {
        const char* start = p;
        if (c == '-') ++p;
        while (p < end && (*p >= '0' && *p <= '9')) ++p;
        if (p < end && *p == '.') { ++p; while (p < end && (*p >= '0' && *p <= '9')) ++p; }
        if (p < end && (*p == 'e' || *p == 'E')) {
            ++p;
            if (p < end && (*p == '+' || *p == '-')) ++p;
            while (p < end && *p >= '0' && *p <= '9') ++p;
        }
        std::string num(start, p);
        out.kind = Value::Number;
        out.d = strtod(num.c_str(), nullptr);
        out.i = strtoll(num.c_str(), nullptr, 10);
        return true;
    }
    if (c == '[') {
        ++p;
        out.kind = Value::Arr;
        skipWs(p, end);
        while (p < end && *p != ']') {
            Value dummy;
            if (!parseValue(p, end, dummy)) return false;
            skipWs(p, end);
            if (p < end && *p == ',') ++p;
            skipWs(p, end);
        }
        if (p < end && *p == ']') ++p;
        return true;
    }
    return false;
}

static inline bool parse(const std::string& input, Value& out) {
    const char* p = input.c_str();
    const char* end = p + input.size();
    return parseValue(p, end, out);
}

} // namespace miniJson

using Json = miniJson::Value;

// ============================================================
//  getRealDeviceHWID - Android ID + Build info
// ============================================================
std::string getRealDeviceHWID() {
    if (!jvm) return "unknown";
    JNIEnv* env = nullptr;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK || !env) return "unknown";

    std::string hwid;
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentApp = env->GetStaticMethodID(activityThread, "currentApplication",
                                                  "()Landroid/app/Application;");
    jobject context = env->CallStaticObjectMethod(activityThread, currentApp);

    jclass contextClass = env->GetObjectClass(context);
    jmethodID getContentResolver = env->GetMethodID(contextClass, "getContentResolver",
                                                    "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolver);

    jclass settingsSecure = env->FindClass("android/provider/Settings$Secure");
    jmethodID getString = env->GetStaticMethodID(settingsSecure, "getString",
                                                 "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");

    jstring androidIdStr = env->NewStringUTF("android_id");
    jstring androidId = (jstring)env->CallStaticObjectMethod(settingsSecure, getString,
                                                             contentResolver, androidIdStr);

    const char* androidIdChars = androidId ? env->GetStringUTFChars(androidId, NULL) : nullptr;
    if (androidIdChars) {
        hwid = std::string(androidIdChars);
        env->ReleaseStringUTFChars(androidId, androidIdChars);
    }

    jclass build = env->FindClass("android/os/Build");
    jfieldID modelField = env->GetStaticFieldID(build, "MODEL", "Ljava/lang/String;");
    jfieldID brandField = env->GetStaticFieldID(build, "BRAND", "Ljava/lang/String;");

    jstring model = (jstring)env->GetStaticObjectField(build, modelField);
    jstring brand = (jstring)env->GetStaticObjectField(build, brandField);

    const char* modelChars = model ? env->GetStringUTFChars(model, NULL) : nullptr;
    const char* brandChars = brand ? env->GetStringUTFChars(brand, NULL) : nullptr;
    if (modelChars) { hwid += std::string(modelChars); env->ReleaseStringUTFChars(model, modelChars); }
    if (brandChars) { hwid += std::string(brandChars); env->ReleaseStringUTFChars(brand, brandChars); }

    if (env->ExceptionCheck()) env->ExceptionClear();
    jvm->DetachCurrentThread();
    return hwid;
}

// ============================================================
//  HTTP POST via JNI -> Java HttpURLConnection
//  No curl, no openssl => .so size stays small
// ============================================================
static std::string httpPost(const std::string& url, const std::string& body) {
    if (!jvm) return "";
    JNIEnv* env = nullptr;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK || !env) return "";

    std::string result;

    jclass urlCls = env->FindClass("java/net/URL");
    jclass connCls = env->FindClass("java/net/HttpURLConnection");
    if (urlCls && connCls && !env->ExceptionCheck()) {
        jmethodID urlCtor = env->GetMethodID(urlCls, "<init>", "(Ljava/lang/String;)V");
        jmethodID openConn = env->GetMethodID(urlCls, "openConnection", "()Ljava/net/URLConnection;");
        if (urlCtor && openConn) {
            jstring jurl = env->NewStringUTF(url.c_str());
            jobject urlObj = env->NewObject(urlCls, urlCtor, jurl);
            env->DeleteLocalRef(jurl);
            jobject conn = urlObj ? env->CallObjectMethod(urlObj, openConn) : nullptr;
            env->DeleteLocalRef(urlObj);

            if (conn && !env->ExceptionCheck()) {
                jmethodID setMethod = env->GetMethodID(connCls, "setRequestMethod", "(Ljava/lang/String;)V");
                jmethodID setProp    = env->GetMethodID(connCls, "setRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
                jmethodID setDoOut   = env->GetMethodID(connCls, "setDoOutput", "(Z)V");
                jmethodID setCT      = env->GetMethodID(connCls, "setConnectTimeout", "(I)V");
                jmethodID setRT      = env->GetMethodID(connCls, "setReadTimeout", "(I)V");
                jmethodID getOut     = env->GetMethodID(connCls, "getOutputStream", "()Ljava/io/OutputStream;");
                jmethodID getResp    = env->GetMethodID(connCls, "getResponseCode", "()I");
                jmethodID getIn      = env->GetMethodID(connCls, "getInputStream", "()Ljava/io/InputStream;");
                jmethodID getErr     = env->GetMethodID(connCls, "getErrorStream", "()Ljava/io/InputStream;");

                if (setMethod && env->ExceptionCheck()) env->ExceptionClear();

                jstring m = env->NewStringUTF("POST");
                env->CallVoidMethod(conn, setMethod, m);
                env->DeleteLocalRef(m);

                jstring k1 = env->NewStringUTF("Content-Type");
                jstring v1 = env->NewStringUTF("application/json");
                jstring k2 = env->NewStringUTF("User-Agent");
                jstring v2 = env->NewStringUTF("Mozilla/5.0 (Linux; Android)");
                env->CallVoidMethod(conn, setProp, k1, v1);
                env->CallVoidMethod(conn, setProp, k2, v2);
                env->DeleteLocalRef(k1); env->DeleteLocalRef(v1);
                env->DeleteLocalRef(k2); env->DeleteLocalRef(v2);

                env->CallVoidMethod(conn, setDoOut, JNI_TRUE);
                env->CallVoidMethod(conn, setCT, 15000);
                env->CallVoidMethod(conn, setRT, 15000);

                jobject out = env->CallObjectMethod(conn, getOut);
                if (out && !env->ExceptionCheck()) {
                    jbyteArray arr = env->NewByteArray((jsize)body.size());
                    env->SetByteArrayRegion(arr, 0, (jsize)body.size(), (const jbyte*)body.c_str());
                    jclass osCls = env->FindClass("java/io/OutputStream");
                    jmethodID osWrite = env->GetMethodID(osCls, "write", "([B)V");
                    jmethodID osFlush = env->GetMethodID(osCls, "flush", "()V");
                    jmethodID osClose = env->GetMethodID(osCls, "close", "()V");
                    env->CallVoidMethod(out, osWrite, arr);
                    env->CallVoidMethod(out, osFlush);
                    env->CallVoidMethod(out, osClose);
                    env->DeleteLocalRef(arr);
                    env->DeleteLocalRef(out);
                }

                if (!env->ExceptionCheck()) {
                    jint code = env->CallIntMethod(conn, getResp);
                    jobject stream = (code >= 200 && code < 300)
                                         ? env->CallObjectMethod(conn, getIn)
                                         : env->CallObjectMethod(conn, getErr);
                    if (stream && !env->ExceptionCheck()) {
                        jclass isCls = env->FindClass("java/io/InputStream");
                        jmethodID isRead = env->GetMethodID(isCls, "read", "([B)I");
                        jbyteArray buf = env->NewByteArray(4096);
                        while (true) {
                            jint r = env->CallIntMethod(stream, isRead, buf);
                            if (r <= 0) break;
                            jbyte* chars = env->GetByteArrayElements(buf, NULL);
                            result.append((const char*)chars, (size_t)r);
                            env->ReleaseByteArrayElements(buf, chars, JNI_ABORT);
                            if (env->ExceptionCheck()) break;
                        }
                        env->DeleteLocalRef(buf);
                        env->DeleteLocalRef(stream);
                    }
                }
                env->DeleteLocalRef(conn);
            }
        }
    }

    if (env->ExceptionCheck()) env->ExceptionClear();
    jvm->DetachCurrentThread();
    return result;
}

// ============================================================
//  Key generator (opens system browser / app link - no Unity API)
// ============================================================
void OpenKeyGeneratorURL() {
    if (!jvm) return;
    JNIEnv* env = nullptr;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK || !env) return;

    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentApp = env->GetStaticMethodID(activityThread, "currentApplication",
                                                  "()Landroid/app/Application;");
    jobject context = env->CallStaticObjectMethod(activityThread, currentApp);

    if (context && !env->ExceptionCheck()) {
        jclass uriCls = env->FindClass("android/net/Uri");
        jmethodID uriParse = env->GetStaticMethodID(uriCls, "parse",
                                                    "(Ljava/lang/String;)Landroid/net/Uri;");
        jstring urlStr = env->NewStringUTF(OBFUSCATE("https://nikumods.site"));
        jobject uri = env->CallStaticObjectMethod(uriCls, uriParse, urlStr);

        jclass intentCls = env->FindClass("android/content/Intent");
        jmethodID intentCtor = env->GetMethodID(intentCls, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
        jstring action = env->NewStringUTF("android.intent.action.VIEW");
        jobject intent = env->NewObject(intentCls, intentCtor, action, uri);

        jmethodID addFlags = env->GetMethodID(intentCls, "addFlags", "(I)Landroid/content/Intent;");
        env->CallObjectMethod(intent, addFlags, 0x10000000); // FLAG_ACTIVITY_NEW_TASK

        jmethodID startActivity = env->GetMethodID(env->GetObjectClass(context), "startActivity",
                                                   "(Landroid/content/Intent;)V");
        env->CallVoidMethod(context, startActivity, intent);
    }

    if (env->ExceptionCheck()) env->ExceptionClear();
    jvm->DetachCurrentThread();
}

// ============================================================
//  Auth class - no curl
// ============================================================
class HttpAuth {
private:
    std::string apiUrl = OBFUSCATE("https://nikumods.site/api.php");
    std::string authKey = "";
    bool isAuthenticated = false;
    Json keyData;
    std::string expiryDate = "";
    std::string m_savedHwid = "";
    std::string m_token = "";
    long m_rng = 0;
    long m_exp = 0;
    long m_remainingDays = 0;

    void clearSessionData() {
        authKey = "";
        isAuthenticated = false;
        keyData = Json();
        expiryDate = "";
        m_token = "";
        m_rng = 0;
        m_exp = 0;
        m_remainingDays = 0;
    }

    std::string getKeyFilePath() { return "/data/local/tmp/nikukey.txt"; }

    void saveKeyToFile(const std::string& key) {
        std::ofstream file(getKeyFilePath());
        if (file.is_open()) {
            file << key;
            file.close();
        }
    }

    std::string loadKeyFromFile() {
        std::ifstream file(getKeyFilePath());
        std::string key;
        if (file.is_open()) {
            std::getline(file, key);
            file.close();
            return key;
        }
        return "";
    }

    void deleteKeyFile() { remove(getKeyFilePath().c_str()); }

    std::string getHWID() {
        if (!m_savedHwid.empty()) return m_savedHwid;
        m_savedHwid = getRealDeviceHWID();
        return m_savedHwid;
    }

    static std::string jsonEscape(const std::string& s) {
        std::string out;
        for (char c : s) {
            switch (c) {
                case '"' : out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n";  break;
                case '\r': out += "\\r";  break;
                case '\t': out += "\\t";  break;
                default:   out += c;       break;
            }
        }
        return out;
    }

    std::string makeBody(const std::string& action, const std::string& key, const std::string& hwid) {
        std::string b = "{\"action\":\"";
        b += action;
        b += "\",\"key\":\"";
        b += jsonEscape(key);
        b += "\",\"hwid\":\"";
        b += jsonEscape(hwid);
        b += "\"}";
        return b;
    }

    bool applyLoginResponse(const Json& resp, const std::string& key, bool save) {
        if (!resp.getBool("success")) return false;
        authKey = key;
        isAuthenticated = true;
        const Json* data = resp.find("data");
        if (data) {
            expiryDate = data->getString("expiry", "Unknown");
            m_token = data->getString("token", "");
            m_rng = (long)data->getInt("rng", 0);
            m_exp = (long)data->getInt("EXP", 0);
            m_remainingDays = (long)data->getInt("remaining_days", 0);
        }
        if (save) saveKeyToFile(key);
        return true;
    }

public:
    bool Login(const std::string& key) {
        if (key.empty() || key.length() < 10) return false;
        std::string response = httpPost(apiUrl, makeBody("login", key, getHWID()));
        Json resp;
        if (!miniJson::parse(response, resp) || !resp.getBool("success")) {
            clearSessionData();
            return false;
        }
        return applyLoginResponse(resp, key, true);
    }

    bool AutoLogin() {
        std::string savedKey = loadKeyFromFile();
        if (savedKey.empty()) return false;

        std::string response = httpPost(apiUrl, makeBody("login", savedKey, getHWID()));
        Json resp;
        if (!miniJson::parse(response, resp)) {
            return false;
        }
        if (resp.getBool("success")) {
            return applyLoginResponse(resp, savedKey, false);
        }
        std::string error = resp.getString("error", "");
        if (error.find("expired") != std::string::npos ||
            error.find("locked") != std::string::npos ||
            error.find("not found") != std::string::npos) {
            deleteKeyFile();
        }
        clearSessionData();
        return false;
    }

    void Logout() {
        clearSessionData();
        deleteKeyFile();
    }

    bool IsAuthenticated() { return isAuthenticated; }

    std::string GetExpiry() { return expiryDate; }
    std::string GetKey() { return authKey; }
    std::string GetHWIDValue() { return getHWID(); }
    std::string GetToken() { return m_token; }
    long GetRng() { return m_rng; }
    long GetExp() { return m_exp; }

    long GetRemainingSeconds() {
        if (!isAuthenticated || m_exp <= 0) return 0;
        return m_exp - (long)time(nullptr);
    }

    bool IsExpired() {
        if (!isAuthenticated) return true;
        if (m_exp > 0) return GetRemainingSeconds() <= 0;
        return false;
    }

    int GetRemainingDays() {
        if (!isAuthenticated) return 0;
        if (m_remainingDays > 0) return (int)m_remainingDays;
        long remaining = GetRemainingSeconds();
        if (remaining <= 0) return 0;
        return (int)(remaining / 86400);
    }

    // Quick server-side validity check without activating the key
    bool QuickCheck(const std::string& key) {
        std::string response = httpPost(apiUrl, makeBody("check", key, ""));
        Json resp;
        if (!miniJson::parse(response, resp)) return false;
        return resp.getBool("success") && resp.getBool("valid");
    }
};

static HttpAuth auth;
static char inputKeyBuffer[256] = {0};
static std::string authMessage = "";
static bool authMessageError = false;
static bool gettingKey = false;
static bool showAuthWindow = true;
static bool authSuccess = false;

std::string getClipboard() {
    if (!jvm) return "";

    std::string result;
    JNIEnv *env = nullptr;
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
        const char* utf = env->GetStringUTFChars(str, 0);
        if (utf) {
            result = utf;
            env->ReleaseStringUTFChars(str, utf);
        }
        env->DeleteLocalRef(str);
        env->DeleteLocalRef(text);
    }

    env->DeleteLocalRef(CharSequenceClass);
    env->DeleteLocalRef(ClipboardManagerClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(mInitialApplication);
    env->DeleteLocalRef(activityThreadClass);
    if (env->ExceptionCheck()) env->ExceptionClear();
    jvm->DetachCurrentThread();

    return result;
}
float windowWidth = 420.0f;
    float windowHeight = 420.0f;
void ShowAuthWindow() {
    if(!showAuthWindow || authSuccess) return;
	
	

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(g_GlWidth/2 - windowWidth/2, g_GlHeight/2 - windowHeight/2 - 20), ImGuiCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(16/255.0f, 16/255.0f, 16/255.0f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.6f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);

    ImGui::Begin(OBFUSCATE("NIKUMODS LOGIN"), &showAuthWindow,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    float windowWidth = ImGui::GetWindowWidth();
float winWidth = ImGui::GetWindowWidth();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + 3), IM_COL32(50, 150, 255, 200));

    ImGui::SetCursorPosY(25);
    ImGui::SetCursorPosX((winWidth - 100) / 2);
    ImGui::SetWindowFontScale(1.3f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), OBFUSCATE("NIKUMODS"));
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPosY(55);
    ImGui::SetCursorPosX((winWidth - 120) / 2);
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), OBFUSCATE("LOGIN REQUIRED"));

    float inputWidth = 320.0f;
    float inputX = (winWidth - inputWidth) / 2;
    ImGui::SetCursorPosY(95);
    ImGui::SetCursorPosX(inputX);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(30/255.0f, 30/255.0f, 30/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 8));

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
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(40/255.0f, 40/255.0f, 40/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(50/255.0f, 50/255.0f, 50/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(45/255.0f, 45/255.0f, 45/255.0f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 7));

    if(ImGui::Button(OBFUSCATE("GET KEY"), ImVec2(buttonWidth, 40))) {
        OpenKeyGeneratorURL();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    ImGui::SameLine(0, spacing);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(40/255.0f, 40/255.0f, 40/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(50/255.0f, 50/255.0f, 50/255.0f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(45/255.0f, 45/255.0f, 45/255.0f, 1.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 7));

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
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.9f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.45f, 0.85f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 7));

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

    // Message display - Centered
    ImGui::SetCursorPosY(250);
    if(!authMessage.empty()) {
        float msgWidth = ImGui::CalcTextSize(authMessage.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - msgWidth) / 2);
        ImVec4 textColor = authMessageError ? ImVec4(1.0f, 0.4f, 0.4f, 1.0f) : ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
        ImGui::TextColored(textColor, "%s", authMessage.c_str());
    }

    // Telegram link - Centered
    ImGui::SetCursorPosY(300);
    std::string telegramText = "Contact Telegram: @ur_nikuu";
    float telegramWidth = ImGui::CalcTextSize(telegramText.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - telegramWidth) / 2);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", telegramText.c_str());

    // Instructions - Centered as block
    ImGui::SetCursorPosY(340);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

    float maxInstructionWidth = 0;
    std::string instructions[] = {
        OBFUSCATE("1. Click GET KEY Button"),
        OBFUSCATE("2. Generate Key"),
        OBFUSCATE("3. Complete Shortner"),
        OBFUSCATE("4. Paste Key & Login Enjoy")
    };

    for(const std::string& instr : instructions) {
        float w = ImGui::CalcTextSize(instr.c_str()).x;
        if(w > maxInstructionWidth) maxInstructionWidth = w;
    }

    float instructionsStartX = (windowWidth - maxInstructionWidth) / 2;
    ImGui::SetCursorPosX(instructionsStartX);

    for(const std::string& instr : instructions) {
        ImGui::Text("%s", instr.c_str());
        if(&instr != &instructions[3]) {
            ImGui::SetCursorPosX(instructionsStartX);
        }
    }

    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}
