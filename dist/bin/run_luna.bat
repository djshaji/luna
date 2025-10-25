@echo off
echo Starting Luna LV2 Host...
echo.

REM Check if the executable exists
if not exist "luna.exe" (
    echo Error: luna.exe not found in current directory
    echo Make sure you are running this from the dist/bin directory
    pause
    exit /b 1
)

REM Run the application
luna.exe

REM If we get here, the application has exited
echo.
echo Luna LV2 Host has exited.
pause