# Building on macOS (Apple Silicon)

This project requires a few dependencies to be built on macOS.

## Prerequisites

1.  **Xcode Command Line Tools**: Ensure you have a C++ compiler (clang).
    ```bash
    xcode-select --install
    ```

2.  **CMake** (>= 3.5) and **Ninja** (optional, but recommended).
    ```bash
    brew install cmake ninja
    ```

3.  **libelf**: The project depends on `libelf`.
    ```bash
    brew install libelf
    ```

4.  **pkg-config** (Optional): Helps CMake find libraries.
    ```bash
    brew install pkg-config
    ```

## Environment

Ensure Homebrew's paths are in your environment (standard on Apple Silicon):
```bash
export PATH="/opt/homebrew/bin:$PATH"
export CPATH="/opt/homebrew/include:$CPATH"
export LIBRARY_PATH="/opt/homebrew/lib:$LIBRARY_PATH"
```

## Build Steps

1.  **Configure**:
    ```bash
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    ```

2.  **Build**:
    ```bash
    cmake --build build
    ```

3.  **Test**:
    ```bash
    ctest --test-dir build
    ```

