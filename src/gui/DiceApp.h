#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include "../math/DiceMath.h"

class DiceApp {
private:
    GLFWwindow* window;
    int width, height;
    std::string title;

    // Variabili di stato della GUI
    DiceRoller mainRoller;
    DicePool attackRoll;
    int inputQuantity = 1;
    int inputFaces = 6;
    int inputModifier = 0;
    RollResult lastRoll;
    bool hasRolled = false;
    
    //Defies the chronology
    std::vector<std::string> rollHistory;
    
    // --- New Variables QUICK ROLLER ---
    int quickAdvantageState = 0; // 0: Normal, 1: Advantage, 2: Disadvantage
    int quickModifier = 0;
    
    // Metodi privati interni
    void renderUI(); 

public:
    DiceApp(int w, int h, const std::string& t);
    ~DiceApp(); // Il distruttore si occuperà del cleanup

    bool initialize();
    void run();
};
