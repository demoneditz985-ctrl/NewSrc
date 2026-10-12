/* SHADOW MODZ - link shim for the vendored 32-bit OpenSSL prebuilt.
 *
 * NikuModsStuff/Hacks/Tools/curl/openssl-android-armeabi-v7a/lib/libcrypto.a
 * was built with an old GCC-based NDK and still calls the legacy
 * __sync_add_and_fetch_4 builtin (threads_pthread.c / CRYPTO_atomic_add).
 * Current NDKs ship compiler-rt instead of libgcc and no longer export that
 * symbol, so the armeabi-v7a link fails with "undefined symbol".
 *
 * Redefine the symbol in terms of the __atomic_* builtin that does exist,
 * using an asm label so we never redeclare a compiler builtin. Never
 * referenced on arm64-v8a, where the prebuilt does not need it.
 *
 * This file only exists to satisfy the linker - no source references it.
 */
int shadowmodz_sync_add_and_fetch_4(int *ptr, int value)
    __asm__("__sync_add_and_fetch_4");

int shadowmodz_sync_add_and_fetch_4(int *ptr, int value)
{
    return __atomic_add_fetch(ptr, value, __ATOMIC_SEQ_CST);
}
