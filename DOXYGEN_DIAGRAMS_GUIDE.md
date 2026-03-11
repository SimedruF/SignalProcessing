# Unde să Găsești Diagramele în Documentația Doxygen

## Locații ale Diagramelor

După ce instalezi Graphviz și regenerezi documentația, diagramele vor apărea în mai multe locuri:

### 1. **Call Graphs** (Diagrame de Apeluri)

**Locație**: În fiecare pagină de funcție individuală

**Exemplu**:
- Navighează la: `Classes` → `SignalProcessing` → `AddValue()`
- Scroll down până vezi secțiunea **Call Graph**
- Vei vedea:
  - **Call Graph** - Ce funcții apelează `AddValue()`
  - **Caller Graph** - Ce funcții sunt apelate DE `AddValue()`

**Fișiere HTML**:
```
docs/html/class_signal_processing.html#functie_specifica
```

### 2. **Class Diagrams** (Diagrame UML)

**Locație**: În pagina de clasă

**Exemplu**:
- Navighează la: `Classes` → `SignalProcessing`
- Vei vedea diagrama de **Inheritance** și **Collaboration**

**Fișiere**:
```
docs/html/class_signal_processing.html
```

### 3. **Include Dependency Graphs**

**Locație**: În pagina fiecărui fișier

**Exemplu**:
- Navighează la: `Files` → `SignalProcessing.h`
- Vei vedea:
  - **Include dependency graph** - Ce headere include acest fișier
  - **Included by graph** - Ce fișiere includ acest header

**Fișiere**:
```
docs/html/_signal_processing_8h.html
```

### 4. **Directory Graphs**

**Locație**: În pagina de directory structure

**Exemplu**:
- Navighează la: `Files` → `Directories` → `source/`
- Vei vedea structura de directoare ca graf

**Fișiere**:
```
docs/html/dir_*.html
```

---

## Cum Arată Diagramele

### Call Graph Example
```
                    ┌─────────────────┐
                    │   AddValue()    │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              │              │              │
         ┌────▼────┐    ┌───▼────┐   ┌────▼─────┐
         │GetMean()│    │GetMin()│   │UpdateBuffer│
         └─────────┘    └────────┘   └──────────┘
```

### Class Collaboration Diagram
```
   ┌──────────────────┐
   │ SignalProcessing │◄───uses───┐
   └──────────────────┘            │
           │                       │
           │ contains              │
           │                       │
    ┌──────▼────────┐       ┌─────┴──────┐
    │MLFeatureVector│       │  MLDataset │
    └───────────────┘       └────────────┘
```

---

## Status Actual (FĂRĂ Graphviz)

**Fără Graphviz instalat**, documentația conține:
✅ Descrieri complete ale funcțiilor
✅ Parametri și return values
✅ Code examples
✅ Linkuri între componente
❌ Call graphs (diagrame de apeluri)
❌ Class diagrams (UML)
❌ Include graphs
❌ Directory graphs

---

## Instalare Graphviz

### Opțiunea 1: WinGet
```cmd
winget install --id=Graphviz.Graphviz -e
```

### Opțiunea 2: Download Direct
```cmd
install_graphviz.bat
```

### Opțiunea 3: Manual
1. Download: https://graphviz.org/download/
2. Instalează: **graphviz-install-14.1.3-win64.exe**
3. **IMPORTANT**: Bifează "Add Graphviz to PATH" în installer
4. Restart terminal
5. Verificare: `dot -V`

### Opțiunea 4: Scoop
```cmd
scoop install graphviz
```

---

## După Instalarea Graphviz

### 1. Verifică Instalarea
```cmd
dot -V
```

Ar trebui să vezi: `dot - graphviz version 14.1.3`

### 2. Regenerează Documentația
```cmd
.\docs_quick.bat
```

### 3. Deschide Documentația
```cmd
start docs\html\index.html
```

### 4. Navighează la Call Graphs
- Click pe `Classes` în meniu
- Click pe `SignalProcessing`
- Click pe orice funcție (exemplu: `AddValue()`)
- Scroll down - vei vedea secțiunea **Call Graph**

---

## Troubleshooting

### "dot not found" după instalare

**Soluție 1**: Restart terminal
```cmd
# Închide PowerShell/CMD complet
# Redeschide și testează:
dot -V
```

**Soluție 2**: Adaugă manual la PATH
```cmd
# Găsește instalarea:
dir "C:\Program Files\Graphviz*" /s /b

# Adaugă la PATH:
setx PATH "%PATH%;C:\Program Files\Graphviz\bin"
```

**Soluție 3**: Extrage portabil
```cmd
# Download ZIP de pe graphviz.org
# Extrage în: C:\Tools\graphviz
# Adaugă la PATH: C:\Tools\graphviz\bin
```

### Diagramele nu apar după regenerare

**Verifică Doxyfile**:
```
HAVE_DOT = YES
DOT_PATH = 
CALL_GRAPH = YES
CALLER_GRAPH = YES
```

**Regenerează complet**:
```cmd
rmdir /s /q docs
.\docs_quick.bat
```

### Diagrame incomplete sau greșite

**Verifică versiunea Graphviz**:
```cmd
dot -V
# Ar trebui să fie >= 2.44
```

---

## Alternativă: Documentație cu Liste Text

Dacă NU poți instala Graphviz, poți vedea relațiile ca liste:

**În documentația curentă**:
1. Click pe orice funcție
2. Vezi secțiunile:
   - **Called by**: Lista funcțiilor care apelează această funcție
   - **Calls**: Lista funcțiilor apelate de această funcție

**Exemplu pentru `GetMean()`**:
```
Called by:
• ComputeTrainingStats()
• ExtractMLFeatures()
• NormalizeDataset()

Calls:
• (internal buffer access)
```

---

## Exemple de Linkuri Direct

După regenerare cu Graphviz, accesează:

### Call Graph pentru functii populare
```
docs/html/class_signal_processing.html#a1234567890abcdef  (AddValue)
docs/html/class_signal_processing.html#a234567890abcdef1  (ExtractMLFeatures)
docs/html/class_signal_processing.html#a34567890abcdef12  (KalmanFilter)
```

### Class Diagrams
```
docs/html/class_signal_processing.html  (main class page)
docs/html/struct_m_l_feature_vector.html  (MLFeatureVector structure)
docs/html/struct_m_l_dataset.html  (MLDataset structure)
```

### Include Graphs
```
docs/html/_signal_processing_8h.html  (header file)
docs/html/_signal_processing_8cpp.html  (implementation)
```

---

## Resurse Suplimentare

- **Doxygen Graphs Documentation**: https://www.doxygen.nl/manual/diagrams.html
- **Graphviz Gallery**: https://graphviz.org/gallery/
- **Customizing Doxygen Graphs**: Edit `Doxyfile` section "dot options"

---

## Configurare Avansată (Opțional)

Pentru diagrame mai detaliate, editează în `Doxyfile`:

```conf
# Mai multe noduri în grafuri
DOT_GRAPH_MAX_NODES = 100  (default: 50)

# Stil UML
UML_LOOK = YES

# Grafuri SVG interactive
DOT_IMAGE_FORMAT = svg
INTERACTIVE_SVG = YES

# Culori personalizate
DOT_COMMON_ATTR = "fontname=Arial,fontsize=10,fillcolor=lightblue"
```

Apoi regenerează: `.\docs_quick.bat`
