@echo off

set LIB_URL=https://github.com/alandtse/CommonLibVR.git
set LIB_BRANCH=ng
set LIB_PATH=extern\CommonLibSSE-NG

echo Setting up SKSE Plugin...
echo Target submodule: %LIB_URL%
echo Target branch: %LIB_BRANCH%
echo Target path: %LIB_PATH%
echo.

REM Check if Git is available
git --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Git is not installed or not in PATH!
    echo Please install Git and try again!
    pause
    exit /b 1
)

REM Initialize Git repo if missing
if not exist ".git" (
    echo No .git folder found. Initializing new Git repository...
    git init
)

REM Remove existing submodule
if not exist "extern" mkdir extern
if exist "%LIB_PATH%" (
    echo Removing existing submodule at %LIB_PATH%...
    rmdir /s /q "%LIB_PATH%"
)

REM Add the submodule
git submodule add -b %LIB_BRANCH% %LIB_URL% %LIB_PATH%
if errorlevel 1 (
    echo Submodule add failed or already exists, attempting update...
)

REM Update the submodule
git submodule update --init --recursive

REM Clean build cache if it exists
if exist "build" (
    echo Cleaning existing build cache...
    rmdir /s /q "build"
)

echo.
echo Setup complete!
echo Press any key to continue...
pause >nul
