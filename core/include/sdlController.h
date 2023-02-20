#ifndef SDLCONTROLLER_H
#define SDLCONTROLLER_H
/**
 * The header declaration of the SDL controller object which can be used to 
 * bind functions to SDL events.
 */
#include <SDL.h>
#include <array>
#include <functional>

// Callback function types. 
typedef std::function<void()> voidFuncWithNoArguments;

/**
 * @brief An object which can be used to bind functions to SDL events.
 * 
 */
class SDLController{

public:
    // Constructors.
    SDLController();

    /**
     * @brief Resets and unbinds all bindings held by the controller.
     * 
     * @returns - void.
     */
    void resetBindings();

    /**
     * @brief Runs the Key Down event handler.
     * 
     * @param event - The SDL event to respond too.
     * 
     * @returns - void.
     */
	void keyDown(SDL_Event& event);

	 /**
     * @brief Runs the Key Up event handler.
     * 
     * @param event - The SDL event to respond too.
     * 
     * @returns - void.
     */
	void keyUp(SDL_Event& event);

	/**
	 * @brief Bind a function to a key down event.
	 * 
	 * @param event - The event to bind `callbackFunction` to.
	 * @param callbackFunction - The function to call when responding to the `event`.
     * 
	 * @return boolean - Indicates success.
	 */
	bool bindKeyDown(SDL_Event& event, voidFuncWithNoArguments callbackFunction);
	/**
	 * @brief Bind a function to a specific scanCode's key down event.
	 * 
	 * @param scanCode - The scanCode to bind `callbackFunction` to.
	 * @param callbackFunction - The function to call when responding to the scanCode's event.
     * 
	 * @return boolean - Indicates success.
	 */
	bool bindKeyDown(SDL_Scancode scanCode, voidFuncWithNoArguments callbackFunction);

	/**
	 * @brief Bind a function to a key up event.
	 * 
	 * @param event - The event to bind `callbackFunction` to.
	 * @param callbackFunction - The function to call when responding to the `event`.
     * 
	 * @return boolean - Indicates success.
	 */
	bool bindKeyUp(SDL_Event& event, voidFuncWithNoArguments callbackFunction);
	/**
	 * @brief Bind a function to a specific scanCode's key up event.
	 * 
	 * @param scanCode - The scanCode to bind `callbackFunction` to.
	 * @param callbackFunction - The function to call when responding to the scanCode's event.
     * 
	 * @return boolean - Indicates success.
	 */
	bool bindKeyUp(SDL_Scancode scanCode, voidFuncWithNoArguments callbackFunction);

private:
    // Arrays which hold all active function bindings.
    std::array<voidFuncWithNoArguments, SDL_NUM_SCANCODES> primaryKeyDownBindings;
    std::array<voidFuncWithNoArguments, SDL_NUM_SCANCODES> primaryKeyUpBindings;
};

#endif