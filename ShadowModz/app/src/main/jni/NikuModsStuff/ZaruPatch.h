/**
 * @file ZaruPatch.h
 * @brief Advanced memory patching system for ARM64 Android Games
 * @author @zaruHereee
 * @telegram https://t.me/+NLmiH5sZPxE0ZDRl
 * @copyright All rights reserved 2026
 * 
 * ZaruPatch provides a stealthy memory modification framework utilizing
 * /proc/self/mem for anti-detection and fallback mprotect mechanisms.
 * 
 * Features:
 * - Dual patching methods (/proc/self/mem + mprotect)
 * - ARM64 cache synchronization
 * - Invalid address protection
 * - Zero external dependencies
 * 
 * @note Designed for ARM64 architecture only
 * @warning Use at your own risk
 */

#pragma once
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <algorithm>

#define ZARUPATCH_VERSION "1.0.0"

class ZaruPatch {
private:
    uintptr_t m_addr;
    std::string m_hex;
    size_t m_len;
    bool m_applied;
    static int m_memFd;
    
    /**
     * @brief Converts hex string to byte array
     * @param hex Input hex string (spaces optional)
     * @param out Output byte buffer
     * @return Number of bytes written
     */
    static size_t hexToBytes(const std::string& hex, uint8_t* out) {
        std::string clean = hex;
        clean.erase(std::remove(clean.begin(), clean.end(), ' '), clean.end());
        for (size_t i = 0; i < clean.length() / 2; i++) {
            sscanf(clean.c_str() + (i * 2), "%2hhx", &out[i]);
        }
        return clean.length() / 2;
    }
    
public:
    /**
     * @brief Constructs a patch object
     * @param addr Target memory address
     * @param hexBytes Hex string to write
     */
    ZaruPatch(uintptr_t addr, const std::string& hexBytes) 
        : m_addr(addr), m_hex(hexBytes), m_applied(false) {
        uint8_t temp[256];
        m_len = hexToBytes(hexBytes, temp);
    }
    
    /**
     * @brief Applies the patch to memory
     * @note Uses /proc/self/mem first for stealth, falls back to mprotect
     */
    void Modify() {
        if (m_applied || m_addr == 0 || m_addr < 0x1000) return;
        
        uint8_t bytes[256];
        hexToBytes(m_hex, bytes);
        
        if (m_memFd != -1) {
            lseek(m_memFd, m_addr, SEEK_SET);
            write(m_memFd, bytes, m_len);
        } else {
            long page = sysconf(_SC_PAGESIZE);
            uintptr_t start = m_addr & ~(page - 1);
            mprotect((void*)start, page, PROT_READ | PROT_WRITE | PROT_EXEC);
            memcpy((void*)m_addr, bytes, m_len);
            mprotect((void*)start, page, PROT_READ | PROT_EXEC);
            __builtin___clear_cache((char*)m_addr, (char*)m_addr + m_len);
        }
        m_applied = true;
    }
    
    /**
     * @brief Restores original memory (placeholder)
     * @note Full restore requires saving original bytes first
     */
    void Restore() {
        m_applied = false;
    }
    
    /**
     * @brief Initializes the patch system
     * @note Call once before any patches
     */
    static void Init() {
        if (m_memFd == -1) {
            m_memFd = open("/proc/self/mem", O_RDWR);
        }
    }
    
    /**
     * @brief Cleans up resources
     */
    static void Cleanup() {
        if (m_memFd != -1) {
            close(m_memFd);
            m_memFd = -1;
        }
    }
    
    /**
     * @brief Checks if patch was applied
     * @return true if applied, false otherwise
     */
    bool IsApplied() const { return m_applied; }
};

int ZaruPatch::m_memFd = -1;

// Utility macros
#define ZARU_PATCH(addr, hex) ZaruPatch((uintptr_t)(addr), hex).Modify()
#define ZARU_PATCH_SAFE(addr, hex) do{ \
    uintptr_t __a = (uintptr_t)(addr); \
    if(__a > 0x10000) ZaruPatch(__a, hex).Modify(); \
}while(0)

/* 
 * Created by @zaruHereee
 * Telegram: https://t.me/+NLmiH5sZPxE0ZDRl
 */