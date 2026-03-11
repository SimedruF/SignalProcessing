# SignalProcessing Documentation

This folder contains the generated documentation for the SignalProcessing library.

## Generating Documentation

### Prerequisites

**Windows:**
```cmd
choco install doxygen graphviz
```

**Linux:**
```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz

# Fedora/RHEL
sudo dnf install doxygen graphviz
```

**macOS:**
```bash
brew install doxygen graphviz
```

### Build Documentation

**Windows:**
```cmd
generate_docs.bat
```

**Linux/macOS:**
```bash
chmod +x generate_docs.sh
./generate_docs.sh
```

### Manual Build

```bash
doxygen Doxyfile
```

Output will be in `docs/html/index.html`

## Documentation Structure

The generated documentation includes:

- **Class Documentation** - Full API reference for SignalProcessing class
- **Struct Reference** - All data structures (MLFeatureVector, MLDataset, etc.)
- **Function Reference** - Detailed documentation for all methods
- **Code Examples** - From examples/ and test/ folders
- **Guides** - ML_FEATURES_GUIDE, ANOMALY_DETECTION_GUIDE, HDF5_EXPORT_GUIDE
- **Call Graphs** - Function call relationships (requires Graphviz)
- **Class Diagrams** - UML diagrams (requires Graphviz)

## Viewing Documentation

Open `docs/html/index.html` in your web browser.

## Customization

Edit `Doxyfile` to customize:
- Output formats (HTML, PDF, XML, etc.)
- Documentation style
- Include/exclude patterns
- Graph generation options

## Online Hosting

You can host the documentation on:
- **GitHub Pages** - Enable in repository settings
- **Read the Docs** - For more advanced documentation
- **GitLab Pages** - Similar to GitHub Pages
- Any static web hosting service

### GitHub Pages Example

```bash
# Add docs/html to .gitignore
echo "docs/" >> .gitignore

# Create gh-pages branch
git checkout --orphan gh-pages
git rm -rf .
cp -r docs/html/* .
git add .
git commit -m "Deploy documentation"
git push origin gh-pages

# Enable GitHub Pages in repository settings
# Select gh-pages branch as source
```

## CI/CD Integration

### GitHub Actions

Create `.github/workflows/docs.yml`:

```yaml
name: Documentation

on:
  push:
    branches: [ main ]

jobs:
  docs:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    
    - name: Install Doxygen
      run: sudo apt-get install doxygen graphviz
      
    - name: Generate Documentation
      run: doxygen Doxyfile
      
    - name: Deploy to GitHub Pages
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./docs/html
```

## Notes

- Documentation is generated from comments in source files
- Use `@brief`, `@param`, `@return` tags for better documentation
- Markdown files are included as additional documentation pages
- Test files are excluded by default (configurable in Doxyfile)
