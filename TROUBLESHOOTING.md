# Troubleshooting Luna LV2 Host

## Common Issues and Solutions

### Issue: "Failed to create main window" Error

This error can occur for several reasons. Here are the troubleshooting steps:

#### Step 1: Use the Debug Version
Run the debug version to get more detailed error information:
```cmd
luna_debug.exe
```

This version will show console output with detailed error codes and debugging information.

#### Step 2: Check Error Codes
Common Windows error codes and their meanings:

- **Error Code 1407**: `ERROR_CANNOT_FIND_WND_CLASS` - Window class registration failed
- **Error Code 1400**: `ERROR_INVALID_WINDOW_HANDLE` - Invalid window handle
- **Error Code 8**: `ERROR_NOT_ENOUGH_MEMORY` - Insufficient memory
- **Error Code 6**: `ERROR_INVALID_HANDLE` - Invalid handle

#### Step 3: System Requirements
Ensure your system meets these requirements:

- **Windows 7 or later** (64-bit)
- **Visual C++ Redistributable** (usually installed automatically)
- **Audio drivers** properly installed
- At least **50MB free disk space**
- **Administrator privileges** may be required for first run

#### Step 4: Common Fixes

1. **Run as Administrator**: Right-click on `luna.exe` and select "Run as administrator"

2. **Check Windows Defender/Antivirus**: Some antivirus software may block the application
   - Add the entire `dist/bin/` folder to your antivirus exclusions

3. **Missing DLLs**: Ensure all DLL files are in the same directory as `luna.exe`
   - Check that files like `liblilv-0.dll`, `libserd-0.dll`, etc. are present

4. **Corrupted Installation**: 
   - Delete and re-extract all files
   - Ensure you're running from the correct directory

5. **Windows Version Compatibility**:
   - Try running in compatibility mode (Windows 8/10 compatibility)

#### Step 5: Debug Information
If the debug version shows specific error codes, here's what to check:

**RegisterClass errors**:
- Usually indicates the application was closed improperly before
- Reboot your computer to clear the window class registry

**CreateWindowEx errors**:
- Check available system resources (Task Manager → Performance)
- Close other applications to free up memory

**LV2 Initialization warnings**:
- This is non-critical - the window should still appear
- Install LV2 plugins to resolve this warning

### Issue: Window Appears but No Plugins Listed

1. **Install LV2 Plugins**:
   - Download LV2 plugins and install them in standard Windows locations
   - Common locations: `C:\Program Files\LV2\` or `%APPDATA%\LV2`

2. **Plugin Path Configuration**:
   - Set the `LV2_PATH` environment variable if plugins are in custom locations

### Issue: Audio Button Doesn't Work

1. **Check Audio Drivers**: Ensure Windows audio is working properly
2. **Audio Device**: Make sure an audio output device is connected and enabled
3. **Sample Rate**: Some audio hardware requires specific sample rates (44.1kHz is default)

### Advanced Debugging

#### Using Windows Event Viewer
1. Open Event Viewer (eventvwr.msc)
2. Navigate to Windows Logs → Application
3. Look for Luna-related error entries
4. Note the error details for further investigation

#### Registry Issues
If the application fails to start repeatedly:
1. Open Registry Editor (regedit)
2. Navigate to `HKEY_CURRENT_USER\Software\Classes`
3. Look for and delete any entries related to "LunaLV2Host"
4. Restart the application

### Getting Help

If none of these solutions work:

1. **Collect Information**:
   - Windows version (run `winver`)
   - Output from `luna_debug.exe`
   - Any error messages or codes
   - System specifications

2. **Check Dependencies**:
   - Run `dumpbin /dependents luna.exe` (if you have Visual Studio tools)
   - Verify all required DLLs are present

3. **Alternative Testing**:
   - Try running on a different Windows computer
   - Test with antivirus temporarily disabled
   - Run from a different directory (e.g., Desktop)

### Environment Variables for Advanced Users

Set these environment variables for additional debugging:

```cmd
set LV2_PATH=C:\YourLV2Plugins
set LILV_DLL_PATH=C:\path\to\luna\dist\bin
```

### Build-Time Troubleshooting

If you're building from source and encountering issues:

1. **Verify mingw-w64 installation**:
   ```bash
   x86_64-w64-mingw32-g++ --version
   ```

2. **Check library paths**:
   ```bash
   ls -la win32/lib/
   ls -la win32/include/
   ```

3. **Clean build**:
   ```bash
   make clean
   make all
   ```

This should resolve most common issues with Luna LV2 Host.