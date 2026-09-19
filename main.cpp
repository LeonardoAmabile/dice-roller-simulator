#include "src/gui/DiceApp.h"
#include <iostream>

int main() {
    DiceApp app(1280, 720, "Dice Roller Studio");
    
    if (!app.initialize()) {
        std::cerr << "Critical error initializing the GUI." << std::endl;
        return 1;
    }
    
    app.run(); 
    
    // La memoria di OpenGL e GLFW viene liberata in automatico 
    // dal distruttore di 'app' quando la funzione main finisce.
    return 0; 
}