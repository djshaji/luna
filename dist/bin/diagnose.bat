@echo off
echo Luna LV2 Host Diagnostic Tool
echo ===============================
echo.

echo Checking system information...
echo Windows Version:
ver
echo.

echo Checking files in current directory...
if exist luna.exe (
    echo [OK] luna.exe found
) else (
    echo [ERROR] luna.exe not found
    echo Make sure you're running this from the dist/bin directory
    goto :end
)

if exist luna_debug.exe (
    echo [OK] luna_debug.exe found
) else (
    echo [WARN] luna_debug.exe not found - debug version not available
)

echo.
echo Checking required DLL files...
set "missing_dlls="

for %%f in (liblilv-0.dll libserd-0.dll libsord-0.dll libsratom-0.dll libsuil-0.dll) do (
    if exist %%f (
        echo [OK] %%f found
    ) else (
        echo [ERROR] %%f missing
        set "missing_dlls=yes"
    )
)

if defined missing_dlls (
    echo.
    echo [ERROR] Some required DLL files are missing!
    echo Please ensure all files from the dist/bin directory are present.
    goto :end
)

echo.
echo All files appear to be present. Attempting to run Luna...
echo.
echo ================================
echo Running luna_debug.exe for detailed output...
echo ================================
echo.

if exist luna_debug.exe (
    luna_debug.exe
) else (
    echo Debug version not available, running regular version...
    luna.exe
)

:end
echo.
echo Diagnostic completed. Press any key to exit.
pause >nul