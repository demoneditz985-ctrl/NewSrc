#!/usr/bin/env python3
"""Repair #include "..." directives whose letter case does not match the file on disk.

This JNI tree was authored on case-insensitive filesystems (Windows / macOS),
so includes such as

    #include "dobby/dobby.h"     -> on disk: NikuModsStuff/Dobby/dobby.h
    #include "Il2cpp.h"          -> on disk: NikuModsStuff/Unity/Il2Cpp.h

only resolve there. ndk-build runs on Linux (case-sensitive), so those
translation units die with "file not found".

This pass indexes every file in the tree once, then rewrites only the quoted
includes that cannot be resolved, pointing them at the real file. It is a
build-time shim for CI: it never touches the committed sources.

Usage: fix-include-case.py <jni-root>
"""
import os
import re
import sys

INCLUDE_ROOTS = [
    "NikuMods",
    "NikuMods/Assets",
    "NikuMods/Fonts",
    "NikuModsStuff",
    "NikuModsStuff/include",
    "NikuModsStuff/include/And64InlineHook",
    "NikuModsStuff/include/Substrate",
    "NikuModsStuff/imgui",
    "NikuModsStuff/KittyMemory",
    "NikuModsStuff/Unity",
]

SOURCE_EXT = (".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".inl")
INCLUDE_RE = re.compile(r'^(\s*#\s*include\s*)"([^"]+)"', re.M)


def main():
    if len(sys.argv) != 2:
        sys.exit("usage: fix-include-case.py <jni-root>")

    root = os.path.abspath(sys.argv[1])
    if not os.path.isdir(root):
        sys.exit("not a directory: %s" % root)

    # index every file by its lowercased basename
    by_name = {}
    for dirpath, _dirnames, filenames in os.walk(root):
        for name in filenames:
            by_name.setdefault(name.lower(), []).append(os.path.join(dirpath, name))

    roots = [root] + [os.path.join(root, r) for r in INCLUDE_ROOTS]
    roots = [r for r in roots if os.path.isdir(r)]

    def resolves(cur_dir, inc):
        for base in [cur_dir] + roots:
            if os.path.isfile(os.path.join(base, inc)):
                return True
        return False

    fixes = []
    for dirpath, _dirnames, filenames in os.walk(root):
        for name in filenames:
            if not name.lower().endswith(SOURCE_EXT):
                continue
            path = os.path.join(dirpath, name)
            try:
                src = open(path, encoding="utf-8", errors="replace").read()
            except OSError:
                continue

            out = src
            dirty = False
            for match in INCLUDE_RE.finditer(src):
                inc = match.group(2)
                if resolves(dirpath, inc):
                    continue

                candidates = by_name.get(os.path.basename(inc).lower())
                if not candidates:
                    continue

                wanted_dir = os.path.basename(os.path.dirname(inc)).lower()
                pick = None
                if wanted_dir:
                    for cand in candidates:
                        if os.path.basename(os.path.dirname(cand)).lower() == wanted_dir:
                            pick = cand
                            break
                if pick is None:
                    pick = sorted(candidates, key=len)[0]

                rel = os.path.relpath(pick, dirpath).replace(os.sep, "/")
                out = out.replace(match.group(0), '%s"%s"' % (match.group(1), rel))
                fixes.append((os.path.relpath(path, root), inc, rel))
                dirty = True

            if dirty:
                open(path, "w", encoding="utf-8", errors="replace").write(out)

    for where, old, new in fixes:
        print("include fix: %-58s %-30s -> %s" % (where, old, new))
    print("total include fixes: %d" % len(fixes))


if __name__ == "__main__":
    main()
