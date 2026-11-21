# Beauty Skin Retouching — ColorSuite OFX (GitHub build project)

This repository contains the existing C++ OFX starter project plus a Windows x64 GitHub Actions workflow. The workflow checks out the official OpenFX SDK headers, compiles the current source with CMake/MSVC, assembles the `.ofx.bundle`, and builds the Inno Setup installer.

## Build on GitHub

1. Create a new repository on GitHub.
2. Upload the **contents of this project folder** (the folder containing `CMakeLists.txt`, `src/`, `installer/`, and `.github/`) to the repository's `main` branch.
3. Open the repository's **Actions** tab and enable Actions if GitHub asks.
4. Select **Build ColorSuite OFX (Windows x64)**, click **Run workflow**, and run it on `main`.
5. Wait for the workflow to finish. A green check means the compile and packaging steps completed.
6. Open that workflow run and download the **ColorSuite-Windows-x64** artifact.
7. Extract `ColorSuite_Windows_x64.zip`. It contains the OFX bundle and `ColorSuite_Setup_x64.exe`.

You can also trigger a build by pushing a commit to `main` or `master`.

## Local Windows build

Requirements: Visual Studio 2022 with Desktop development with C++, CMake 3.20+, the official OpenFX SDK headers, and Inno Setup 6.

```powershell
cmake -S . -B build -A x64 -DOPENFX_INCLUDE_DIR="C:/path/to/openfx/include"
cmake --build build --config Release
& "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe" "installer/ColorSuiteInstaller.iss"
```

## Current implementation status — read before use

This package is a **buildable starter, not a finished Magic Bullet Looks replacement**. The existing C++ source has CPU image processing and controls for basic adjustments, RGB matrix, color saturation, approximate hue/saturation/luma selection, skin softening, and advanced RGB controls.

The supplied PDF also specifies the following work, which is not fully implemented in this starter and must not be considered delivered just because CI builds:

- A polished, complete, multi-panel Looks-style interface (the current source exposes OFX host parameters; it does not create a custom dockable UI).
- Fifteen completed cinematic look recipes, including the six named additions: Blockbuster Orange, Wedding Soft Pastel, Kodak Warm Film, Matte Black, Cyber Teal-Magenta, and Cold Thriller.
- S-Log3 to Rec.709 conversion: Neutral plus six creative looks, gamut selection, and exposure compensation, with conversion before the creative look stage.
- The PDF's AI Subject/Sky selection and OpenFX frame-row orientation fixes. Current selection is an approximate color-range mask, not AI segmentation.
- Full validation in EDIUS 11 / OFX Bridge with real footage, different frame sizes, pixel formats, and render tests.

The GitHub workflow only proves that the current source compiles and that the bundle/installer can be packaged on its Windows runner. It does **not** prove EDIUS compatibility or completion of the missing features. Install and test on a separate test system/project before production use.

## Project layout

- `src/ColorSuite.cpp` — C++ OFX effect source
- `CMakeLists.txt` — Windows x64 build configuration
- `.github/workflows/windows-build.yml` — automatic GitHub build
- `installer/ColorSuiteInstaller.iss` — Inno Setup installer recipe
- `docs/` — six-page control specification and PDF-derived update notes
