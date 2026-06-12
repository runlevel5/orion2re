@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..") do set "ROOT_DIR=%%~fI"
set "SDL_DIR=%ROOT_DIR%\vendors\SDL"
set "SDL_COMMIT=8e37db5e797b6167f3a00d697d816a684bd259c7"

call :require_command git || exit /b 1
call :require_command cmake || exit /b 1
call :require_command ninja || exit /b 1

git -C "%ROOT_DIR%" rev-parse --show-toplevel >nul 2>&1
if errorlevel 1 (
    echo ERROR: This script must be run from within the repository checkout
    exit /b 1
)

echo Initializing SDL3 submodule...
git -C "%ROOT_DIR%" submodule sync -- vendors/SDL
if errorlevel 1 exit /b 1
git -C "%ROOT_DIR%" submodule update --init --recursive --checkout vendors/SDL
if errorlevel 1 exit /b 1

for /f %%I in ('git -C "%SDL_DIR%" rev-parse HEAD') do set "SDL_HEAD=%%I"
if /I not "%SDL_HEAD%"=="%SDL_COMMIT%" (
    echo ERROR: vendors\SDL is at %SDL_HEAD%, expected %SDL_COMMIT%
    exit /b 1
)

for /f "delims=" %%I in ('git -C "%SDL_DIR%" describe --tags --always') do set "SDL_DESC=%%I"

echo Dependency bootstrap complete.
echo SDL3 pinned at %SDL_DESC%
exit /b 0

:require_command
where %~1 >nul 2>&1
if errorlevel 1 (
    echo ERROR: Required command not found: %~1
    exit /b 1
)
exit /b 0