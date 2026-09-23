
# 🎲 Dice Roller Studio

A cross-platform dice simulator written in C++17. 
This project goes beyond simple random number generation; it features a custom analytical engine capable of calculating and visualizing the exact probability mass function (PMF) for any combination of dice (e.g., `4d6 + 2d8 + 5`) using discrete convolution algorithms. 

The application is built with a strict Model-View architecture, ensuring a highly scalable and maintainable codebase.

## Features

* **Analytical Math Engine:** Real-time calculation of Expected Value (Mean), Standard Deviation, and the exact probability of every single outcome.
* **Dynamic Pool Builder:** Add dice of any type and modifiers to create complex roll formulas.
* **Graphical User Interface (GUI):** Fluid, responsive, full-screen UI built with Dear ImGui.
* **Data Visualization:** Instant generation of the exact probability distribution chart (bell curve) using ImPlot.
* **Clean Architecture:** Strict separation of concerns (Model-View pattern). The UI logic is completely encapsulated within its own dedicated class.
* **Included Test Suite:** Dedicated testing executable to validate mathematical accuracy and memory management.

---

## Tech Stack

* **Language:** C++17
* **Build System:** CMake
* **GUI:** Dear ImGui & ImPlot
* **Window Management:** GLFW3
* **Rendering:** OpenGL 3

---

## Dependencies and Requirements

To build and run **Dice Roller Studio**, your system must meet the following requirements:

* **Compiler:** Any C++ compiler supporting **C++17** (e.g., GCC, Clang, MSVC).
* **Build System:** **CMake** (version 3.10 or higher).
* **System Libraries:** **GLFW3** and **OpenGL 3** for window management and rendering.
* **Internal Libraries:** **Dear ImGui** and **ImPlot** (Already bundled in the `external/` folder, no separate installation required).

### Linux (Debian/Ubuntu)
You can install all necessary system dependencies and build tools with a single command:
```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev

*(Note: The code is natively compatible with Windows and macOS, provided CMake and GLFW are installed via their respective package managers like vcpkg or Homebrew).*

```

### Windows /  macOS
Ensure you have a modern C++ compiler and CMake installed. You will also need to provide the GLFW library using your preferred package manager (such as vcpkg for Windows or brew install glfw for macOS).

## How to Build and Run

The project uses CMake for a simple and standardized build process. Open the terminal in the project's root folder and run:

**1. Generate build files:**

```bash
cmake -S . -B build

```

**2. Compile the project:**

```bash
cmake --build build

```

**3. Run the application:**

```bash
./build/DiceThrower

```

---

## Running Tests

The project includes a secondary executable that validates the robustness of the math engine (`DiceMath`) in isolation, without loading the GUI. To run the tests:

```bash
./build/TestDiceThrower

```

If the terminal outputs `SUCCESS!`, all probability distributions, expected values, and standard deviation calculations are validated and correct.

---

## Code Structure

* `src/math/` (**Model**): Contains the logic and math engine (`DiceMath.h/cpp`). Completely agnostic of the platform and GUI.
* `src/gui/` (**View**): Contains the `DiceApp` class (`DiceApp.h/cpp`) which encapsulates the window lifecycle, UI state, and rendering logic.
* `external/`: Contains third-party source files (Dear ImGui and ImPlot).
* `main.cpp`: A minimal, clean entry point that simply initializes and runs the `DiceApp`.

