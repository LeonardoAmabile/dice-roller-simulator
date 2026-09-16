#include <iostream>
#include <string>

// --- GRAPHICS LIBRARIES ---
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// --- YOUR LOGIC ENGINE ---
#include "src/math/DiceMath.h"

// Safety callback to catch any GLFW errors
static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // ========================================================================
    // 1. SETUP AND INITIALIZATION
    // ========================================================================
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // Specify OpenGL version (GLSL 130 is standard on Linux)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create the operating system window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dice Roller Studio", nullptr, nullptr);
    if (window == nullptr) return 1;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync (limits FPS to monitor refresh rate)

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Set a clean dark theme
    ImGui::StyleColorsDark();

    // Connect ImGui to GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ========================================================================
    // 2. GUI STATE VARIABLES (Persist outside the loop)
    // ========================================================================
    DiceRoller mainRoller;
    DicePool attackRoll;
    
    // User interface inputs
    int inputQuantity = 1;
    int inputFaces = 6;
    int inputModifier = 0;
    RollResult lastRoll;
    bool hasRolled = false; // Tracks whether to display roll results

    // ========================================================================
    // 3. GRAPHIC LOOP (Runs until the window is closed)
    // ========================================================================
    while (!glfwWindowShouldClose(window)) {
        // Poll input events (mouse, keyboard, window actions)
        glfwPollEvents();

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- UI LAYOUT ---
        ImGui::Begin("Dice Builder"); 
        
        ImGui::Text("1. Build your formula");
        
        // Print the current combined formula in yellow
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Formula: %s", attackRoll.getFormulaText().c_str());
        ImGui::Spacing();

        // Compact input fields layout
        ImGui::PushItemWidth(100); 
        ImGui::InputInt("Quantity", &inputQuantity);
        ImGui::SameLine(); 
        ImGui::InputInt("Faces", &inputFaces);
        ImGui::PopItemWidth(); 

        // Safety bounds check
        if (inputQuantity < 1) inputQuantity = 1;
        if (inputFaces < 2) inputFaces = 2;

        // Button to add dice group
        if (ImGui::Button("Add Dice", ImVec2(120, 30))) {
            attackRoll.addDices(inputQuantity, inputFaces);
        }

        ImGui::SameLine();
        
        // Button to clear the pool
        if (ImGui::Button("Clear Pool", ImVec2(120, 30))) {
            attackRoll.clear();
            attackRoll.setModifier(inputModifier); // Keep current modifier
            hasRolled = false;
        }

        ImGui::Spacing();
        
        // Modifier updates in real-time
        if (ImGui::InputInt("Total Modifier", &inputModifier)) {
            attackRoll.setModifier(inputModifier);
        }

        ImGui::Separator();

        // ---------------------------------------------
        // Statistics and Rolling Section
        ImGui::Text("2. Analysis & Roll");
        ImGui::Spacing();

        // Real-time analytical stats
        ImGui::Text("Expected Value (Mean): %.2f", attackRoll.getExpectedValue());
        ImGui::Text("Standard Deviation: %.2f", attackRoll.getStandardDeviation());
        
        ImGui::Spacing();

        if (ImGui::Button("Roll Dice!", ImVec2(200, 50))) {
            lastRoll = attackRoll.rollTotal(mainRoller);
            hasRolled = true;
        }

        // --- RESULTS DISPLAY LOGIC ---
        if (hasRolled) {
            ImGui::Spacing();
            ImGui::Text("Individual Rolls: ");
            
            // Print individual dice side-by-side on the same line
            for (size_t i = 0; i < lastRoll.individualRolls.size(); ++i) {
                ImGui::SameLine();
                ImGui::Text("[%d]", lastRoll.individualRolls[i]);
            }
            
            ImGui::Spacing(); // Move to the next line
            
            // Print total clearly in a highlighted color
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "TOTAL: %d", lastRoll.total);
        }

        ImGui::End(); // End of ImGui window
        // ---------------------------------------------

        // Render frame data
        ImGui::Render();
        
        // Clear screen with dark gray background
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw ImGui data onto OpenGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers to display the rendered frame
        glfwSwapBuffers(window);
    }

    // ========================================================================
    // 4. CLEANUP (Executed on exit)
    // ========================================================================
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
