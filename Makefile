# Win86emun Cross-Compilation Makefile for MinGW-w64
# Target: Windows x86 (32-bit)

CROSS_PREFIX = i686-w64-mingw32
CXX = $(CROSS_PREFIX)-g++
CC = $(CROSS_PREFIX)-gcc
WINDRES = $(CROSS_PREFIX)-windres

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fno-strict-aliasing
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -shared -static-libgcc -static-libstdc++

# Include paths
INCLUDES = -I./yact/inc -I./yact

# Preprocessor definitions
DEFINES = -DWIN32 -D_WINDOWS -D_USRDLL -DNDEBUG

# Directories
YACT_EMU_DIR = yact/yact_emu
X86_EMU_DIR = yact/x86_emu
SUPP_DIR = yact/supp
STUB_DLLS_DIR = yact/stub_dlls
RELEASE_DIR = release
OBJ_DIR = obj

# Create output directories
$(shell mkdir -p $(RELEASE_DIR) $(OBJ_DIR))

# Source files for supp.nt.dll
SUPP_SRCS = $(SUPP_DIR)/dllmain.cpp \
            $(SUPP_DIR)/logging.cpp \
            $(SUPP_DIR)/supp.cpp

SUPP_OBJS = $(patsubst $(SUPP_DIR)/%.cpp,$(OBJ_DIR)/supp_%.o,$(notdir $(SUPP_SRCS)))

# Source files for yact_emu.dll
YACT_EMU_SRCS = $(YACT_EMU_DIR)/cmdstream.cpp \
                $(YACT_EMU_DIR)/dllmain.cpp \
                $(YACT_EMU_DIR)/operands.cpp \
                $(YACT_EMU_DIR)/onebytecmds.cpp \
                $(YACT_EMU_DIR)/stdafx.cpp \
                $(YACT_EMU_DIR)/twobytecmds.cpp \
                $(YACT_EMU_DIR)/yact_emu.cpp

YACT_EMU_OBJS = $(patsubst $(YACT_EMU_DIR)/%.cpp,$(OBJ_DIR)/yact_emu_%.o,$(notdir $(YACT_EMU_SRCS)))

# Source files for x86_emu.dll
X86_EMU_SRCS = $(X86_EMU_DIR)/dllmain.cpp \
               $(X86_EMU_DIR)/stdafx.cpp \
               $(X86_EMU_DIR)/x86_emu.cpp

X86_EMU_OBJS = $(patsubst $(X86_EMU_DIR)/%.cpp,$(OBJ_DIR)/x86_emu_%.o,$(notdir $(X86_EMU_SRCS)))

# Stub DLLs (optional, can be built with make stubs)
STUB_DLLS = kernel32.86.dll user32.86.dll gdi32.86.dll advapi32.86.dll

# Default target
all: $(RELEASE_DIR)/supp.nt.dll $(RELEASE_DIR)/yact_emu.dll $(RELEASE_DIR)/x86_emu.dll

# Link supp.nt.dll
$(RELEASE_DIR)/supp.nt.dll: $(SUPP_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ -lwindows -lkernel32 -luser32 -ladvapi32

# Link yact_emu.dll
$(RELEASE_DIR)/yact_emu.dll: $(YACT_EMU_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ -lwindows -lkernel32

# Link x86_emu.dll (depends on supp.nt.dll)
$(RELEASE_DIR)/x86_emu.dll: $(X86_EMU_OBJS) $(RELEASE_DIR)/supp.nt.dll
	$(CXX) $(LDFLAGS) -o $@ $^ -lwindows -lkernel32 -luser32

# Compile supp sources
$(OBJ_DIR)/supp_%.o: $(SUPP_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) -DSUPP_EXPORTS $(INCLUDES) -c $< -o $@

# Compile yact_emu sources
$(OBJ_DIR)/yact_emu_%.o: $(YACT_EMU_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) -DYACT_EMU_EXPORTS $(INCLUDES) -c $< -o $@

# Compile x86_emu sources
$(OBJ_DIR)/x86_emu_%.o: $(X86_EMU_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEFINES) -DX86_EMU_EXPORTS $(INCLUDES) -c $< -o $@

# Build stub DLLs (optional target)
stubs: $(patsubst %,$(RELEASE_DIR)/%,$(STUB_DLLS))

# Generic stub DLL build rule
$(RELEASE_DIR)/%.86.dll: $(STUB_DLLS_DIR)/%_stub/*.cpp
	$(CXX) $(LDFLAGS) -D$*_STUB_EXPORTS $(DEFINES) $(INCLUDES) -o $@ $^ -lwindows -lkernel32

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(RELEASE_DIR)/*.dll $(RELEASE_DIR)/*.exe

# Phony targets
.PHONY: all clean stubs

# Debug: print variables
print-vars:
	@echo "CROSS_PREFIX: $(CROSS_PREFIX)"
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "SUPP_OBJS: $(SUPP_OBJS)"
	@echo "YACT_EMU_OBJS: $(YACT_EMU_OBJS)"
	@echo "X86_EMU_OBJS: $(X86_EMU_OBJS)"
