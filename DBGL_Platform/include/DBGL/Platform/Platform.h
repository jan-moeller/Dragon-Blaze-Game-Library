//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <string>
#include "DBGL/Platform/Window/IWindow.h"

namespace dbgl
{
    class Platform
    {
	public:
	    class IType
	    {
		public:
		    virtual ~IType() = default;
		    virtual IWindow* createWindow(std::string title = "Dragon Blaze Game Library", int width =
			    800, int height = 600, bool fullscreen = false,
			    unsigned int multisampling = 2) = 0;
	    };

	    /**
	     * @brief Initializes the specified platform implementation
	     * @param type Type of platform implementation to use
	     */
	    template <class Type> static void init();
	    /**
	     * @brief Destroy the currently active platform implementation
	     */
	    static void destroy();
	    /**
	     * @brief Provides a pointer to the currently initialized platform implementation
	     * @return Pointer to platform implementation or nullptr if init() hasn't been called yet
	     */
	    static IType* get();

	private:
	    static IType* s_pType;
    };
}

#include "Platform.imp"

#endif /* PLATFORM_H_ */