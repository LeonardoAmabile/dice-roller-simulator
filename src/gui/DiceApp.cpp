#include "DiceApp.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

DiceApp::DiceApp(int w, int h, const std::string& t) 
    : window(nullptr), width(w), height(h), title(t) {}

DiceApp::~DiceApp() {
    // PHASE 4: CLEANUP (Executed automatically on close)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool DiceApp::initialize() {
    // PHASE 1: SETUP AND INITIALIZATION
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return false;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) return false;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr; // Deactivate the creation of .ini file
    
    ImGui::StyleColorsDark();
    
    // Rounded style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.ItemSpacing = ImVec2(10, 8);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void DiceApp::run() {
    // PHASE 3: GRAPHIC LOOP
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderUI(); // Draw the interface

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void DiceApp::renderUI() {
    // Fullscreen layout
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Dice Builder", nullptr, window_flags); 
    
    // --- TOP SECTION: 2-COLUMN INVISIBLE TABLE ---
    if (ImGui::BeginTable("TopSection", 2, ImGuiTableFlags_None)) {
        // Setup columns: Left takes remaining space, Right is fixed to 320 pixels
        ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, 320.0f);
        ImGui::TableNextRow();
        
        // --- LEFT COLUMN: MANUAL FORMULA ---
        ImGui::TableNextColumn();
        ImGui::Text("1. Build your formula");
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Formula: %s", attackRoll.getFormulaText().c_str());
        ImGui::Spacing();

        ImGui::PushItemWidth(100); 
        ImGui::InputInt("Quantity", &inputQuantity);
        ImGui::SameLine(); 
        ImGui::InputInt("Faces", &inputFaces);
        ImGui::PopItemWidth(); 

        if (inputQuantity < 1) inputQuantity = 1;
        if (inputFaces < 2) inputFaces = 2;

        if (ImGui::Button("Add Custom", ImVec2(120, 30))) {
            attackRoll.addDices(inputQuantity, inputFaces);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Pool", ImVec2(120, 30))) {
            attackRoll.clear();
            attackRoll.setModifier(inputModifier);
            hasRolled = false;
        }

        ImGui::Spacing();
        ImGui::PushItemWidth(100);
        if (ImGui::InputInt("Total Modifier", &inputModifier)) {
            attackRoll.setModifier(inputModifier);
        }
        ImGui::PopItemWidth();

        // --- RIGHT COLUMN: QUICK ADD BUTTONS ---
        ImGui::TableNextColumn();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Quick Add (+1 Dice)");
        ImGui::Spacing();
        
        // Define uniform button size
        ImVec2 btnSize(65, 40);
        
        // First row of standard dice
        if (ImGui::Button("d4", btnSize)) attackRoll.addDices(1, 4); ImGui::SameLine();
        if (ImGui::Button("d6", btnSize)) attackRoll.addDices(1, 6); ImGui::SameLine();
        if (ImGui::Button("d8", btnSize)) attackRoll.addDices(1, 8); ImGui::SameLine();
        if (ImGui::Button("d10", btnSize)) attackRoll.addDices(1, 10);
        
        ImGui::Spacing(); // Moves to the next line of buttons
        
        // Second row of standard dice
        if (ImGui::Button("d12", btnSize)) attackRoll.addDices(1, 12); ImGui::SameLine();
        if (ImGui::Button("d20", btnSize)) attackRoll.addDices(1, 20); ImGui::SameLine();
        if (ImGui::Button("d100", btnSize)) attackRoll.addDices(1, 100);

        ImGui::EndTable();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("2. Analysis & Roll");
    ImGui::Spacing();

    ImGui::Text("Expected Value (Mean): %.2f", attackRoll.getExpectedValue());
    ImGui::Text("Standard Deviation: %.2f", attackRoll.getStandardDeviation());
    ImGui::Spacing();

    // ImPlot Graph
    Distribution dist = attackRoll.getExactDistribution();
    std::vector<double> x_data, y_data;
    for (size_t i = 0; i < dist.probabilities.size(); ++i) {
        x_data.push_back(dist.minValue + i);
        y_data.push_back(dist.probabilities[i] * 100.0);
    }

    if (!x_data.empty()) {
        if (ImPlot::BeginPlot("Probability Distribution", ImVec2(-1, 250))) {
            ImPlot::SetupAxes("Roll Result", "Probability (%)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 100.0, ImPlotCond_Once);
            ImPlot::PlotBars("Exact Prob.", x_data.data(), y_data.data(), (int)x_data.size(), 0.8);
            ImPlot::EndPlot();
        }
    }

    ImGui::Spacing();

    // Roll Button & Log Generation
    if (ImGui::Button("Roll Dice!", ImVec2(200, 50))) {
        lastRoll = attackRoll.rollTotal(mainRoller);
        
        // Build the log string
        std::string logEntry = "Formula: " + attackRoll.getFormulaText() + "  ->  Rolls: ";
        for (int r : lastRoll.individualRolls) {
            logEntry += "[" + std::to_string(r) + "] ";
        }
        logEntry += " ->  TOTAL: " + std::to_string(lastRoll.total);
        
        // Add to history vector
        rollHistory.push_back(logEntry);
        hasRolled = true; // Trigger auto-scroll
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Roll History");

    // Scrollable History Area
    if (ImGui::BeginChild("HistoryRegion", ImVec2(0, 0), true)) {
        
        // Print all past rolls
        for (const auto& log : rollHistory) {
            ImGui::TextUnformatted(log.c_str());
        }
        
        // Auto-scroll to bottom on new roll
        if (hasRolled) {
            ImGui::SetScrollHereY(1.0f);
            hasRolled = false;
        }
    }
    ImGui::EndChild();

    ImGui::End();
}