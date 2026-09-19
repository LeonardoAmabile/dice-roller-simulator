cmake_minimum_required(VERSION 3.15)
project(DiceThrower)

# Richiedi C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Trova le librerie di sistema per la grafica (GLFW e OpenGL) ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# 2. Definisci i file sorgente di Dear ImGui
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_impl_glfw.cpp
    external/imgui/imgui_impl_opengl3.cpp
)

# --- NUOVO: File sorgente di ImPlot ---
set(IMPLOT_SOURCES
    external/implot/implot.cpp
    external/implot/implot_items.cpp
)

# 3. L'eseguibile principale (Aggiungi ${IMPLOT_SOURCES})
add_executable(DiceThrower main.cpp src/math/DiceMath.cpp src/gui/DiceApp.cpp ${IMGUI_SOURCES} ${IMPLOT_SOURCES})

# Diciamo al compilatore dove trovare i file .h di ImGui E ImPlot!
target_include_directories(DiceThrower PRIVATE external/imgui external/implot)

# Colleghiamo le librerie grafiche del sistema al nostro programma
target_link_libraries(DiceThrower PRIVATE glfw OpenGL::GL)


# --- 4. L'eseguibile dei test (Logica pura) ---
# I test rimangono "ciechi": non sanno nulla di finestre o ImGui, testano solo la matematica!
add_executable(TestDiceThrower src/test/test.cpp src/math/DiceMath.cpp)
