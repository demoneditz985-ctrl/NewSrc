#ifndef LUNAR_MODCONFIG_H
#define LUNAR_MODCONFIG_H

#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <android/log.h>

#define MODCFG_TAG "LUNAR_CFG"
#define MODCFG_SERVER "169.58.137.21"
#define MODCFG_PORT 25565
#define MODCFG_PATH "/api/modconfig/stream"

extern bool chams, shading, wireframe, glow, neonGlow, shellOnly;
extern bool AimFire, AimLuffy, AimScope, AimAuto;

static void apply_config(const std::string &body) {
    auto get_bool = [&](const std::string &key) -> bool {
        size_t pos = body.find("\"" + key + "\"");
        if (pos == std::string::npos) return false;
        pos = body.find(":", pos);
        if (pos == std::string::npos) return false;
        pos++;
        while (pos < body.size() && body[pos] == ' ') pos++;
        return body.substr(pos, 4) == "true";
    };

    chams = get_bool("chams");
    shading = get_bool("shading");
    wireframe = get_bool("wireframe");
    glow = get_bool("glow");
    neonGlow = get_bool("neonGlow");
    shellOnly = get_bool("shellOnly");
    AimFire = get_bool("AimFire");
    AimLuffy = get_bool("AimLuffy");
    AimScope = get_bool("AimScope");
    AimAuto = get_bool("AimAuto");

    __android_log_print(ANDROID_LOG_INFO, MODCFG_TAG,
        "Config applied: chams=%d shading=%d wireframe=%d glow=%d neonGlow=%d shellOnly=%d "
        "AimFire=%d AimLuffy=%d AimScope=%d AimAuto=%d",
        chams, shading, wireframe, glow, neonGlow, shellOnly,
        AimFire, AimLuffy, AimScope, AimAuto);
}

static void *modconfig_sse_thread(void *) {
    while (true) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) { sleep(5); continue; }

        struct hostent *he = gethostbyname(MODCFG_SERVER);
        if (!he) { close(sock); sleep(5); continue; }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(MODCFG_PORT);
        memcpy(&addr.sin_addr, he->h_addr, he->h_length);

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            close(sock);
            sleep(5);
            continue;
        }

        char req[512];
        snprintf(req, sizeof(req),
            "GET %s HTTP/1.0\r\nHost: %s:%d\r\nAccept: text/event-stream\r\nConnection: close\r\n\r\n",
            MODCFG_PATH, MODCFG_SERVER, MODCFG_PORT);

        if (send(sock, req, strlen(req), 0) < 0) { close(sock); sleep(5); continue; }

        __android_log_print(ANDROID_LOG_INFO, MODCFG_TAG, "SSE connected, listening for updates...");

        char buf[4096];
        std::string resp;
        bool header_done = false;
        std::string body;

        while (true) {
            int n = recv(sock, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';

            if (!header_done) {
                resp += buf;
                size_t sep = resp.find("\r\n\r\n");
                if (sep != std::string::npos) {
                    header_done = true;
                    body = resp.substr(sep + 4);
                    resp.clear();
                }
            } else {
                body += buf;
            }

            size_t pos;
            while ((pos = body.find("\n\n")) != std::string::npos) {
                std::string event = body.substr(0, pos);
                body.erase(0, pos + 2);

                size_t data_pos = event.find("data: ");
                if (data_pos != std::string::npos) {
                    std::string json = event.substr(data_pos + 6);
                    apply_config(json);
                }
            }
        }

        close(sock);
        __android_log_print(ANDROID_LOG_INFO, MODCFG_TAG, "SSE disconnected, reconnecting in 3s...");
        sleep(3);
    }
    return nullptr;
}

static void start_modconfig_watcher() {
    pthread_t tid;
    pthread_create(&tid, nullptr, modconfig_sse_thread, nullptr);
    pthread_detach(tid);
    __android_log_print(ANDROID_LOG_INFO, MODCFG_TAG, "SSE config watcher started");
}

#endif
