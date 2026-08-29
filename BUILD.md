# Win86emun Build Guide

## Overview

Win86emun is a usermode emulator that runs legacy x86 (Win32) applications on Windows RT (ARMv7) devices. This guide explains how to build both x86 and ARM32 versions of the emulator.

## Prerequisites

### For x86 Build (Windows 32-bit DLLs)
- Debian Linux with MinGW-w64 installed
- Package: `mingw-w64` (`i686-w64-mingw32-g++`)

### For ARM32 Build (Windows RT DLLs)
- Debian Linux with LLVM-MinGW toolchain
- Download from: https://github.com/mstorsjo/llvm-mingw/releases
- Required compiler: `armv7-w64-mingw32-g++`

## Quick Start

### Setup LLVM-MinGW for ARM32 (if not already installed)

```bash
cd /tmp
wget https://github.com/mstorsjo/llvm-mingw/releases/download/20230614/llvm-mingw-20230614-ucrt-ubuntu-18.04-x86_64.tar.xz
tar -xf llvm-mingw-20230614-ucrt-ubuntu-18.04-x86_64.tar.xz
export PATH=/tmp/llvm-mingw-20230614-ucrt-ubuntu-18.04-x86_64/bin:$PATH
```

### Build x86 Version

```bash
cd /workspace
cp Makefile.x86 Makefile
make clean
make -j$(nproc)
```

Output DLLs will be in `release/` directory:
- `yact_emu_x86.dll` - Emulation core (x86)
- `x86_emu_x86.dll` - Wrapper DLL (x86)
- `supp_x86.dll` - Utility DLL (x86)

### Build ARM32 Version

```bash
cd /workspace
cp Makefile.arm Makefile
make clean
make -j$(nproc)
```

Output DLLs will be in `release/` directory:
- `yact_emu_arm.dll` - Emulation core (ARM32)
- `x86_emu_arm.dll` - Wrapper DLL (ARM32)
- `supp_arm.dll` - Utility DLL (ARM32)

## Makefile Variants

| Makefile | Target Architecture | Cross Compiler |
|----------|---------------------|----------------|
| `Makefile.x86` | x86 (32-bit Windows) | `i686-w64-mingw32` |
| `Makefile.arm` | ARM32 (Windows RT) | `armv7-w64-mingw32` |

## Compiler Flags

### ARM32 Specific Flags
- `-march=armv7-a`: Target ARMv7-A architecture (Windows RT devices)
- `-mthumb`: Use Thumb-2 instruction set for better code density
- `-mno-unaligned-access`: Prevent unaligned memory access faults on ARM

## Deployment to Windows RT Device

1. Copy ARM DLLs to your Windows RT device:
   - `release/yact_emu_arm.dll`
   - `release/x86_emu_arm.dll`
   - `release/supp_arm.dll`

2. Place them in the same directory as your x86 application, or in `C:\Windows\System32\`

3. **Important**: Your Windows RT device must be jailbroken to run x86 applications.

## Known Issues

### Warnings During Build
- `__declspec(thread)` attribute ignored on ARM: This is expected and does not affect functionality.
- `FASTCALL` macro redefined: This is harmless.

### Runtime Issues
- Some x86 applications may not work due to unimplemented SSE/MMX instructions.
- Applications requiring privileged instructions will fail.

## Testing

Tested applications (on Windows RT):
- 7-Zip: ✓ Works
- Notepad++: ✓ Works
- WinRAR: ⚠ Partial support

See [COMPATIBILITY.md](./COMPATIBILITY.md) for a detailed compatibility list.

## GitHub Release

To create a release with both x86 and ARM binaries:

```bash
git tag -a v1.0.0 -m "Release v1.0.0 (ARM + x86)"
git push origin v1.0.0

gh release create v1.0.0 \
    --title "Win86emu v1.0.0" \
    --notes "ARM32 (Windows RT) and x86 binaries. Jailbreak required for ARM." \
    release/yact_emu_arm.dll \
    release/x86_emu_arm.dll \
    release/supp_arm.dll \
    release/yact_emu_x86.dll \
    release/x86_emu_x86.dll \
    release/supp_x86.dll
```

## License

This project is based on the original Win86emu by mamaich, which was abandoned. See the original thread for license details.

## References

- Original Project: https://sourceforge.net/projects/win86emu/
- Fork Repository: https://github.com/satouhinata1/Win86emun
- LLVM-MinGW: https://github.com/mstorsjo/llvm-mingw
- Intel SDM: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
