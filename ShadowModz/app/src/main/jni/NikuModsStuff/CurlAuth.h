#pragma once

#include <string>
#include <curl/curl.h>
#include <curl/json.hpp>
#include <thread>
#include <chrono>
#include <ctime>
#include <random>
#include <sstream>
#include <iomanip>
#include <android/log.h>
#include <fstream>
#include <sys/stat.h>
#include <jni.h>

using json = nlohmann::json;

// extern JavaVM* jvm;
void SetJavaVM(JavaVM* vm);


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

std::string getRealDeviceHWID() {
    JNIEnv* env;
    jvm->AttachCurrentThread(&env, NULL);
    
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentApp = env->GetStaticMethodID(activityThread, "currentApplication", "()Landroid/app/Application;");
    jobject context = env->CallStaticObjectMethod(activityThread, currentApp);
    
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getContentResolver = env->GetMethodID(contextClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolver);
    
    jclass settingsSecure = env->FindClass("android/provider/Settings$Secure");
    jmethodID getString = env->GetStaticMethodID(settingsSecure, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
    
    jstring androidIdStr = env->NewStringUTF("android_id");
    jstring androidId = (jstring)env->CallStaticObjectMethod(settingsSecure, getString, contentResolver, androidIdStr);
    
    const char* androidIdChars = env->GetStringUTFChars(androidId, NULL);
    std::string hwid = std::string(androidIdChars);
    env->ReleaseStringUTFChars(androidId, androidIdChars);
    
    jclass build = env->FindClass("android/os/Build");
    jfieldID modelField = env->GetStaticFieldID(build, "MODEL", "Ljava/lang/String;");
    jfieldID brandField = env->GetStaticFieldID(build, "BRAND", "Ljava/lang/String;");
    
    jstring model = (jstring)env->GetStaticObjectField(build, modelField);
    jstring brand = (jstring)env->GetStaticObjectField(build, brandField);
    
    const char* modelChars = env->GetStringUTFChars(model, NULL);
    const char* brandChars = env->GetStringUTFChars(brand, NULL);
    
    hwid += std::string(modelChars) + std::string(brandChars);
    
    env->ReleaseStringUTFChars(model, modelChars);
    env->ReleaseStringUTFChars(brand, brandChars);
    
    env->DeleteLocalRef(androidId);
    env->DeleteLocalRef(androidIdStr);
    env->DeleteLocalRef(contentResolver);
    env->DeleteLocalRef(settingsSecure);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(context);
    env->DeleteLocalRef(activityThread);
    
    jvm->DetachCurrentThread();
    
    return hwid;
}

class CurlAuth {
private:
    std::string apiUrl = OBFUSCATE("https://nikumods.site/api.php");
    std::string authKey = "";
    bool isAuthenticated = false;
    json keyData;
    std::string expiryDate = "";
    std::string m_savedHwid = "";
    std::string m_token = "";
    long m_rng = 0;
    long m_exp = 0;
    
    void clearSessionData() {
        authKey = "";
        isAuthenticated = false;
        keyData = json();
        expiryDate = "";
        m_token = "";
        m_rng = 0;
        m_exp = 0;
    }
    
    std::string getKeyFilePath() {
        return "/data/local/tmp/lunar_key.txt";
    }
    
    void saveKeyToFile(const std::string& key) {
        std::ofstream file("/data/local/tmp/lunar_key.txt");
        if (file.is_open()) {
            file << key;
            file.close();
        }
    }
    
    std::string loadKeyFromFile() {
        std::ifstream file("/data/local/tmp/lunar_key.txt");
        std::string key;
        if (file.is_open()) {
            std::getline(file, key);
            file.close();
            return key;
        }
        return "";
    }
    
    void deleteKeyFile() {
        remove("/data/local/tmp/lunar_key.txt");
    }
    
    std::string getHWID() {
        if (!m_savedHwid.empty()) {
            return m_savedHwid;
        }
        m_savedHwid = getRealDeviceHWID();
        return m_savedHwid;
    }
    
    std::string curlPost(const json& data) {
        CURL* curl = curl_easy_init();
        std::string response;
        
        if(curl) {
            std::string postData = data.dump();
            
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "User-Agent: LunarOFC/2.0");
            
            curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            
            if(res != CURLE_OK) {
                return "{\"success\":false,\"error\":\"Network Error\"}";
            }
        }
        
        return response;
    }
    
public:
    CurlAuth() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    ~CurlAuth() {
        curl_global_cleanup();
    }
    
    bool Login(const std::string& key) {
        if(key.empty() || key.length() < 10) return false;
        
        std::string hwid = getHWID();
        
        json request = {
            {"action", "login"},
            {"key", key},
            {"hwid", hwid}
        };
        
        std::string response = curlPost(request);
        
        try {
            json resp = json::parse(response);
            
            if(resp["success"] == true) {
                authKey = key;
                isAuthenticated = true;
                keyData = resp["data"];
                expiryDate = keyData.value("expiry", "Unknown");
                m_token = keyData.value("token", "");
                m_rng = keyData.value("rng", 0);
                m_exp = keyData.value("EXP", 0);
                saveKeyToFile(key);
                return true;
            } else {
                clearSessionData();
                return false;
            }
        } catch(...) {
            clearSessionData();
            return false;
        }
    }
    
    bool AutoLogin() {
        std::string savedKey = loadKeyFromFile();
        if (savedKey.empty()) {
            return false;
        }
        
        std::string hwid = getHWID();
        
        json request = {
            {"action", "login"},
            {"key", savedKey},
            {"hwid", hwid}
        };
        
        std::string response = curlPost(request);
        
        try {
            json resp = json::parse(response);
            
            if(resp["success"] == true) {
                authKey = savedKey;
                isAuthenticated = true;
                keyData = resp["data"];
                expiryDate = keyData.value("expiry", "Unknown");
                m_token = keyData.value("token", "");
                m_rng = keyData.value("rng", 0);
                m_exp = keyData.value("EXP", 0);
                return true;
            } else {
                std::string error = resp.value("error", "Unknown error");
                if (error.find("expired") != std::string::npos || 
                    error.find("locked") != std::string::npos ||
                    error.find("not found") != std::string::npos) {
                    deleteKeyFile();
                }
                clearSessionData();
                return false;
            }
        } catch(...) {
            clearSessionData();
            return false;
        }
    }
    
    void Logout() {
        clearSessionData();
        deleteKeyFile();
    }
    
    bool IsAuthenticated() { 
        return isAuthenticated; 
    }
    
    std::string GetExpiry() {
        return expiryDate;
    }
    
    std::string GetKey() {
        return authKey;
    }
    
    std::string GetHWIDValue() {
        return getHWID();
    }
    
    std::string GetToken() {
        return m_token;
    }
    
    long GetRng() {
        return m_rng;
    }
    
    long GetExp() {
        return m_exp;
    }
    
    long GetRemainingSeconds() {
        if(!isAuthenticated || m_exp <= 0) return 0;
        return m_exp - (long)time(nullptr);
    }
    
    bool IsExpired() {
        if(!isAuthenticated) return true;
        if(m_exp > 0) return GetRemainingSeconds() <= 0;
        return false;
    }
    
    int GetRemainingDays() {
        if(!isAuthenticated) return 0;
        if(keyData.contains("remaining_days")) {
            return keyData.value("remaining_days", 0);
        }
        long remaining = GetRemainingSeconds();
        if(remaining <= 0) return 0;
        return (int)(remaining / 86400);
    }
    
    // Quick server-side validity check without activating the key
    bool QuickCheck(const std::string& key) {
        json request = {
            {"action", "check"},
            {"key", key}
        };
        
        std::string response = curlPost(request);
        
        try {
            json resp = json::parse(response);
            if(resp["success"] == true && resp.value("valid", false) == true) {
                return true;
            }
        } catch(...) {}
        return false;
    }
};

