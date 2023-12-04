# install.ps1

function Write-Color {
    param(
        [string]$Message,
        [ConsoleColor]$Color = 'White'
    )
    Write-Host $Message -ForegroundColor $Color
}

if (-not (Get-Command "cmake" -ErrorAction SilentlyContinue)) {
    Write-Color "Error: cmake is not installed on your system." -Color Red
    exit 1
}

$ErrorActionPreference = "Stop"

Write-Color "Configuring the project with CMake..." -Color Yellow
cmake -B build -G "MinGW Makefiles"

Write-Color "Building the project..." -Color Yellow
cmake --build build

Copy-Item "build/Server" "build/R-Type" -Destination "."

Remove-Item -Recurse -Force "build"

Write-Color "`nR-Type successfully installed" -Color Green

Write-Color "`nExecute ./Server [port] to run the server, don't forget to specify a port" -Color White
Write-Color "Execute ./R-Type to run the game`n" -Color White
