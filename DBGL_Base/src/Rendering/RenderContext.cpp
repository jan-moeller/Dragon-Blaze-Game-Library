//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#include "Rendering/RenderContext.h"

namespace dbgl
{
    RenderContext::RenderContext(unsigned int frameWidth,
	    unsigned int frameHeight) :
	    _frameWidth(frameWidth), _frameHeight(frameHeight)
    {
    }

    RenderContext::~RenderContext()
    {
    }

    void RenderContext::addViewport(Viewport* viewport)
    {
	_viewports.push_back(viewport);
    }

    void RenderContext::removeViewport(Viewport* viewport)
    {
	_viewports.erase(
		std::remove(_viewports.begin(), _viewports.end(), viewport),
		_viewports.end());
    }

    void RenderContext::draw(const Mesh* mesh, ShaderProgram* shader) const
    {
	// Check all viewports
	for (auto &viewport : _viewports)
	{
	    // Calculate absolute viewport values
	    int viewportX = viewport->getXRatio() * _frameWidth;
	    int viewportY = viewport->getYRatio() * _frameHeight;
	    int viewportWidth = viewport->getWidthRatio()
		    * (_frameWidth - viewportX);
	    int viewportHeight = viewport->getHeightRatio()
		    * (_frameHeight - viewportY);
	    // If the viewport has a camera attached send matrices to shader
	    if (viewport->getCamera() != NULL)
	    {
		// Update camera in case the window has been resized
		viewport->getCamera()->update(
			(float) viewportWidth / viewportHeight);
		// TODO: Frustrum culling
		// TODO: Don't recalculate matrices for every model
		// Send model matrix if the shader needs it
		GLint model = shader->getDefaultUniformHandle(
			ShaderProgram::Uniform::MODEL);
		if (model >= 0)
		{
		    // TODO: Replace with real matrix
		    shader->setUniformFloatMatrix4Array(model, 1, GL_FALSE,
			    Mat4f().getDataPointer());
		}
		// Send view matrix if the shader needs it
		GLint view = shader->getDefaultUniformHandle(
			ShaderProgram::Uniform::VIEW);
		if (view >= 0)
		{
		    shader->setUniformFloatMatrix4Array(view, 1, GL_FALSE,
			    viewport->getCamera()->getViewMat().getDataPointer());
		}
		// Send projection matrix if the shader needs it
		GLint projection = shader->getDefaultUniformHandle(
			ShaderProgram::Uniform::PROJECTION);
		if (projection >= 0)
		{
		    shader->setUniformFloatMatrix4Array(projection, 1, GL_FALSE,
			    viewport->getCamera()->getProjectionMat().getDataPointer());
		}
		// Send itmv matrix if the shader needs it
		GLint itmv = shader->getDefaultUniformHandle(
			ShaderProgram::Uniform::ITMV);
		if (itmv >= 0)
		{
		    auto itmvMat =
			    viewport->getCamera()->getViewMat().getInverted().getTransposed();
		    shader->setUniformFloatMatrix4Array(itmv, 1, GL_FALSE,
			    itmvMat.getDataPointer());
		}
	    }
	    // Set viewport
	    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
	    // Render mesh
	    renderMesh(mesh);
	}
    }

    void RenderContext::update(unsigned int width, unsigned int height)
    {
	_frameWidth = width;
	_frameHeight = height;
    }

    void RenderContext::renderMesh(const Mesh* mesh) const
    {
	// Bind vertex buffer : 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_vertexBuffer);
	glVertexAttribPointer(0,	// attribute
		3,			// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		0,			// stride
		(void*) 0);		// offset

	// Bind UV buffer : 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_uvBuffer);
	glVertexAttribPointer(1,	// attribute
		2,			// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		0,			// stride
		(void*) 0);		// offset

	// Bind normal buffer : 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_normalBuffer);
	glVertexAttribPointer(2,	// attribute
		3,			// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		0,			// stride
		(void*) 0);		// offset

	// Bind color buffer : 3
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->_colorBuffer);
	glVertexAttribPointer(3,	// attribute
		3,			// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		0,			// stride
		(void*) 0);		// offset

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->_indexBuffer);

	// Draw!
	glDrawElements(GL_TRIANGLES,	// mode
		mesh->_indices.size(),	// count
		GL_UNSIGNED_SHORT,	// type
		(void*) 0);		// offset

	// Disable buffers
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
    }
}

