#!/bin/bash
# Generate Doxygen documentation for SignalProcessing library

echo "===================================="
echo "SignalProcessing Documentation Build"
echo "===================================="
echo ""

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "ERROR: Doxygen not found!"
    echo ""
    echo "Please install Doxygen:"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen graphviz"
    echo "  Fedora/RHEL:   sudo dnf install doxygen graphviz"
    echo "  macOS:         brew install doxygen graphviz"
    echo ""
    exit 1
fi

echo "Doxygen found:"
doxygen --version
echo ""

# Check if Graphviz is installed (optional but recommended)
if ! command -v dot &> /dev/null; then
    echo "WARNING: Graphviz not found - diagrams will not be generated"
    echo "  Install with your package manager (graphviz)"
    echo ""
else
    echo "Graphviz found - UML diagrams will be generated"
    echo ""
fi

# Clean previous documentation
if [ -d "docs" ]; then
    echo "Cleaning previous documentation..."
    rm -rf docs
fi

# Generate documentation
echo "Generating documentation..."
doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo ""
    echo "===================================="
    echo "Documentation generated successfully!"
    echo "===================================="
    echo ""
    echo "Output location: docs/html/index.html"
    echo ""
    
    # Try to open in browser (platform-specific)
    if command -v xdg-open &> /dev/null; then
        echo "Opening documentation in browser..."
        xdg-open docs/html/index.html &
    elif command -v open &> /dev/null; then
        echo "Opening documentation in browser..."
        open docs/html/index.html
    else
        echo "Open docs/html/index.html in your browser to view the documentation."
    fi
else
    echo ""
    echo "ERROR: Documentation generation failed!"
    echo "Check the warnings above."
    exit 1
fi

echo ""
