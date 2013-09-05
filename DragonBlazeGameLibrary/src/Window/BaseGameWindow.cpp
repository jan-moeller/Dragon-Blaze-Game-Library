/*
 * BaseGameWindow.cpp
 *
 *  Created on: 05.08.2013
 *      Author: Drag-On
 */

#include "BaseGameWindow.h"

namespace DBGL
{

// Initialize list of all game windows
std::list<BaseGameWindow*> BaseGameWindow::windows;
unsigned int BaseGameWindow::deltaTime = 0; // Time since last frame
int BaseGameWindow::lastCumulativeTime = 0; // Cumulative time since application start
int BaseGameWindow::fpsTimeBase = 0; // Time since application start when we last computed the frame rate
unsigned int BaseGameWindow::fpsAmount = 0; // Amount of frames since last computation of frame rate
unsigned int BaseGameWindow::fps = 0; // Current frame rate
unsigned int BaseGameWindow::lastFps = 0; // Last frame rate

/**
 * Constructor
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param displayMode Binary OR connected flags, i.e. GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH
 * @brief Allows to pass arguments, set the window size and display mode manually
 */
BaseGameWindow::BaseGameWindow(const char* title, unsigned int width,
		unsigned int height, unsigned int displayMode)
{
	_pTitle = title;
	_pInput = new Input();
	_pRC = new RenderContext();
	_pGame = NULL;

	// Set input callback functions
	Input::inputCallback keyPressedCallaback = std::tr1::bind(
			&BaseGameWindow::keyPressed, this, std::tr1::placeholders::_1,
			std::tr1::placeholders::_2);
	_pInput->setKeyPressedCallback(keyPressedCallaback);
	Input::inputCallback keyReleasedCallaback = std::tr1::bind(
			&BaseGameWindow::keyReleased, this, std::tr1::placeholders::_1,
			std::tr1::placeholders::_2);
	_pInput->setKeyReleasedCallback(keyReleasedCallaback);
	Input::mouseWheelCallback mouseWheelCallaback = std::tr1::bind(
			&BaseGameWindow::mouseWheel, this, std::tr1::placeholders::_1,
			std::tr1::placeholders::_2, std::tr1::placeholders::_3);
	_pInput->setMouseWheelCallback(mouseWheelCallaback);
	Input::dragCallback mouseDragCallaback = std::tr1::bind(
			&BaseGameWindow::mouseDragged, this, std::tr1::placeholders::_1,
			std::tr1::placeholders::_2, std::tr1::placeholders::_3,
			std::tr1::placeholders::_4);
	_pInput->setMouseDragCallback(mouseDragCallaback);

	// Initialize GLUT values
	glutInitDisplayMode(displayMode);
	glutInitWindowSize(width, height);

	_id = glutCreateWindow(_pTitle);
	glutReshapeFunc(onResize);
	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKeyInput);
	glutSpecialFunc(onSpecialKeyInput);
	glutKeyboardUpFunc(onKeyInputUp);
	glutSpecialUpFunc(onSpecialKeyUp);
	glutMouseFunc(onMouseInput);
	glutMotionFunc(onMouseDrag);
	glutPassiveMotionFunc(onMouseMove);
	glutMouseWheelFunc(onMouseWheel);
	glutIdleFunc(onIdle);

	// Register window
	BaseGameWindow::windows.push_back(this);
}

/**
 * Destructor
 */
BaseGameWindow::~BaseGameWindow()
{
	BaseGameWindow::windows.remove(this);

	delete (_pInput);
	_pInput = NULL;

	delete[] (_pTitle);
	_pTitle = NULL;

	delete (_pGame);
	_pGame = NULL;

	delete (_pRC);
	_pRC = NULL;
}

/**
 * @brief Closes the window immediately
 */
void BaseGameWindow::close()
{
	glutDestroyWindow(getId());
}

/**
 * @return Time in milliseconds since last frame
 */
unsigned int BaseGameWindow::getDeltaTime() const
{
	return BaseGameWindow::deltaTime;
}

/**
 * @return Average frames per second for the last second
 */
unsigned int BaseGameWindow::getFPS() const
{
	return BaseGameWindow::fps;
}

/**
 * @brief Renders everything to screen
 */
void BaseGameWindow::render()
{
	if (_pGame != NULL &&  glutGetWindow() != 0)
	{
		_pGame->preRender(this);
		_pGame->render(this, _pRC);
		_pGame->postRender(this);
	}
}

/**
 * @brief Updates everything before rendering
 */
void BaseGameWindow::update()
{
	// Update frame rate in window title
	if (BaseGameWindow::fps != BaseGameWindow::lastFps)
	{
		const char* title = getTitle();
		char* newTitle = new char[32];
		int i = 0;
		for (; i < 32 - 10 && title[i] != '\0'; i++)
		{
			newTitle[i] = title[i];
		}
		newTitle[i] = '|';
		newTitle[i + 1] = 'F';
		newTitle[i + 2] = 'P';
		newTitle[i + 3] = 'S';
		newTitle[i + 4] = ':';
		char fpsString[5];
		sprintf(fpsString, "%d", BaseGameWindow::fps);
		for (int j = 0; j < 5; j++)
			newTitle[i + 5 + j] = fpsString[j];
		setActive();
		glutSetWindowTitle(newTitle);
	}

	// Update game object
	if (_pGame != NULL)
		_pGame->update(this, _pRC);
}

/**
 * @brief Handles window resize
 * @param width New window width
 * @param height New window height
 */
void BaseGameWindow::resize(int width, int height)
{
	glViewport(0, 0, width, height);
}

/**
 * @brief Handles key pressed events
 */
void BaseGameWindow::keyPressed(KEY key, char character)
{
}

/**
 * @brief Handles key released events
 */
void BaseGameWindow::keyReleased(KEY key, char character)
{
}

/**
 * @brief Handles key pressed events
 */
void BaseGameWindow::mouseDragged(int x, int y, int lastX, int lastY)
{
}

/**
 * @brief Handles key pressed events
 */
void BaseGameWindow::mouseWheel(int change, int x, int y)
{
}

/**
 * @brief Sets the owner of this window
 * @param game Game handler owning this window
 */
void BaseGameWindow::setOwner(BasicGame* game)
{
	_pGame = game;
}

/**
 * @brief Called once a frame, updates and renders all windows
 */
void BaseGameWindow::onDisplay()
{
	// Update windows
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		(*iterator)->update();
		(*iterator)->render();

		if ((*iterator)->isActive())
			(*iterator)->_pInput->update();
	}
}

/**
 * @brief Called on every resize operation
 * @param width New window width
 * @param height New window height
 */
void BaseGameWindow::onResize(int width, int height)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->resize(width, height);
			break;
		}
	}
}

/**
 * @brief Called every time some input happened
 * @param key Key that got pressed
 * @param x Mouse x coordinate
 * @param y Mouse y coordinate
 */
void BaseGameWindow::onKeyInput(unsigned char key, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onKeyInput(key, x, y);
			break;
		}
	}
}

/**
 * @brief Called on every mouse input
 * @param button Mouse button that changed state
 * @param state New state of the button
 * @param x Mouse x coordinate
 * @param y Mouse y coordinate
 */
void BaseGameWindow::onMouseInput(int button, int state, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onMouseInput(button, state, x, y);
			break;
		}
	}
}

/**
 * @brief Called when the mouse got moved with a mouse button pressed down
 * @param x X mouse coordinate
 * @param y Y mouse coordinate
 */
void BaseGameWindow::onMouseDrag(int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onMouseDrag(x, y);
			break;
		}
	}
}

/**
 * @brief Called when the mouse got moved
 * @param x X mouse coordinate
 * @param y Y mouse coordinate
 */
void BaseGameWindow::onMouseMove(int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onMouseMove(x, y);
			break;
		}
	}
}

/**
 * @brief Called once a special key on the keyboard got pressed
 * @param key Special key that got pressed
 * @param x X mouse coordinate
 * @param y Y mouse coordinate
 */
void BaseGameWindow::onSpecialKeyInput(int key, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onSpecialKeyInput(key, x, y);
			break;
		}
	}
}

/**
 * @brief Called once a key was released
 * @param key Key that has been released
 * @param x Mouse x coordinate
 * @param y Mouse y coordinate
 */
void BaseGameWindow::onKeyInputUp(unsigned char key, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onKeyInputUp(key, x, y);
			break;
		}
	}
}

/**
 * @brief Called once a special key was released
 * @param key Key that has been released
 * @param x Mouse x coordinate
 * @param y Mouse y coordinate
 */
void BaseGameWindow::onSpecialKeyUp(int key, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onSpecialKeyUp(key, x, y);
			break;
		}
	}
}

/**
 * @brief Called once the mouse wheel changed
 * @param button Mouse button
 * @param dir Direction of scroll wheel
 * @param x Mouse x coordinate
 * @param y Mouse y coordinate
 */
void BaseGameWindow::onMouseWheel(int button, int dir, int x, int y)
{
	for (std::list<BaseGameWindow*>::const_iterator iterator =
			BaseGameWindow::windows.begin(), end =
			BaseGameWindow::windows.end(); iterator != end; ++iterator)
	{
		if ((*iterator)->isActive())
		{
			(*iterator)->_pInput->onMouseWheel(button, dir, x, y);
			break;
		}
	}
}

/**
 * Called when the current draw process has been finished
 */
void BaseGameWindow::onIdle()
{
	// Calculate fps
	int curTime = glutGet(GLUT_ELAPSED_TIME);
	BaseGameWindow::fpsAmount++;
	if ((int) (curTime - BaseGameWindow::fpsTimeBase) > 1000)
	{
		BaseGameWindow::lastFps = BaseGameWindow::fps;
		BaseGameWindow::fps = BaseGameWindow::fpsAmount * 1000.0
				/ (curTime - BaseGameWindow::fpsTimeBase);
		BaseGameWindow::fpsAmount = 0;
		BaseGameWindow::fpsTimeBase = curTime;
	}

	// Calculate time
	BaseGameWindow::deltaTime = curTime - BaseGameWindow::lastCumulativeTime;
	BaseGameWindow::lastCumulativeTime = curTime;

	// Redraw
	glutPostRedisplay();
}

/**
 * @brief Sets the current window position immediately
 * @param x X coordinate
 * @param y Y coordinate
 */
void BaseGameWindow::setPosition(unsigned int x, unsigned int y) const
{
	if (!isActive())
		setActive();
	glutPositionWindow(x, y);
}

/**
 * @return Current x position
 */
int BaseGameWindow::getX() const
{
	if (!isActive())
		setActive();
	return glutGet((GLenum) GLUT_WINDOW_X);
}

/**
 * @return Current y position
 */
int BaseGameWindow::getY() const
{
	if (!isActive())
		setActive();
	return glutGet((GLenum) GLUT_WINDOW_Y);
}

/**
 * @brief Sets the window size immediately
 * @param width New window width
 * @param height New window height
 */
void BaseGameWindow::setSize(unsigned int width, unsigned int height) const
{
	if (!isActive())
		setActive();
	glutReshapeWindow(width, height);
}

/**
 * @return Current window width
 */
int BaseGameWindow::getWidth() const
{
	if (!isActive())
		setActive();
	return glutGet((GLenum) GLUT_WINDOW_WIDTH);
}

/**
 * @return Current window height
 */
int BaseGameWindow::getHeight() const
{
	if (!isActive())
		setActive();
	return glutGet((GLenum) GLUT_WINDOW_HEIGHT);
}

/**
 * @brief Toggles fullscreen mode of this window
 * @param fullscreen Flag indicating if fullscreen should be on or off
 */
void BaseGameWindow::setFullscreen(bool fullscreen) const
{
	if (!isActive())
		setActive();
	if (fullscreen)
		glutFullScreen();
	else
		glutLeaveFullScreen();
}

/**
 * @return True in case this window is currently in fullscreen mode, otherwise false
 */
bool BaseGameWindow::isFullscreen() const
{
	if (!isActive())
		setActive();
	return glutGet(GLUT_FULL_SCREEN);
}

/**
 * Makes this window the currently active window
 */
void BaseGameWindow::setActive() const
{
	glutSetWindow(getId());
}

/**
 * @return Indicates, if this window is currently active
 */
bool BaseGameWindow::isActive() const
{
	return getId() == glutGetWindow();
}

/**
 * @param newTitle Sets a new title for this window
 */
void BaseGameWindow::setTitle(const char* newTitle)
{
	delete[] _pTitle;
	_pTitle = newTitle;
	setActive();
	glutSetWindowTitle(_pTitle);
}

/**
 * @return Title of the window
 */
const char* BaseGameWindow::getTitle() const
{
	return _pTitle;
}

/**
 * @return ID of this window
 */
int BaseGameWindow::getId() const
{
	return _id;
}

/**
 * @return The input handler of this window
 */
const Input* BaseGameWindow::getInput() const
{
	return _pInput;
}

/**
 * @return The render context of this window
 */
const RenderContext* BaseGameWindow::getRC() const
{
	return _pRC;
}

} /* namespace DBGL */
