/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "SignalProcessing", "index.html", [
    [ "Main Features", "index.html#autotoc_md1", null ],
    [ "Function Descriptions", "index.html#autotoc_md2", null ],
    [ "How to Build and Test", "index.html#autotoc_md3", [
      [ "Run All Tests Automatically:", "index.html#autotoc_md4", null ]
    ] ],
    [ "Examples", "index.html#autotoc_md5", [
      [ "Console Example (ECG Processing)", "index.html#autotoc_md6", null ],
      [ "🎮 Interactive ImGui Demo (NEW!)", "index.html#autotoc_md7", null ]
    ] ],
    [ "Documentation", "index.html#autotoc_md8", null ],
    [ "Applications", "index.html#autotoc_md9", null ],
    [ "License", "index.html#autotoc_md10", null ],
    [ "Denoising Capabilities", "index.html#autotoc_md11", [
      [ "Kalman Filter", "index.html#autotoc_md12", null ],
      [ "Wavelet Denoising", "index.html#autotoc_md13", null ],
      [ "Median Filter", "index.html#autotoc_md14", null ],
      [ "Noise Estimation", "index.html#autotoc_md15", null ]
    ] ],
    [ "Anomaly Detection for Industrial Applications", "index.html#autotoc_md16", [
      [ "Turbine Blade Monitoring Example", "index.html#autotoc_md17", null ],
      [ "Multiple Anomaly Detection Methods", "index.html#autotoc_md18", null ],
      [ "Segment Statistics", "index.html#autotoc_md19", null ]
    ] ],
    [ "Frequency Analysis Capabilities", "index.html#autotoc_md20", [
      [ "FFT Analysis", "index.html#autotoc_md21", null ],
      [ "Frequency Peak Detection", "index.html#autotoc_md22", null ],
      [ "Harmonic Analysis", "index.html#autotoc_md23", null ],
      [ "Frequency Band Power Analysis", "index.html#autotoc_md24", null ],
      [ "Turbine Blade Frequency Analysis", "index.html#autotoc_md25", null ],
      [ "Frequency-Based Anomaly Detection", "index.html#autotoc_md26", null ],
      [ "Use Cases for Frequency Analysis", "index.html#autotoc_md27", null ]
    ] ],
    [ "Signal Recording with HDF5", "index.html#autotoc_md28", [
      [ "Features", "index.html#autotoc_md29", null ],
      [ "Requirements", "index.html#autotoc_md30", null ],
      [ "Basic Usage", "index.html#autotoc_md31", null ],
      [ "Hierarchical Organization", "index.html#autotoc_md32", null ],
      [ "Multi-Channel Recording", "index.html#autotoc_md33", null ],
      [ "Machine Learning Dataset Creation", "index.html#autotoc_md34", null ],
      [ "Turbine Monitoring Example", "index.html#autotoc_md35", null ],
      [ "Compilation", "index.html#autotoc_md36", null ],
      [ "Viewing HDF5 Files", "index.html#autotoc_md37", null ],
      [ "Python Integration", "index.html#autotoc_md38", null ],
      [ "API Reference", "index.html#autotoc_md39", null ],
      [ "Use Cases", "index.html#autotoc_md40", null ]
    ] ],
    [ "TODO", "index.html#autotoc_md41", null ],
    [ "ML/AI Integration Guide", "md_ML__FEATURES__GUIDE.html", [
      [ "Overview", "md_ML__FEATURES__GUIDE.html#autotoc_md43", null ],
      [ "Conceptul \"Downstream ML/AI\"", "md_ML__FEATURES__GUIDE.html#autotoc_md44", [
        [ "Cazuri de utilizare tipice:", "md_ML__FEATURES__GUIDE.html#autotoc_md45", [
          [ "1. Monitorizare medicală ECG", "md_ML__FEATURES__GUIDE.html#autotoc_md46", null ],
          [ "2. Predictive Maintenance pentru turbine", "md_ML__FEATURES__GUIDE.html#autotoc_md47", null ]
        ] ]
      ] ],
      [ "Feature Vector Structure", "md_ML__FEATURES__GUIDE.html#autotoc_md48", [
        [ "Statistical Features (8)", "md_ML__FEATURES__GUIDE.html#autotoc_md49", null ],
        [ "Frequency Features (9)", "md_ML__FEATURES__GUIDE.html#autotoc_md50", null ],
        [ "Time Domain Features (4)", "md_ML__FEATURES__GUIDE.html#autotoc_md51", null ]
      ] ],
      [ "API Functions", "md_ML__FEATURES__GUIDE.html#autotoc_md52", [
        [ "1. ExtractMLFeatures()", "md_ML__FEATURES__GUIDE.html#autotoc_md53", null ],
        [ "2. ExtractMLFeaturesFromSegment()", "md_ML__FEATURES__GUIDE.html#autotoc_md54", null ],
        [ "3. ExportFeaturesToArray()", "md_ML__FEATURES__GUIDE.html#autotoc_md55", null ],
        [ "4. NormalizeMLFeatures()", "md_ML__FEATURES__GUIDE.html#autotoc_md56", null ]
      ] ],
      [ "Complete Example: ECG Classification", "md_ML__FEATURES__GUIDE.html#autotoc_md57", null ],
      [ "Integration with Popular ML Frameworks", "md_ML__FEATURES__GUIDE.html#autotoc_md58", [
        [ "Python/NumPy Export Pattern", "md_ML__FEATURES__GUIDE.html#autotoc_md59", null ],
        [ "Real-time Classification Pattern", "md_ML__FEATURES__GUIDE.html#autotoc_md60", null ]
      ] ],
      [ "Training Workflow", "md_ML__FEATURES__GUIDE.html#autotoc_md61", [
        [ "Step 1: Collect Training Data", "md_ML__FEATURES__GUIDE.html#autotoc_md62", null ],
        [ "Step 2: Train Model (Python example)", "md_ML__FEATURES__GUIDE.html#autotoc_md63", null ],
        [ "Step 3: Deploy in C++", "md_ML__FEATURES__GUIDE.html#autotoc_md64", null ]
      ] ],
      [ "Feature Importance by Application", "md_ML__FEATURES__GUIDE.html#autotoc_md65", [
        [ "ECG Analysis", "md_ML__FEATURES__GUIDE.html#autotoc_md66", null ],
        [ "Turbine Monitoring", "md_ML__FEATURES__GUIDE.html#autotoc_md67", null ],
        [ "Audio/Speech", "md_ML__FEATURES__GUIDE.html#autotoc_md68", null ]
      ] ],
      [ "Performance Notes", "md_ML__FEATURES__GUIDE.html#autotoc_md69", null ],
      [ "Testing", "md_ML__FEATURES__GUIDE.html#autotoc_md70", null ],
      [ "Downstream ML/AI Integration", "md_ML__FEATURES__GUIDE.html#autotoc_md71", [
        [ "Overview", "md_ML__FEATURES__GUIDE.html#autotoc_md72", null ],
        [ "Dataset Management", "md_ML__FEATURES__GUIDE.html#autotoc_md73", [
          [ "Crearea unui Dataset", "md_ML__FEATURES__GUIDE.html#autotoc_md74", null ],
          [ "Adăugarea de Sample-uri", "md_ML__FEATURES__GUIDE.html#autotoc_md75", null ],
          [ "Eliberarea Memoriei", "md_ML__FEATURES__GUIDE.html#autotoc_md76", null ]
        ] ],
        [ "Rolling Window Processing", "md_ML__FEATURES__GUIDE.html#autotoc_md77", null ],
        [ "Training Statistics & Normalization", "md_ML__FEATURES__GUIDE.html#autotoc_md78", [
          [ "Calculează Statistici din Training Set", "md_ML__FEATURES__GUIDE.html#autotoc_md79", null ],
          [ "Normalizează Dataset (Z-Score)", "md_ML__FEATURES__GUIDE.html#autotoc_md80", null ],
          [ "Normalizează Features Noi (Inference)", "md_ML__FEATURES__GUIDE.html#autotoc_md81", null ]
        ] ],
        [ "CSV Export pentru Python/MATLAB", "md_ML__FEATURES__GUIDE.html#autotoc_md82", null ],
        [ "Batch Processing", "md_ML__FEATURES__GUIDE.html#autotoc_md83", null ],
        [ "Complete Workflow: ECG Classification", "md_ML__FEATURES__GUIDE.html#autotoc_md84", null ],
        [ "Integration Testing", "md_ML__FEATURES__GUIDE.html#autotoc_md85", null ]
      ] ],
      [ "See Also", "md_ML__FEATURES__GUIDE.html#autotoc_md86", null ],
      [ "Framework Integration Resources", "md_ML__FEATURES__GUIDE.html#autotoc_md87", [
        [ "TensorFlow / Keras (Python)", "md_ML__FEATURES__GUIDE.html#autotoc_md88", null ],
        [ "PyTorch (Python)", "md_ML__FEATURES__GUIDE.html#autotoc_md89", null ],
        [ "scikit-learn SVM (Python)", "md_ML__FEATURES__GUIDE.html#autotoc_md90", null ],
        [ "MATLAB Machine Learning Toolbox", "md_ML__FEATURES__GUIDE.html#autotoc_md91", null ]
      ] ]
    ] ],
    [ "Anomaly Detection Guide", "md_ANOMALY__DETECTION__GUIDE.html", [
      [ "Overview", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md93", null ],
      [ "Table of Contents", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md94", null ],
      [ "Core Concepts", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md96", [
        [ "What is an Anomaly?", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md97", null ],
        [ "When to Use Each Method", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md98", null ]
      ] ],
      [ "Detection Methods", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md100", [
        [ "1. Z-Score Method (Statistical Threshold)", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md101", null ],
        [ "2. IQR Method (Interquartile Range)", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md103", null ],
        [ "3. Moving Average Deviation (MAD)", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md105", null ],
        [ "4. Sudden Change Detection", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md107", null ],
        [ "5. Periodic Anomaly Detection", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md109", null ]
      ] ],
      [ "Signal Segmentation", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md111", [
        [ "Turbine Blade Monitoring", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md112", null ],
        [ "SegmentStats Structure", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md113", null ],
        [ "Anomaly Score Methods", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md114", null ]
      ] ],
      [ "Practical Examples", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md116", [
        [ "Example 1: Real-Time Turbine Monitoring", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md117", null ],
        [ "Example 2: Multi-Method Anomaly Detection", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md118", null ],
        [ "Example 3: Trending and Degradation Tracking", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md119", null ]
      ] ],
      [ "Best Practices", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md121", [
        [ "1. Choosing Detection Thresholds", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md122", null ],
        [ "2. Combining Methods", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md123", null ],
        [ "3. Data Preprocessing", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md124", null ],
        [ "4. Validation and Testing", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md125", null ],
        [ "5. Performance Optimization", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md126", null ],
        [ "6. Logging and Alerts", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md127", null ],
        [ "7. Maintenance Recommendations", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md128", null ]
      ] ],
      [ "Troubleshooting", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md130", [
        [ "False Positives", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md131", null ],
        [ "Missed Detections", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md132", null ],
        [ "Performance Issues", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md133", null ]
      ] ],
      [ "Summary", "md_ANOMALY__DETECTION__GUIDE.html#autotoc_md135", null ]
    ] ],
    [ "HDF5 Export for ML/AI - Setup Guide", "md_HDF5__EXPORT__GUIDE.html", [
      [ "Overview", "md_HDF5__EXPORT__GUIDE.html#autotoc_md137", null ],
      [ "Features", "md_HDF5__EXPORT__GUIDE.html#autotoc_md138", [
        [ "1. Export ML Datasets", "md_HDF5__EXPORT__GUIDE.html#autotoc_md139", null ],
        [ "2. Export Training Statistics", "md_HDF5__EXPORT__GUIDE.html#autotoc_md140", null ],
        [ "3. Export Features + Raw Signal", "md_HDF5__EXPORT__GUIDE.html#autotoc_md141", null ]
      ] ],
      [ "Installation", "md_HDF5__EXPORT__GUIDE.html#autotoc_md142", [
        [ "Windows", "md_HDF5__EXPORT__GUIDE.html#autotoc_md143", [
          [ "Option 1: Pre-built Binaries (Recommended)", "md_HDF5__EXPORT__GUIDE.html#autotoc_md144", null ],
          [ "Option 2: Build from Source", "md_HDF5__EXPORT__GUIDE.html#autotoc_md145", null ],
          [ "Compilation", "md_HDF5__EXPORT__GUIDE.html#autotoc_md146", null ]
        ] ],
        [ "Linux", "md_HDF5__EXPORT__GUIDE.html#autotoc_md147", [
          [ "Ubuntu/Debian", "md_HDF5__EXPORT__GUIDE.html#autotoc_md148", null ],
          [ "Fedora/RHEL/CentOS", "md_HDF5__EXPORT__GUIDE.html#autotoc_md149", null ],
          [ "Arch Linux", "md_HDF5__EXPORT__GUIDE.html#autotoc_md150", null ],
          [ "Compilation", "md_HDF5__EXPORT__GUIDE.html#autotoc_md151", null ]
        ] ],
        [ "macOS", "md_HDF5__EXPORT__GUIDE.html#autotoc_md152", null ]
      ] ],
      [ "Using Without HDF5", "md_HDF5__EXPORT__GUIDE.html#autotoc_md153", null ],
      [ "Python Integration", "md_HDF5__EXPORT__GUIDE.html#autotoc_md154", [
        [ "Loading HDF5 Datasets", "md_HDF5__EXPORT__GUIDE.html#autotoc_md155", null ],
        [ "TensorFlow Integration", "md_HDF5__EXPORT__GUIDE.html#autotoc_md156", null ],
        [ "PyTorch Integration", "md_HDF5__EXPORT__GUIDE.html#autotoc_md157", null ]
      ] ],
      [ "MATLAB Integration", "md_HDF5__EXPORT__GUIDE.html#autotoc_md158", null ],
      [ "Example: Complete Workflow", "md_HDF5__EXPORT__GUIDE.html#autotoc_md159", null ],
      [ "Troubleshooting", "md_HDF5__EXPORT__GUIDE.html#autotoc_md160", [
        [ "Error: \"H5Cpp.h: No such file or directory\"", "md_HDF5__EXPORT__GUIDE.html#autotoc_md161", null ],
        [ "Linker Error: \"undefined reference to H5::...\"", "md_HDF5__EXPORT__GUIDE.html#autotoc_md162", null ],
        [ "Functions Return False", "md_HDF5__EXPORT__GUIDE.html#autotoc_md163", null ],
        [ "\"Symbol not found: _H5...\" (macOS)", "md_HDF5__EXPORT__GUIDE.html#autotoc_md164", null ]
      ] ],
      [ "Advantages of HDF5 over CSV", "md_HDF5__EXPORT__GUIDE.html#autotoc_md165", null ],
      [ "See Also", "md_HDF5__EXPORT__GUIDE.html#autotoc_md166", null ]
    ] ],
    [ "LICENSE", "md_examples_2glfw_2LICENSE.html", null ],
    [ "BACKENDS", "md_examples_2imgui_2docs_2BACKENDS.html", [
      [ "Dear ImGui: Backends", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md174", [
        [ "Index", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md175", null ],
        [ "Introduction", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md176", [
          [ "Getting Started", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md177", null ],
          [ "What are Backends?", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md178", null ]
        ] ],
        [ "Using standard Backends", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md179", [
          [ "List of standard Backends", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md180", null ],
          [ "Recommended Backends", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md181", null ]
        ] ],
        [ "Using third-party Backends", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md182", null ],
        [ "Writing your own Backend", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md183", [
          [ "Using a custom engine?", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md184", null ],
          [ "Platform: Implementing your Platform Backend", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md185", null ],
          [ "Rendering: Implementing your RenderDrawData function", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md186", null ],
          [ "Rendering: Adding support for <tt>ImGuiBackendFlags_RendererHasTextures</tt> (1.92+)", "md_examples_2imgui_2docs_2BACKENDS.html#autotoc_md187", null ]
        ] ]
      ] ]
    ] ],
    [ "Contributing Guidelines", "md_examples_2imgui_2docs_2CONTRIBUTING.html", [
      [ "Index", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md189", null ],
      [ "Getting Started & General Advice", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md190", null ],
      [ "'Issues' vs 'Discussions'", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md191", null ],
      [ "How to open an issue", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md192", null ],
      [ "How to open a Pull Request", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md193", null ],
      [ "Copyright / Contributor License Agreement", "md_examples_2imgui_2docs_2CONTRIBUTING.html#autotoc_md194", null ]
    ] ],
    [ "EXAMPLES", "md_examples_2imgui_2docs_2EXAMPLES.html", null ],
    [ "FAQ (Frequently Asked Questions)", "md_examples_2imgui_2docs_2FAQ.html", [
      [ "Index", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md200", null ],
      [ "Q&A: Basics", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md201", null ],
      [ "Q&A: Integration", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md214", null ],
      [ "Q&A: Usage", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md237", null ],
      [ "Q&A: Fonts, Text", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md262", null ],
      [ "Q&A: Concerns", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md276", null ],
      [ "Q&A: Community", "md_examples_2imgui_2docs_2FAQ.html#autotoc_md289", null ]
    ] ],
    [ "FONTS", "md_examples_2imgui_2docs_2FONTS.html", null ],
    [ "TODO", "md_examples_2implot_2TODO.html", null ],
    [ "ConfigDataWriter Integration Guide", "md_examples_2README__CONFIG.html", [
      [ "Overview", "md_examples_2README__CONFIG.html#autotoc_md522", null ],
      [ "Features", "md_examples_2README__CONFIG.html#autotoc_md523", null ],
      [ "What Gets Saved", "md_examples_2README__CONFIG.html#autotoc_md524", [
        [ "Signal Generation", "md_examples_2README__CONFIG.html#autotoc_md525", null ],
        [ "Filters", "md_examples_2README__CONFIG.html#autotoc_md526", null ],
        [ "Anomaly Detection", "md_examples_2README__CONFIG.html#autotoc_md527", null ],
        [ "UI Preferences", "md_examples_2README__CONFIG.html#autotoc_md528", null ]
      ] ],
      [ "Setup Instructions", "md_examples_2README__CONFIG.html#autotoc_md529", [
        [ "1. Clone ConfigDataWriter Library", "md_examples_2README__CONFIG.html#autotoc_md530", null ],
        [ "2. Add to CMakeLists.txt", "md_examples_2README__CONFIG.html#autotoc_md531", null ],
        [ "3. Integration into imgui_demo.cpp", "md_examples_2README__CONFIG.html#autotoc_md532", [
          [ "Include Headers", "md_examples_2README__CONFIG.html#autotoc_md533", null ],
          [ "Add Global Variables (top of file)", "md_examples_2README__CONFIG.html#autotoc_md534", null ],
          [ "In main() function - After ImGui Initialization", "md_examples_2README__CONFIG.html#autotoc_md535", null ],
          [ "In Main Loop - Add UI Element", "md_examples_2README__CONFIG.html#autotoc_md536", null ],
          [ "Before Application Exit - In Cleanup Section", "md_examples_2README__CONFIG.html#autotoc_md537", null ]
        ] ]
      ] ],
      [ "Usage Example", "md_examples_2README__CONFIG.html#autotoc_md538", [
        [ "Generated INI File Format", "md_examples_2README__CONFIG.html#autotoc_md539", null ]
      ] ],
      [ "Manual Configuration Editing", "md_examples_2README__CONFIG.html#autotoc_md540", null ],
      [ "Advanced Usage", "md_examples_2README__CONFIG.html#autotoc_md541", [
        [ "Using ConfigDataWriter Directly", "md_examples_2README__CONFIG.html#autotoc_md542", null ],
        [ "Multiple Configuration Profiles", "md_examples_2README__CONFIG.html#autotoc_md543", null ]
      ] ],
      [ "Troubleshooting", "md_examples_2README__CONFIG.html#autotoc_md544", [
        [ "Configuration Not Loading", "md_examples_2README__CONFIG.html#autotoc_md545", null ],
        [ "Reset to Defaults", "md_examples_2README__CONFIG.html#autotoc_md546", null ],
        [ "Debug Mode", "md_examples_2README__CONFIG.html#autotoc_md547", null ]
      ] ],
      [ "Build and Run", "md_examples_2README__CONFIG.html#autotoc_md548", null ],
      [ "Benefits", "md_examples_2README__CONFIG.html#autotoc_md549", null ],
      [ "See Also", "md_examples_2README__CONFIG.html#autotoc_md550", null ],
      [ "License", "md_examples_2README__CONFIG.html#autotoc_md551", null ]
    ] ],
    [ "SignalProcessing ImGui Interactive Demo", "md_examples_2README__IMGUI.html", [
      [ "Features", "md_examples_2README__IMGUI.html#autotoc_md553", [
        [ "🎛️ Signal Generation", "md_examples_2README__IMGUI.html#autotoc_md554", null ],
        [ "🔧 Filters (Real-time)", "md_examples_2README__IMGUI.html#autotoc_md555", null ],
        [ "⚠️ Anomaly Detection", "md_examples_2README__IMGUI.html#autotoc_md556", null ],
        [ "📈 Analysis Tools", "md_examples_2README__IMGUI.html#autotoc_md557", null ],
        [ "💾 HDF5 Recording", "md_examples_2README__IMGUI.html#autotoc_md558", null ],
        [ "📊 Visualization", "md_examples_2README__IMGUI.html#autotoc_md559", null ]
      ] ],
      [ "Prerequisites", "md_examples_2README__IMGUI.html#autotoc_md560", [
        [ "Linux (Ubuntu/Debian)", "md_examples_2README__IMGUI.html#autotoc_md561", null ],
        [ "macOS", "md_examples_2README__IMGUI.html#autotoc_md562", null ],
        [ "Windows", "md_examples_2README__IMGUI.html#autotoc_md563", null ]
      ] ],
      [ "Build Instructions", "md_examples_2README__IMGUI.html#autotoc_md564", [
        [ "Step 1: Setup (download ImGui and ImPlot)", "md_examples_2README__IMGUI.html#autotoc_md565", null ],
        [ "Step 2: Build", "md_examples_2README__IMGUI.html#autotoc_md566", null ]
      ] ],
      [ "Usage Guide", "md_examples_2README__IMGUI.html#autotoc_md567", [
        [ "Basic Workflow", "md_examples_2README__IMGUI.html#autotoc_md568", null ],
        [ "Example Scenarios", "md_examples_2README__IMGUI.html#autotoc_md569", [
          [ "ECG Analysis", "md_examples_2README__IMGUI.html#autotoc_md570", null ],
          [ "Turbine Monitoring", "md_examples_2README__IMGUI.html#autotoc_md571", null ],
          [ "Filter Comparison", "md_examples_2README__IMGUI.html#autotoc_md572", null ],
          [ "ML Dataset Creation", "md_examples_2README__IMGUI.html#autotoc_md573", null ]
        ] ]
      ] ],
      [ "Code Structure", "md_examples_2README__IMGUI.html#autotoc_md574", null ],
      [ "Customization", "md_examples_2README__IMGUI.html#autotoc_md575", [
        [ "Adding New Signal Types", "md_examples_2README__IMGUI.html#autotoc_md576", null ],
        [ "Adding New Visualizations", "md_examples_2README__IMGUI.html#autotoc_md577", null ],
        [ "Adding New Filters", "md_examples_2README__IMGUI.html#autotoc_md578", null ]
      ] ],
      [ "Keyboard Shortcuts", "md_examples_2README__IMGUI.html#autotoc_md579", null ],
      [ "Performance", "md_examples_2README__IMGUI.html#autotoc_md580", null ],
      [ "Troubleshooting", "md_examples_2README__IMGUI.html#autotoc_md581", [
        [ "Build Errors", "md_examples_2README__IMGUI.html#autotoc_md582", null ],
        [ "Runtime Issues", "md_examples_2README__IMGUI.html#autotoc_md583", null ]
      ] ],
      [ "Screenshots", "md_examples_2README__IMGUI.html#autotoc_md584", [
        [ "Main Interface", "md_examples_2README__IMGUI.html#autotoc_md585", null ],
        [ "Anomaly Detection", "md_examples_2README__IMGUI.html#autotoc_md586", null ],
        [ "FFT Analysis", "md_examples_2README__IMGUI.html#autotoc_md587", null ],
        [ "ML Features", "md_examples_2README__IMGUI.html#autotoc_md588", null ]
      ] ],
      [ "See Also", "md_examples_2README__IMGUI.html#autotoc_md589", null ],
      [ "License", "md_examples_2README__IMGUI.html#autotoc_md590", null ]
    ] ],
    [ "Quick Setup Guide for ImGui Demo on Windows", "md_examples_2SETUP__WINDOWS.html", [
      [ "Problema: GLFW lipsește", "md_examples_2SETUP__WINDOWS.html#autotoc_md592", null ],
      [ "Opțiunea 1: MSYS2 (RECOMANDAT - cel mai simplu)", "md_examples_2SETUP__WINDOWS.html#autotoc_md593", [
        [ "1.1 Instalează MSYS2", "md_examples_2SETUP__WINDOWS.html#autotoc_md594", null ],
        [ "1.2 Instalează dependențele", "md_examples_2SETUP__WINDOWS.html#autotoc_md595", null ],
        [ "1.3 Build", "md_examples_2SETUP__WINDOWS.html#autotoc_md596", null ]
      ] ],
      [ "Opțiunea 2: Visual Studio (fără g++)", "md_examples_2SETUP__WINDOWS.html#autotoc_md597", [
        [ "2.1 Instalează Visual Studio 2022", "md_examples_2SETUP__WINDOWS.html#autotoc_md598", null ],
        [ "2.2 Instalează CMake", "md_examples_2SETUP__WINDOWS.html#autotoc_md599", null ],
        [ "2.3 Build cu Visual Studio", "md_examples_2SETUP__WINDOWS.html#autotoc_md600", null ]
      ] ],
      [ "Opțiunea 3: vcpkg + CMake", "md_examples_2SETUP__WINDOWS.html#autotoc_md601", [
        [ "3.1 Instalează vcpkg", "md_examples_2SETUP__WINDOWS.html#autotoc_md602", null ],
        [ "3.2 Instalează GLFW", "md_examples_2SETUP__WINDOWS.html#autotoc_md603", null ],
        [ "3.3 Instalează CMake", "md_examples_2SETUP__WINDOWS.html#autotoc_md604", null ],
        [ "3.4 Build", "md_examples_2SETUP__WINDOWS.html#autotoc_md605", null ]
      ] ],
      [ "Verificare instalare", "md_examples_2SETUP__WINDOWS.html#autotoc_md606", [
        [ "Verifică g++ (MinGW):", "md_examples_2SETUP__WINDOWS.html#autotoc_md607", null ],
        [ "Verifică CMake:", "md_examples_2SETUP__WINDOWS.html#autotoc_md608", null ],
        [ "Verifică GLFW (după instalare MSYS2):", "md_examples_2SETUP__WINDOWS.html#autotoc_md609", null ]
      ] ],
      [ "Troubleshooting", "md_examples_2SETUP__WINDOWS.html#autotoc_md610", [
        [ "\"g++ not found\"", "md_examples_2SETUP__WINDOWS.html#autotoc_md611", null ],
        [ "\"glfw3 not found\"", "md_examples_2SETUP__WINDOWS.html#autotoc_md612", null ],
        [ "\"cmake not found\"", "md_examples_2SETUP__WINDOWS.html#autotoc_md613", null ]
      ] ],
      [ "Recomandare finală", "md_examples_2SETUP__WINDOWS.html#autotoc_md614", null ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Topics", "topics.html", "topics" ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Functions", "namespacemembers_func.html", "namespacemembers_func" ],
        [ "Variables", "namespacemembers_vars.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", "globals_vars" ],
        [ "Typedefs", "globals_type.html", "globals_type" ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", "globals_eval" ],
        [ "Macros", "globals_defs.html", "globals_defs" ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"ImGuiConfig_8hpp.html",
"functions_func_f.html",
"group__errors.html#gacff24d2757da752ae4c80bf452356487",
"group__keys.html#ga7f70970db6e8be1794da8516a6d14058",
"group__window.html#ga4d766499ac02c60f02221a9dfab87299",
"imgui_2examples_2example__sdl3__vulkan_2main_8cpp.html#a565b7bd1b35474040834a26cf1cba856",
"imgui_2imgui__demo_8cpp.html#a97ab35c8c24ca76e7e46f05e82f8e74a",
"imgui_8h.html#a04ba8fcf00550695cbb71fbcd5d4b376a6f5ec29daacab204c093dff8f8f673d7",
"imgui_8h.html#a3720a69496e9b8cf110a11dfe7d334e6",
"imgui_8h.html#a6724109814c04a3c3d8797dfd893383aac4afe386273fd141a25076a29742649b",
"imgui_8h.html#aa22ffe36b188427d712447ec465203d4a20c574bb154d150a7ce88b38d7d2a2ee",
"imgui_8h.html#ac297462e33309d0540fa459bccdc9304",
"imgui_8h.html#af4be377242ba2480b28b116390af7261",
"imgui__freetype_8cpp.html#af1e1c42812b61159dcbd699a56dbb017",
"imgui__impl__glut_8cpp.html#a1ce7c516538704e5b04a99450edc572f",
"imgui__impl__opengl3__loader_8h.html#a82ac4415f852b8b5bfe7356202ca934e",
"imgui__impl__sdlgpu3_8cpp.html#a06777e812d52acdcc2de477bfa76b43c",
"imgui__internal_8h.html",
"imgui__internal_8h.html#a4216382dd5ad6c17e183f9f894db2e15",
"imgui__internal_8h.html#a7db13485f1945070e9de98facafe7e1d",
"imgui__internal_8h.html#ac2561e31f0de066ef0bb5bfbb541c8ae",
"imgui__internal_8h.html#afad90b366b6471e3b13175c0ebeb26c8",
"implot_8cpp.html#a4d840769e5750b610d6882e771af10e9",
"implot_8h.html#a34694ee881e409bf7ef6ac6009e4c9eaa0b927a12fa4e9ec6913fddee38521468",
"implot_8h.html#ac65218062eb874442f520a0875c4cec4",
"implot__internal_8h.html#a57a6954fad65712e30b4044040914f6c",
"implot__items_8cpp.html#a8d965b26ca991dccd1b089ee172daf33",
"md_ANOMALY__DETECTION__GUIDE.html#autotoc_md93",
"namespaceImGui.html#a0a3c410ca6f4423d08872cb2505e8a00",
"namespaceImGui.html#a4d1cc0ddb0d66bb46cdbea1923b88e9c",
"namespaceImGui.html#a98124479b099e75aa364cb1726e3dca6",
"namespaceImGui.html#ae0a8de0d3286104b818a521b74d8d0ff",
"namespaceImPlot.html#a3f2dc1abf0772d69ba1f288a53bbb9b7",
"namespaceImPlot.html#ab600394f23e5d5374c072f477efd58df",
"structExampleAppDocuments.html",
"structImDrawList.html#a6282e85ad78b01ebb74832336e9a2a55",
"structImFontBaked.html#a42c8a11e68fd31a3dba9d676a6a3275c",
"structImGuiContext.html#a5e31703c42698a572be557ec57c5cbe7",
"structImGuiDemoWindowData.html#aa8ec2d29e1a6155bea4b6d51211205d0",
"structImGuiInputTextState.html#ab17832413ff121a5663319c06bbb989a",
"structImGuiPlatformIO.html#a9ad7642f273fbf9064f962db7c8b4bea",
"structImGuiTabItem.html",
"structImGuiTextBuffer.html#a9e4edc9f950bc9e35d5c3b3071dbc0df",
"structImGuiWindowTempData.html#ac5ffaaf397b8501b523aa55aa7d34d3d",
"structImGui__ImplSDLGPU3__InitInfo.html#ac5f31eaa10d1c3a198245631b47a7814",
"structImPlotAxis.html#ad06cff36e02f602cf9837849034c1115",
"structImPlotStyle.html#a07bcb1223205b2e5632c0b43c39936e2",
"structImPlot_1_1GetterLoop.html#a6540627fe4c145ed99deecb09fa17fcb",
"structImPlot_1_1RendererLineSegments2.html#a6ac8c7016501843aa8bf4a6a2ea873c9",
"structImPlot_1_1Transformer2.html#a15770902769b84474eda7198e75fddd5",
"structImVector.html#aec87a57a48b7c07e636f338b6113935b",
"structstbtt__packedchar.html#a3a33880f925ca826c908cbf9f0673c9f"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';