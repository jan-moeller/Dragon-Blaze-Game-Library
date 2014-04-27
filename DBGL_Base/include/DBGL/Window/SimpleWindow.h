//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef SIMPLEWINDOW_H_
#define SIMPLEWINDOW_H_

#include "Window.h"

namespace dbgl
{
    /**
     * @brief Simple default window with often used properties
     */
    class SimpleWindow: public Window
    {
	public:
	    /**
	     * @brief Frees all memory allocated to this window
	     */
	    ~SimpleWindow();

	protected:
	    /**
	     * @brief Creates a simple window
	     * @param share Other window to share resources with
	     * @param title Window title
	     * @param width Window width
	     * @param height Window height
	     * @param fullscreen Indicates if the window should be initialized as fullscreen
	     */
	    SimpleWindow(GLFWwindow* share, const char* title = "Dragon Blaze Game Library",
		    int width = 800, int height = 600, bool fullscreen = false);
	    /**
	     * @brief Called on key events.
	     * @param args Key event arguments
	     */
	    void keyCallback(KeyEventArgs args);

	private:
	    friend class WindowManager;
    };
}

#endif /* SIMPLEWINDOW_H_ */