BUILD_DIR = build
INSTALL_DIR = dist
TARGET = luna.exe

# Compiler and flags
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN -Wno-unused-parameter

# Linker flags
LDFLAGS = -static-libgcc -static-libstdc++ -Wl,--subsystem,windows

# Windows libraries
WINDOWS_LIBS = -lwinmm -lole32 -lcomctl32 -lgdi32 -luser32 -lkernel32 -lcomdlg32

# Local win32 library paths
WIN32_DIR = win32
WIN32_INCLUDE = $(WIN32_DIR)/include
WIN32_LIB = $(WIN32_DIR)/lib

# Include directories
INCLUDES = -I$(WIN32_INCLUDE) \
           -I$(WIN32_INCLUDE)/lilv-0 \
           -I$(WIN32_INCLUDE)/lv2 \
           -I$(WIN32_INCLUDE)/serd-0 \
           -I$(WIN32_INCLUDE)/sord-0 \
           -I$(WIN32_INCLUDE)/sratom-0 \
           -I$(WIN32_INCLUDE)/suil-0 \
           -Isrc

# Library directories and libraries
LIBRARY_DIRS = -L$(WIN32_LIB)
LIBS = $(LIBRARY_DIRS) -llilv-0 -lserd-0 -lsord-0 -lsratom-0 -lsuil-0 $(WINDOWS_LIBS)

# Source files
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/lv2_host.cpp \
          $(SRCDIR)/audio_engine.cpp \
          $(SRCDIR)/plugin_manager.cpp \
          $(SRCDIR)/gui/main_window.cpp \
          $(SRCDIR)/gui/plugin_list.cpp \
          $(SRCDIR)/gui/controls.cpp

OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean install directories

all: directories $(BUILD_DIR)/$(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/gui
	@mkdir -p $(INSTALL_DIR)/bin

$(BUILD_DIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)

install: $(BUILD_DIR)/$(TARGET)
	cp $(BUILD_DIR)/$(TARGET) $(INSTALL_DIR)/bin/
	cp $(WIN32_DIR)/bin/*.dll $(INSTALL_DIR)/bin/ 2>/dev/null || true

clean:
	rm -rf $(BUILD_DIR)
# 	rm -rf $(INSTALL_DIR)

# Development targets
dev: CXXFLAGS += -g -DDEBUG
dev: all

# Console version for debugging
console: LDFLAGS = -static-libgcc -static-libstdc++ -Wl,--subsystem,console
console: CXXFLAGS += -g -DDEBUG -DCONSOLE_APP
console: TARGET = luna_console.exe
console: directories $(BUILD_DIR)/$(TARGET)

.SUFFIXES: .cpp .o