#pragma once

#if defined(__i386__) || defined(__x86_64__)
#include <dlfcn.h>
#include "SubstrateHook.h"
#include "Struct/obfuscate.h"

#ifndef DobbyHook
#define DobbyHook(address, replace, origin) MSHookFunction(address, replace, origin)
#endif

#ifndef DobbySymbolResolver
inline void* DobbySymbolResolver(const char* image_name, const char* symbol_name) {
    void* handle = dlopen(image_name, RTLD_LAZY);
    if (!handle) return nullptr;
    void* sym = dlsym(handle, symbol_name);
    return sym;
}
#endif

#endif
