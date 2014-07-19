//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#include "Entity.h"
#include "DBGL/Math/Vector3.h"
#include "DBGL/Math/Quaternion.h"

namespace dbgl
{
    class TransformComponent : public Entity::Component
    {
	public:
	    /**
	     * @brief Initializes this component
	     * @param pos Initial position
	     * @param scale Initial scale
	     * @param rot Initial rotation
	     */
	    TransformComponent(Vec3f pos = {}, Vec3f scale = {}, QuatF rot = {});
	    /**
	     * @return Reference to position
	     */
	    Vec3f& position();
	    /**
	     * @return Reference to scale
	     */
	    Vec3f& scale();
	    /**
	     * @return Reference to rotation
	     */
	    QuatF& rotation();
	    /**
	     * @brief Updates all components
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
	    Vec3f m_position = Vec3f(0, 0, 0);
	    Vec3f m_scale = Vec3f(1, 1, 1);
	    QuatF m_rotation = QuatF(0, 0, 0, 1);
    };
}

#endif /* TRANSFORMCOMPONENT_H_ */
