@echo off
setlocal

set "SCRIPT_DIR=%~dp0"

if "%~1"=="" (
    set "EXE=%SCRIPT_DIR%orion2re.exe"
) else (
    set "EXE=%~1"
)

for %%I in ("%EXE%") do set "ROOT_DIR=%%~dpI"
if "%ROOT_DIR:~-1%"=="\" set "ROOT_DIR=%ROOT_DIR:~0,-1%"

if not exist "%EXE%" (
    echo ERROR: Could not find orion2re.exe
    echo Expected: "%EXE%"
    echo.
    echo Usage:
    echo   run_with_asan_logging.bat [full-or-relative-path-to-exe]
    exit /b 1
)

if not exist "%ROOT_DIR%\logs" mkdir "%ROOT_DIR%\logs"
if not exist "%ROOT_DIR%\dumps" mkdir "%ROOT_DIR%\dumps"

for /f %%I in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set "STAMP=%%I"

set "ASAN_OPTIONS=log_path=logs/asan_%STAMP%"
set "ASAN_SAVE_DUMPS=%ROOT_DIR%\dumps\orion2re_%STAMP%.dmp"

echo Starting "%EXE%"
echo ASan logs: "%ROOT_DIR%\logs"
echo Crash dumps: "%ROOT_DIR%\dumps"
echo.
echo If the game crashes, send the newest files from both folders.
echo.

pushd "%ROOT_DIR%"
"%EXE%" >> "%ROOT_DIR%\logs\stdout_%STAMP%.log" 2>&1
set "RC=%ERRORLEVEL%"
popd

echo.
echo Process exited with code %RC%.
exit /b %RC%