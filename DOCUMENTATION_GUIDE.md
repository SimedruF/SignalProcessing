# Documentation Generation Guide

## Opțiuni pentru Generarea Documentației

### 1. **Doxygen** (Recomandat) ⭐

**Ce este:**
- Standard pentru C/C++
- Generează documentație HTML, PDF, LaTeX din comentarii
- Creează diagrame UML și call graphs automate

**Instalare:**

```bash
# Windows
choco install doxygen graphviz

# Linux Ubuntu/Debian
sudo apt-get install doxygen graphviz

# Linux Fedora/RHEL
sudo dnf install doxygen graphviz

# macOS
brew install doxygen graphviz
```

**Utilizare:**
```bash
# Windows
generate_docs.bat

# Linux/macOS
./generate_docs.sh
```

**Output:** `docs/html/index.html`

### 2. **Sphinx + Breathe** (Pentru Documentație Avansată)

**Ce este:**
- Același sistem folosit de Python (ReadTheDocs)
- Combină C++ API docs cu documentație narrative
- Suport excelent pentru Markdown și reStructuredText

**Instalare:**
```bash
pip install sphinx breathe sphinx-rtd-theme
```

**Setup:**
```bash
sphinx-quickstart
# Edit conf.py to add breathe extension
breathe_projects = {"SignalProcessing": "./doxyxml"}
```

**Avantaje:**
- Aspect profesional tip "ReadTheDocs"
- Mai flexibil pentru tutoriale și ghiduri
- Versionare built-in

### 3. **Hdoc** (Modern C++ Documentation)

**Ce este:**
- Tool modern specializat pe C++
- Output HTML foarte rapid
- Sintaxă simplificată

**Instalare:**
```bash
cargo install hdoc
```

### 4. **Natural Docs**

**Ce este:**
- Sintaxă mai naturală decât Doxygen
- Multi-limbaj
- Output HTML elegant

**Instalare:**
```
Download from: https://www.naturaldocs.org/
```

## Structura Comentariilor Doxygen

### Format Recomandat

```cpp
/**
 * @brief Scurtă descriere (o linie)
 * 
 * Descriere detaliată poate să fie pe mai multe linii.
 * Aici pui toate detaliile importante.
 * 
 * @param param1 Descriere parametru 1
 * @param param2 Descriere parametru 2
 * @return Descriere valoare returnată
 * 
 * @note Note speciale
 * @warning Avertismente importante
 * @see FuncțieRelatată()
 * 
 * @code
 * // Exemplu de utilizare
 * SignalProcessing sp;
 * sp.AddValue(10.5);
 * @endcode
 */
double MyFunction(int param1, double param2);
```

### Tag-uri Utile Doxygen

| Tag | Descriere |
|-----|-----------|
| `@brief` | Descriere scurtă (o linie) |
| `@param` | Documentează un parametru |
| `@return` | Documentează valoarea returnată |
| `@note` | Notă informativă |
| `@warning` | Avertisment important |
| `@deprecated` | Marchează funcție deprecată |
| `@see` | Referință la alte funcții |
| `@code` | Bloc de cod exemplu |
| `@example` | Fișier exemplu complet |
| `@todo` | Task de implementat |
| `@bug` | Bug cunoscut |

### Exemple Comentarii pentru Structuri

```cpp
/**
 * @brief Feature vector pentru ML/AI
 * 
 * Structură care conține 21 de features extrase dintr-un semnal
 * pentru utilizare în machine learning.
 * 
 * @see SignalProcessing::ExtractMLFeatures()
 */
typedef struct MLFeatureVector
{
    double mean;        ///< Media semnalului
    double std_dev;     ///< Abaterea standard
    double variance;    ///< Varianța
    // ...
} MLFeatureVector;
```

## Comparație Tooluri

| Feature | Doxygen | Sphinx+Breathe | Hdoc | Natural Docs |
|---------|---------|----------------|------|--------------|
| C++ Support | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| Output HTML | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| UML Diagrams | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐ |
| Learning Curve | Medium | High | Low | Low |
| Customization | High | Very High | Medium | Medium |
| PDF Export | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐ | ⭐⭐ |

## Generarea Diferitelor Output-uri

### HTML (Default)
```bash
doxygen Doxyfile
# Output: docs/html/index.html
```

### PDF via LaTeX
```bash
# Edit Doxyfile:
# GENERATE_LATEX = YES

doxygen Doxyfile
cd docs/latex
make pdf
```

### XML (pentru Sphinx/Breathe)
```bash
# Edit Doxyfile:
# GENERATE_XML = YES

doxygen Doxyfile
# Output: docs/xml/
```

### Man Pages (Linux)
```bash
# Edit Doxyfile:
# GENERATE_MAN = YES

doxygen Doxyfile
# Output: docs/man/
```

## Integrare CI/CD

### GitHub Actions

Creează `.github/workflows/documentation.yml`:

```yaml
name: Generate Documentation

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  docs:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install Dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y doxygen graphviz
    
    - name: Generate Documentation
      run: doxygen Doxyfile
    
    - name: Deploy to GitHub Pages
      if: github.event_name == 'push' && github.ref == 'refs/heads/main'
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./docs/html
```

### GitLab CI

Creează `.gitlab-ci.yml`:

```yaml
pages:
  stage: deploy
  image: alpine:latest
  before_script:
    - apk add --no-cache doxygen graphviz ttf-freefont
  script:
    - doxygen Doxyfile
    - mv docs/html public
  artifacts:
    paths:
      - public
  only:
    - main
```

## Hosting Online

### 1. GitHub Pages
```bash
# Activează în Settings → Pages → Source: gh-pages branch
```

### 2. Read the Docs
```bash
# Creează cont pe readthedocs.org
# Link repository
# Configure webhook
```

### 3. GitLab Pages
```bash
# Automat dacă ai .gitlab-ci.yml cu job "pages"
```

### 4. Netlify
```bash
# Drag & drop docs/html folder
# Sau connect git repository
```

## Best Practices

### 1. **Comentează Consistent**
- Toate funcțiile publice trebuie documentate
- Parametrii și return values trebuie explicați
- Folosește @brief pentru rezumat

### 2. **Adaugă Exemple de Cod**
```cpp
/**
 * @brief Calculează media
 * 
 * @code
 * SignalProcessing sp;
 * sp.AddValue(10);
 * sp.AddValue(20);
 * double mean = sp.GetMean(); // Returns 15.0
 * @endcode
 */
```

### 3. **Organizează cu Groupuri**
```cpp
/**
 * @defgroup ml_functions ML/AI Functions
 * @{
 */

// Funcții ML/AI aici...

/** @} */ // end of ml_functions
```

### 4. **Include Diagrame**
```cpp
/**
 * @dot
 * digraph workflow {
 *   "Raw Signal" -> "Preprocessing";
 *   "Preprocessing" -> "Feature Extraction";
 *   "Feature Extraction" -> "ML Model";
 * }
 * @enddot
 */
```

### 5. **Link la Alte Resurse**
```cpp
/**
 * @see ML_FEATURES_GUIDE.md
 * @see test_ml_features.cpp
 * @see https://www.example.com/tutorial
 */
```

## Troubleshooting

### Probleme Comune

**1. Graphviz diagrams nu se generează**
```bash
# Verifică instalare
dot -V

# Verifică în Doxyfile
HAVE_DOT = YES
```

**2. Comentariile nu apar**
```bash
# Verifică în Doxyfile
EXTRACT_ALL = YES
JAVADOC_AUTOBRIEF = YES
```

**3. Encoding issues**
```bash
# Setează în Doxyfile
DOXYFILE_ENCODING = UTF-8
INPUT_ENCODING = UTF-8
```

## Resurse Adiționale

- **Doxygen Manual**: https://www.doxygen.nl/manual/
- **Doxygen Special Commands**: https://www.doxygen.nl/manual/commands.html
- **Sphinx Documentation**: https://www.sphinx-doc.org/
- **Breathe**: https://breathe.readthedocs.io/
- **ReadTheDocs**: https://docs.readthedocs.io/

## Concluzie

Pentru proiectul **SignalProcessing**, recomandarea este:

✅ **Doxygen** - Pentru că:
- Deja ai comentarii bune în cod
- Generează quick start guide automat
- UML diagrams pentru structura clasei
- Standard industrie pentru C++
- Setup rapid (Doxyfile inclus)

Rulează pur și simplu:
```bash
generate_docs.bat  # Windows
./generate_docs.sh # Linux/macOS
```

Și documentația ta interactivă va fi gata în `docs/html/index.html`! 🚀
