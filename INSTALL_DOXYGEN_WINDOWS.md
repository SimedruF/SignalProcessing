# Instalare Doxygen pe Windows (fără Chocolatey)

## Opțiunea 1: Download Direct de pe Site Oficial ⭐ RECOMANDAT

### Pași:

1. **Download Doxygen**
   - Mergi la: https://www.doxygen.nl/download.html
   - Descarcă: **doxygen-X.X.X-setup.exe** (Windows installer)
   - Versiunea curentă: ~1.9.8 sau mai nouă

2. **Instalare**
   - Rulează setup-ul descărcat
   - Acceptă setările default
   - **IMPORTANT**: Bifează "Add to PATH" în wizard

3. **Verificare**
   ```cmd
   doxygen --version
   ```

4. **Download Graphviz** (Opțional - pentru diagrame)
   - Mergi la: https://graphviz.org/download/
   - Descarcă: **Stable Windows install packages**
   - Rulează installer-ul
   - Adaugă la PATH manual dacă e necesar:
     * Right-click "This PC" → Properties → Advanced → Environment Variables
     * Editează PATH, adaugă: `C:\Program Files\Graphviz\bin`

5. **Generare Documentație**
   ```cmd
   .\generate_docs.bat
   ```

---

## Opțiunea 2: WinGet (Windows Package Manager)

Windows 10/11 au WinGet built-in:

```powershell
# Instalează Doxygen
winget install --id=DimitriVanHeesch.Doxygen -e

# Instalează Graphviz (opțional)
winget install --id=Graphviz.Graphviz -e

# Verificare
doxygen --version
```

**Nota**: Dacă WinGet nu există, update-ază Windows sau instalează din Microsoft Store: "App Installer"

---

## Opțiunea 3: Scoop Package Manager

```powershell
# Instalează Scoop (dacă nu există)
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
irm get.scoop.sh | iex

# Instalează Doxygen
scoop install doxygen graphviz
```

---

## Opțiunea 4: Manual ZIP (Fără Installer)

1. **Download Doxygen Portable**
   - https://www.doxygen.nl/download.html
   - Descarcă versiunea ZIP (portable)

2. **Extract**
   ```
   Extrage în: C:\Tools\doxygen\
   ```

3. **Adaugă la PATH Manual**
   - Windows Search → "Environment Variables"
   - System Properties → Advanced → Environment Variables
   - User Variables → PATH → Edit → New
   - Adaugă: `C:\Tools\doxygen\bin`

4. **Deschide CMD nou** (pentru a încărca PATH)
   ```cmd
   doxygen --version
   ```

---

## Verificare Instalare

După instalare prin oricare metodă:

```cmd
# Testează Doxygen
doxygen --version

# Testează Graphviz (opțional)
dot -V

# Generează documentația
cd D:\dev\SignalProcessing\SignalProcessing
.\generate_docs.bat
```

---

## Troubleshooting

### "doxygen is not recognized"
- Închide și redeschide CMD/PowerShell după instalare
- Verifică PATH: `echo %PATH%` (CMD) sau `$env:PATH` (PowerShell)
- Adaugă manual la PATH dacă lipsește

### "dot is not recognized" (Graphviz)
- Graphviz este opțional, doar pentru diagrame UML
- Poți genera documentație fără el:
  ```cmd
  # Edit Doxyfile:
  HAVE_DOT = NO
  ```

### WinGet nu există
- Windows Update → caută "App Installer"
- Sau download de pe Microsoft Store

---

## Generare Simplificată (fără Graphviz)

Dacă vrei doar documentația text/HTML fără diagrame:

```cmd
# Editează Doxyfile
# Schimbă: HAVE_DOT = YES
# Cu:      HAVE_DOT = NO

# Apoi:
doxygen Doxyfile
start docs\html\index.html
```

---

## Alternative Complete la Doxygen

### 1. Hdoc (Modern, Minimal)
```powershell
# Requires Rust
cargo install hdoc
hdoc source/SignalProcessing.h
```

### 2. CppDoc (Lightweight)
- Manual parse comments to markdown
- Good for small projects

### 3. Online Services
- **Codedocs.xyz** - Upload git repo
- **Read the Docs** - With Sphinx/Breathe

---

## Quick Start - Doar cu Doxygen Binary

Cea mai rapidă metodă dacă vrei să testezi ACUM:

1. Download: https://www.doxygen.nl/files/doxygen-1.9.8.windows.x64.bin.zip
2. Extract în: `C:\Tools\doxygen`
3. Run direct:
   ```cmd
   C:\Tools\doxygen\doxygen.exe Doxyfile
   start docs\html\index.html
   ```

Nu trebuie neapărat în PATH pentru a funcționa!
