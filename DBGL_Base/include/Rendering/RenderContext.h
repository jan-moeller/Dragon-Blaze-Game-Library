//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef RENDERCONTEXT_H_
#define RENDERCONTEXT_H_

#include <GL/glew.h>
#include <vector>
#include "Mesh.h"
#include "Viewport.h"

namespace dbgl
{
    /**
     * @brief The RenderContext serves as the main rendering authority.
     * @details All objects that are supposed to be displayed need to be passed to an instance
     * 		of this class. It will then take care of the model and render it to all
     * 		viewports where it is visible.
     */
    class RenderContext
    {
	public:
	    /**
	     * @brief Creates a new render context
	     */
	    RenderContext();
	    /**
	     * @brief Cleans up memory
	     */
	    ~RenderContext();
	    /**
	     * @brief Adds a new viewport to this context which will then be drawn to
	     * @details The viewport may point to any open window. Without a viewport
	     * 		you won't see anything!
	     * @param viewport New viewport object to add
	     */
	    void addViewport(Viewport const& viewport);
	    /**
	     * @brief Draws a mesh to all viewports where it is visible
	     * @note Currently being implemented
	     * @param mesh
	     */
	    void draw(const Mesh* mesh) const;
	    /**
	     * @brief Initializes the general OpenGL context. Needs to be called once
	     * 	      after starup!
	     * @param depthTest Indicates if depth testing should be enabled
	     * @param faceCulling Indicates if faces that are not facing the camera
	     * 	      should be culled
	     */
	    static void init(bool depthTest = true, bool faceCulling = true);
	    /**
	     * @brief Destroys the OpenGL context. Needs to be called once before
	     * 	      exiting the application!
	     */
	    static void destroy();
	private:
	    /**
	     * @brief Renders a mesh to the current context, assuming everything has
	     * 	      been set up accordingly. This method does not touch any matrices,
	     * 	      shaders, or other stuff. It just renders the mesh to whatever has
	     * 	      been set up before.
	     * @param mesh Pointer to the mesh to render
	     */
	    void renderMesh(const Mesh* mesh) const;

	    std::vector<Viewport> _viewports;
	    static GLuint vertexArrayId;
    };
}

#endif /* RENDERCONTEXT_H_ */
