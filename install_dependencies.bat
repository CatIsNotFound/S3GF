@echo off
where git >nul 2>nul
if %errorlevel% equ 0 (
    git submodule update --init --remote libs/SDL3 libs/SDL3_image libs/SDL3_ttf libs/SDL3_mixer libs/fmt libs/date
    ./libs/SDL3_ttf/external/download.sh
) else (
    echo Error: git is NOT available. Please install Git or add it to PATH.
    exit 1
)
