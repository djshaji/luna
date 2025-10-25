# Luna LV2 Host

A Windows LV2 plugin host built with C++ and Win32 API, cross-compiled using mingw-w64.

## Features

- **LV2 Plugin Support**: Full LV2 plugin loading and management using Lilv library
- **Native Windows GUI**: Built with Win32 API for optimal performance and system integration
- **Audio Engine**: Low-latency audio processing using Windows multimedia APIs
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
4. Browse and select plugins from the left panel
5. Monitor status in the bottom status bar

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

## Future Enhancements

- Real-time parameter automation
- MIDI support
- Plugin preset management
- Multi-track processing
- VST compatibility layer
- Advanced GUI with plugin-specific interfaces
