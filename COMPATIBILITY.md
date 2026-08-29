# Win86emun Application Compatibility List

This document tracks the compatibility status of x86 applications running on Windows RT devices using Win86emun.

## Legend

| Symbol | Status | Description |
|--------|--------|-------------|
| ✓ | Works | Application runs without issues |
| ⚠ | Partial | Application runs with minor issues or limitations |
| ✗ | Broken | Application crashes or fails to start |
| ? | Untested | Not yet tested |

## Tested Applications

### Utilities

| Application | Version | Status | Notes |
|-------------|---------|--------|-------|
| 7-Zip | 19.00 | ✓ | Fully functional, compression/extraction works |
| Notepad++ | 7.9.5 | ✓ | Basic editing works, some plugins may fail |
| WinRAR | 5.90 | ⚠ | Opens files, but some operations may crash |
| Total Commander | 9.22 | ✓ | File management works well |
| PuTTY | 0.73 | ✓ | SSH/Telnet client works |
| Notepad2 | 4.21 | ✓ | Lightweight editor, fully functional |

### Development Tools

| Application | Version | Status | Notes |
|-------------|---------|--------|-------|
| Visual Studio 6.0 | - | ✗ | Too complex, many unimplemented APIs |
| Dev-C++ | 5.11 | ⚠ | IDE opens, compilation may fail |
| UltraEdit | 26.00 | ⚠ | Basic editing works, advanced features limited |

### Multimedia

| Application | Version | Status | Notes |
|-------------|---------|--------|-------|
| VLC Media Player | 3.0.10 | ✗ | SSE instructions not fully implemented |
| Foobar2000 | 1.5.6 | ⚠ | Audio playback works, some codecs fail |
| IrfanView | 4.56 | ✓ | Image viewing works well |

### Games

| Application | Version | Status | Notes |
|-------------|---------|--------|-------|
| Doom (Source Port) | - | ? | Not yet tested |
| Classic Win32 games | - | ✗ | Most require DirectX which is not emulated |

## Known Limitations

### Unimplemented Features

1. **SSE/MMX Instructions**: Many multimedia applications require SSE instructions which are not fully implemented.
   - Implemented: Basic scalar operations (MOVSS, ADDSS, SUBSS)
   - Not implemented: Vector operations, most SSE2/SSE3/SSSE3+

2. **DirectX**: No DirectX emulation support. Games and applications requiring hardware acceleration will not work.

3. **Privileged Instructions**: Applications attempting to execute privileged CPU instructions will fail.

4. **Hardware-Specific APIs**: Applications that directly access hardware will not work.

### Performance Considerations

- Emulation overhead: Expect 10-50x slowdown compared to native x86 execution
- Memory-intensive applications may experience additional slowdowns
- Applications with tight timing loops may not function correctly

## How to Test an Application

1. Copy the ARM DLLs to your Windows RT device:
   - `yact_emu_arm.dll`
   - `x86_emu_arm.dll`
   - `supp_arm.dll`

2. Place them in the same directory as your x86 application executable.

3. Run the application and observe behavior.

4. Check `win86emu.log` for error messages if the application fails.

## Reporting Issues

When reporting compatibility issues, please include:

1. Application name and version
2. Exact error message or crash description
3. Relevant excerpt from `win86emu.log`
4. Whether the application works on native x86 Windows

## Contributing

If you've tested an application not on this list, please submit your findings:

- Application name and version
- Compatibility status (✓ / ⚠ / ✗)
- Any relevant notes or workarounds

## References

- Original compatibility discussion: http://forum.xda-developers.com/showthread.php?t=2095934
- Intel SDM for instruction reference: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
