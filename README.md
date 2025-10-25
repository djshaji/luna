# Luna LV2 Host

A Windows LV2 plugin host built with C++ and Win32 API, cross-compiled using mingw-w64.

## Features

- **LV2 Plugin Support**: Full LV2 plugin loading and management using Lilv library
- **Native Windows GUI**: Built with Win32 API for optimal performance and system integration
- **Audio Engine**: Low-latency audio processing using Windows multimedia APIs
- **Interactive Plugin Cards**: Each plugin displays as a card with real-time parameter controls and management buttons
- **Plugin Browser**: Interactive plugin list with real-time scanning
- **Cross-Platform Build**: Uses mingw-w64 for cross-compilation from Linux

## Architecture

The project is organized into modular components:

- **Main Application** (`src/main.cpp`): Entry point and Windows application lifecycle
- **LV2 Host** (`src/lv2_host.*`): Core host functionality and coordination
- **Plugin Manager** (`src/plugin_manager.*`): LV2 plugin discovery and management using Lilv
- **Audio Engine** (`src/audio_engine.*`): Real-time audio processing and Windows audio integration
- **GUI Components** (`src/gui/`): Win32-based user interface
  - Main Window: Primary application window with layout management
  - Plugin List: Interactive plugin browser and selection
  - Active Plugin Cards: Interactive plugin cards with parameter sliders and control buttons
  - Controls: Reusable UI components

## Dependencies

The project uses these key libraries (included in `win32/` directory):

- **Lilv**: LV2 plugin host library
- **LV2**: LV2 plugin specification headers
- **Serd**: RDF serialization library
- **Sord**: RDF storage library
- **Sratom**: LV2 atom serialization
- **Suil**: LV2 UI host library

## Building

### Prerequisites

- mingw-w64 cross-compiler toolchain
- Make build system

On Ubuntu/Debian:
```bash
sudo apt install mingw-w64 make
```

### Build Commands

```bash
# Build the project
make all

# Clean build artifacts
make clean

# Build and install to dist/ directory
make install

# Build with debug symbols
make dev
```

### Using the Build Script

```bash
# Simple build
./build.sh

# Build and install
./build.sh install
```

## Output

- **Executable**: `build/luna.exe` (standalone build)
- **Distribution**: `dist/bin/luna.exe` (with required DLLs)

The distribution directory contains the executable along with all required runtime libraries.

## Usage

1. Run `luna.exe` on a Windows system
2. The application will scan for installed LV2 plugins
3. Use the "Start Audio" button to enable audio processing
4. Browse available plugins in the left panel (30% of window width)
5. Double-click plugins to create interactive plugin cards (right panel, 70% width)
6. Use parameter sliders on each card to control plugin settings in real-time
7. Use bypass (🔊/🔇) and remove (🗑️) buttons on individual plugin cards
8. Use "Clear All" button to remove all plugin cards at once
9. Monitor all operations and parameter changes in the bottom status bar

## Development

### Project Structure

```
luna/
├── src/                    # Source code
│   ├── gui/               # GUI components
│   ├── main.cpp           # Application entry point
│   ├── lv2_host.*         # Core host functionality
│   ├── plugin_manager.*   # Plugin management
│   └── audio_engine.*     # Audio processing
├── win32/                 # Dependencies (Lilv, LV2, etc.)
│   ├── include/          # Header files
│   ├── lib/              # Static libraries
│   └── bin/              # Runtime DLLs
├── build/                 # Build artifacts
├── dist/                  # Distribution files
├── Makefile              # Build configuration
└── build.sh              # Build script
```

### Adding Features

The modular architecture makes it easy to extend functionality:

- **New GUI components**: Add to `src/gui/`
- **Audio effects**: Extend `audio_engine.cpp`
- **Plugin features**: Enhance `plugin_manager.cpp`
- **Host capabilities**: Modify `lv2_host.cpp`

## License

This project is designed for educational and development purposes. Please ensure compliance with LV2 and dependency licenses when distributing.

## Contributing

1. Follow C++17 standards
2. Use consistent coding style
3. Test cross-compilation compatibility
4. Document new features

## Troubleshooting

### Build Issues

- Ensure mingw-w64 is properly installed
- Verify `win32/` directory contains all dependencies
- Check that `libsuil-0` and other libraries are present in `win32/lib/`

### Runtime Issues

- Ensure all DLLs are in the same directory as `luna.exe`
- Verify LV2 plugins are installed in standard Windows locations
- Check Windows audio drivers are functioning

## Interactive Plugin Cards Feature

Luna features a modern **Two-Panel Plugin Card System** that provides professional plugin control capabilities:

- **Two-Panel Design**: Available plugins (left) and active plugin cards (right)
- **Interactive Cards**: Each plugin displays as a card with parameter sliders and control buttons
- **Real-time Parameter Control**: Adjust plugin parameters with sliders that show live values
- **Individual Plugin Management**: Bypass (🔊/🔇) or remove (🗑️) plugins independently
- **Professional Layout**: 30%/70% split optimized for plugin browsing and control

For detailed information about the plugin cards feature, see [PLUGIN_CHAIN.md](PLUGIN_CHAIN.md).

## Future Enhancements

- **Enhanced Parameter Discovery**: Automatic detection of all plugin parameters with proper names and ranges
- **Plugin Card Reordering**: Drag-and-drop reordering of plugin cards in the processing chain
- **Card Presets**: Save and load plugin card configurations and parameter settings
- **MIDI Parameter Control**: Map MIDI controllers to plugin card parameters
- **Automation**: Timeline-based parameter automation for plugin cards
- **Plugin Preset Management**: Load and save individual plugin presets within cards
- **Multi-track Processing**: Multiple plugin card chains for different audio tracks
- **VST Compatibility Layer**: Support VST plugins alongside LV2 in plugin cards
- **Advanced Plugin UIs**: Native plugin interfaces embedded in cards when available
