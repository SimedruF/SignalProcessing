# Quick Setup Guide for ImGui Demo on Windows

## Problema: GLFW lipsește

Aplicația ImGui necesită GLFW (librărie pentru ferestre și input). Iată cum o instalezi:

## Opțiunea 1: MSYS2 (RECOMANDAT - cel mai simplu)

### 1.1 Instalează MSYS2
- Descarcă de la: https://www.msys2.org/
- Rulează installerul și urmează instrucțiunile
- După instalare, deschide **MSYS2 MinGW 64-bit** (nu MSYS2 MSYS!)

### 1.2 Instalează dependențele
```bash
# În terminalul MSYS2 MinGW 64-bit:
pacman -Syu
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-glfw
pacman -S mingw-w64-x86_64-cmake
```

### 1.3 Build
```bash
cd /d/dev/SignalProcessing/SignalProcessing/examples
./setup_imgui.sh
./build_imgui.sh
./imgui_demo.exe
```

## Opțiunea 2: Visual Studio (fără g++)

### 2.1 Instalează Visual Studio 2022
- Descarcă Community Edition (gratis)
- La instalare selectează: "Desktop development with C++"
- Include: "C++ CMake tools for Windows"

### 2.2 Instalează CMake
- Descarcă de la: https://cmake.org/download/
- Bifează "Add to PATH" la instalare

### 2.3 Build cu Visual Studio
```powershell
cd D:\dev\SignalProcessing\SignalProcessing\examples
.\setup_imgui.bat

# Deschide folder în Visual Studio
# File → Open → Folder → selectează "examples"
# Visual Studio va detecta CMakeLists.txt automat
# Build → Build All (F7)
```

## Opțiunea 3: vcpkg + CMake

### 3.1 Instalează vcpkg
```powershell
cd D:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### 3.2 Instalează GLFW
```powershell
.\vcpkg install glfw3:x64-windows
```

### 3.3 Instalează CMake
- Descarcă de la: https://cmake.org/download/
- Bifează "Add to PATH"

### 3.4 Build
```powershell
cd D:\dev\SignalProcessing\SignalProcessing\examples
.\setup_imgui.bat

mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=D:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
.\Release\imgui_demo.exe
```

## Verificare instalare

### Verifică g++ (MinGW):
```powershell
g++ --version
```

### Verifică CMake:
```powershell
cmake --version
```

### Verifică GLFW (după instalare MSYS2):
```bash
# În MSYS2 MinGW terminal:
pkg-config --modversion glfw3
```

## Troubleshooting

### "g++ not found"
- Instalează MinGW prin MSYS2 sau
- Adaugă MinGW la PATH

### "glfw3 not found"
- Asigură-te că ai instalat GLFW
- Verifică că folosești terminalul corect (MSYS2 MinGW, nu MSYS2 MSYS)

### "cmake not found"
- Instalează CMake de la cmake.org
- Restartează terminalul după instalare

## Recomandare finală

**Cel mai simplu:** Folosește Visual Studio 2022 Community + CMake
- Nu trebuie să instalezi manual GLFW
- Visual Studio gestionează tot automat
- Interface grafic pentru debugging

**Alternativ:** MSYS2 pentru build line de comandă
- Rapid și ușor pentru development
- Toate dependențele prin pacman
