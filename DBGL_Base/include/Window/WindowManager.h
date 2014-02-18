//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include <stdlib.h>
#include <map>//#include <unordered_map>#include <GL/glew.h>#include <GLFW/glfw3.h>
#include "Window.h"
#include "Log/Log.h"

namespace dbgl
{
    class Window;

    /**
     * @brief Handles all open windows
     * @details Is implemented as a singleton, so the only instance needs to be
     * 		acquired by calling the static get() method.
     */
    class WindowManager
    {
	private:
	    GLFWwindow* _shareWindow;
	public:
	    /**
	     * @return The only instance of this class
	     */
	    static WindowManager* get();
	    /**
	     * @brief Constructs a new window
	     * @return A new window of the specified type with default initialization
	     */
	    template<typename T> T* createWindow()
	    {
		GLFWwindow* share = _shareWindow;
		auto wnd = new T(share);
		windows.insert(
			std::pair<GLFWwindow*, Window*>(wnd->_pWndHandle, wnd));
		if (_shareWindow == NULL)
		    _shareWindow = wnd->_pWndHandle;
		return wnd;
	    }
	    /**
	     * @brief Constructs a new window
	     * @param title Window title
	     * @return A new window of the specified type with default initialization
	     */
	    template<typename T> T* createWindow(const char* title)
	    {
		GLFWwindow* share = _shareWindow;
		auto wnd = new T(share, title);
		windows.insert(
			std::pair<GLFWwindow*, Window*>(wnd->_pWndHandle, wnd));
		if (_shareWindow == NULL)
		    _shareWindow = wnd->_pWndHandle;
		return wnd;
	    }
	    /**
	     * @brief Constructs a new window
	     * @param title Window title
	     * @param width Window width
	     * @param height Window height
	     * @return A new window of the specified type
	     */
	    template<typename T> T* createWindow(const char* title, int width,
		    int height)
	    {
		GLFWwindow* share = _shareWindow;
		auto wnd = new T(share, title, width, height);
		windows.insert(
			std::pair<GLFWwindow*, Window*>(wnd->_pWndHandle, wnd));
		if (_shareWindow == NULL)
		    _shareWindow = wnd->_pWndHandle;
		return wnd;
	    }
	    /**
	     * @brief Constructs a new window
	     * @param title Window title
	     * @param width Window width
	     * @param height Window height
	     * @param fullscreen True if the window shall be opened fullscreen,
	     * 			 otherwise false
	     * @return A new window of the specified type
	     */
	    template<typename T> T* createWindow(const char* title, int width,
		    int height, bool fullscreen)
	    {
		GLFWwindow* share = _shareWindow;
		auto wnd = new T(share, title, width, height, fullscreen);
		windows.insert(
			std::pair<GLFWwindow*, Window*>(wnd->_pWndHandle, wnd));
		if (_shareWindow == NULL)
		    _shareWindow = wnd->_pWndHandle;
		return wnd;
	    }
	    /**
	     * @brief Updates and renders all windows for one frame
	     */
	    void update();
	    /**
	     * @brief Terminates all windows.
	     * @warning Needs to be called before exiting the program!
	     */
	    void terminate();
	    /**
	     * @return True in case there are still windows open, otherwise false
	     */
	    bool isRunning() const;
	private:
	    WindowManager();
	    ~WindowManager();
	    static void errorCallback(int error, const char* description);
	    static void closeCallback(GLFWwindow* window);
	    static void focusCallback(GLFWwindow* window, int focused);
	    static void iconifiedCallback(GLFWwindow* window, int iconified);
	    static void refreshCallback(GLFWwindow* window);
	    static void resizeCallback(GLFWwindow* window, int width,
		    int height);
	    static void framebufferResizeCallback(GLFWwindow* window, int width,
		    int height);
	    static void positionCallback(GLFWwindow* window, int xpos,
		    int ypos);
	    static void characterCallback(GLFWwindow* window,
		    unsigned int codepoint);
	    static void cursorEnterCallback(GLFWwindow* window, int entered);
	    static void cursorCallback(GLFWwindow* window, double x, double y);
	    static void mouseButtonCallback(GLFWwindow* window, int button,
		    int action, int mods);
	    static void scrollCallback(GLFWwindow* window, double xOffset,
		    double yOffset);
	    static void keyCallback(GLFWwindow* window, int key, int scancode,
		    int action, int mods);
	    //static void monitorCallback(GLFWmonitor* monitor, int event);

	    void updateHandle(GLFWwindow* oldHandle, GLFWwindow* newHandle);

	    static std::map<GLFWwindow*, Window*> windows; // TODO: unordered_map?
	    static WindowManager instance;

	    friend class Window;
    };
}

#endif /* WINDOWMANAGER_H_ */
