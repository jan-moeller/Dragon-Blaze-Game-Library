//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef RENDERCOMPONENT_H_
#define RENDERCOMPONENT_H_

#include <memory>
#include "Entity.h"
#include "TransformComponent.h"
#include "DBGL/Rendering/RenderContext.h"
#include "DBGL/Rendering/Material/Material.h"
#include "DBGL/Rendering/Mesh/Mesh.h"
#include "DBGL/Rendering/Environment/Environment.h"

namespace dbgl
{
    class RenderComponent : Entity::Component
    {
	public:
	    /**
	     * @brief Initializes this render component
	     * @param mesh Mesh to render
	     * @param mat Material to use
	     * @param env Environment to use for rendering
	     */
	    RenderComponent(Mesh& mesh, Material& mat, Environment& env);
	    /**
	     * @brief Updates this component
	     * @param deltaTime Time since last update
	     */
	    virtual void update(Entity* owner, double deltaTime);
	    /**
	     * @brief Called upon render process
	     * @param rc RenderContext to draw to
	     */
	    virtual void render(Entity* owner, RenderContext const* rc) = 0;
	    /**
	     * @brief Determines whether this component wants to be updated
	     * @return True if needs to be updated, otherwise false
	     */
	    virtual bool needUpdate() const = 0;
	    /**
	     * @brief Determines whether this component wants to be rendered
	     * @return True if needs to be rendered, otherwise false
	     */
	    virtual bool needRender() const = 0;
	private:
	    void prepareMatrices(TransformComponent* transform, RenderContext const* rc);

	    Mesh* m_pMesh;
	    Material* m_pMaterial;
	    Environment* m_pEnvironment;
    };
}



#endif /* RENDERCOMPONENT_H_ */
