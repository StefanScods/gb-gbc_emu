/**
 * The implementation of the SDL controller object which can be used to 
 * bind functions to SDL events.
 */
#include "include\sdlController.h"
#include <iostream>

SDLController::SDLController(){
    resetBindings();
}

void SDLController::resetBindings(){
    // Create new arrays with all bindings set to nullptr.
    // This effectively clears all bindings.
    primaryKeyDownBindings = { nullptr };
	primaryKeyUpBindings = { nullptr };
}

void SDLController::keyDown(SDL_Event& event){
     // Error check to ensure we are responding to the correct event.
    if(event.type != SDL_KEYDOWN){
        std::cerr << "Error: Invalid event: '" << event.type << "'." << std::endl;
        return;
    }

    // Check that the scan code lies within the expected range.
    SDL_Scancode scanCode = event.key.keysym.scancode;
    if(scanCode <= SDL_SCANCODE_UNKNOWN  || scanCode >= SDL_NUM_SCANCODES){
        std::cerr << "Error: Unknown scancode '" << scanCode << "'." << std::endl;
        return;
    }

    // Check to see if the function is bound.
    voidFuncWithNoArguments boundCallback = primaryKeyDownBindings[scanCode];
    // Do nothing if no function is bound.
    if(boundCallback == nullptr) return;

    // Call the function if it is bound to this controller.
    boundCallback();
}

void SDLController::keyUp(SDL_Event& event){
    // Error check to ensure we are responding to the correct event.
    if(event.type != SDL_KEYUP){
        std::cerr << "Error: Invalid event: '" << event.type << "'." << std::endl;
        return;
    }

    SDL_Scancode scanCode = event.key.keysym.scancode;
    if(scanCode <= SDL_SCANCODE_UNKNOWN  || scanCode >= SDL_NUM_SCANCODES){
        std::cerr << "Error: Unknown scancode '" << scanCode << "'." << std::endl;
        return;
    }

    // Check to see if the function is bound.
    voidFuncWithNoArguments boundCallback = primaryKeyUpBindings[scanCode];
    // Do nothing if no function is bound.
    if(boundCallback == nullptr) return;

    // Call the function if it is bound to this controller.
    boundCallback();
}

bool SDLController::bindKeyDown(SDL_Scancode scanCode, voidFuncWithNoArguments callbackFunction){
    // Check that the scan code lies within the expected range.
    if(scanCode <= SDL_SCANCODE_UNKNOWN  || scanCode >= SDL_NUM_SCANCODES){
        std::cerr << "Error: Unknown scancode '" << scanCode << "'." << std::endl;
        return false;
    }

    // Check to see if the function is already bound.
    voidFuncWithNoArguments boundCallback = primaryKeyDownBindings[scanCode];
    // Ensure we do not get binding collisions.
    if( boundCallback != nullptr ){
        std::cerr << "Error: Scancode '" << scanCode << "' is already bound. Consider calling 'SDLController::unbindKeyDown()'." << std::endl;
        return false;
    }

    // Bind the new function.
    primaryKeyDownBindings[scanCode] = callbackFunction;
    return true;
}

bool SDLController::bindKeyDown(SDL_Event& event, voidFuncWithNoArguments callbackFunction){
     // Error check to ensure we are responding to the correct event.
    if(event.type != SDL_KEYDOWN){
        std::cerr << "Error: Invalid event: '" << event.type << "'." << std::endl;
        return false;
    }
    // Call the scancode binding function.
    return bindKeyDown(event.key.keysym.scancode, callbackFunction);
}

bool SDLController::bindKeyUp(SDL_Scancode scanCode, voidFuncWithNoArguments callbackFunction){
    // Check that the scan code lies within the expected range.
    if(scanCode <= SDL_SCANCODE_UNKNOWN  || scanCode >= SDL_NUM_SCANCODES){
        std::cerr << "Error: Unknown scancode '" << scanCode << "'." << std::endl;
        return false;
    }

    // Check to see if the function is already bound.
    voidFuncWithNoArguments boundCallback = primaryKeyUpBindings[scanCode];
    // Ensure we do not get binding collisions.
    if( boundCallback != nullptr ){
        std::cerr << "Error: Scancode '" << scanCode << "' is already bound. Consider calling 'SDLController::unbindKeyUp()'." << std::endl;
        return false;
    }

    // Bind the new function.
    primaryKeyUpBindings[scanCode] = callbackFunction;
    return true;
}

bool SDLController::bindKeyUp(SDL_Event& event, voidFuncWithNoArguments callbackFunction){
     // Error check to ensure we are responding to the correct event.
    if(event.type != SDL_KEYUP){
        std::cerr << "Error: Invalid event: '" << event.type << "'." << std::endl;
        return false;
    }
    // Call the scancode binding function.
    return bindKeyUp(event.key.keysym.scancode, callbackFunction);
}