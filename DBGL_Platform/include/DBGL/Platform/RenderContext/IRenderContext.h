//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef IRENDERCONTEXT_H_
#define IRENDERCONTEXT_H_

namespace dbgl
{
    class IRenderContext
    {
	public:
	    /**
	     * @brief Types of buffers attached to the render context
	     */
	    enum Buffer : char
	    {
		COLOR = 1 << 0,   //!< COLOR
		DEPTH = 1 << 1,   //!< DEPTH
		STENCIL = 1 << 2, //!< STENCIL
	    };
	    /**
	     * @brief Options that can be set for depth testing
	     */
	    enum class DepthTestValue
	    {
		Always,      //!< Always
		Never,       //!< Never
		Less,        //!< Less
		LessEqual,   //!< LessEqual
		Greater,     //!< Greater
		GreaterEqual,//!< GreaterEqual
		Equal,       //!< Equal
		NotEqual,    //!< NotEqual
	    };
	    /**
	     * @brief Options that can be set for alpha blending
	     */
	    enum class AlphaBlendValue
	    {
		Zero,            //!< Zero
		One,             //!< One
		DstColor,        //!< DstColor
		OneMinusDstColor,//!< OneMinusDstColor
		SrcAlpha,        //!< SrcAlpha
		OneMinusSrcAlpha,//!< OneMinusSrcAlpha
		DstAlpha,        //!< DstAlpha
		OneMinusDstAlpha,//!< OneMinusDstAlpha
		SrcAlphaSaturate,//!< SrcAlphaSaturate
	    };
	    /**
	     * @brief Options that can be set for face culling
	     */
	    enum class FaceCullingValue
	    {
		Off,      //!< Off
		Front,    //!< Front
		Back,     //!< Back
		FrontBack,//!< FrontBack
	    };

	    /**
	     * @brief Destructor
	     */
	    virtual ~IRenderContext() = default;
	    /**
	     * @brief Clears buffers
	     * @param bitmask ORed combination of values from IRenderContext::Buffer
	     */
	    virtual void clear(int bitmask) = 0;
	    /**
	     * @brief Sets the depth testing behavior
	     * @param val Value to set
	     */
	    virtual void setDepthTest(DepthTestValue val) = 0;
	    /**
	     * @brief Retrieves the currently set depth testing behavior
	     * @return
	     */
	    virtual DepthTestValue getDepthTest() const = 0;
	    /**
	     * @brief Sets the alpha blending behavior
	     * @param src Source factor
	     * @param dest Destination factor
	     * @note Set both \p src and \p dest to ZERO in order to disable alpha blending
	     */
	    virtual void setAlphaBlend(AlphaBlendValue src, AlphaBlendValue dest) = 0;
	    /**
	     * @brief Retrieves the currently set source factor for alpha blending
	     * @return Source factor
	     */
	    virtual AlphaBlendValue getSrcAlphaBlend() const = 0;
	    /**
	     * @brief Retrieves the currently set destination factor for alpha blending
	     * @return Destination factor
	     */
	    virtual AlphaBlendValue getDestAlphaBlend() const = 0;
	    /**
	     * @brief Sets the face culling behavior
	     * @param val Value to set
	     */
	    virtual void setFaceCulling(FaceCullingValue val) = 0;
	    /**
	     * @brief Retrieves the currently set face culling behavior
	     * @return Currently set face culling behavior
	     */
	    virtual FaceCullingValue getFaceCulling() const = 0;
	    /**
	     * @brief Bind this context
	     */
	    virtual void bind() = 0;
	    /**
	     * @brief Checks if this render context is currently bound
	     * @return True in case the context is bound, otherwise false
	     */
	    virtual bool isBound() const = 0;
	    /**
	     * @brief Retrieves the width of this render context
	     * @return Render context frame width in pixels
	     */
	    virtual int getWidth() = 0;
	    /**
	     * @brief Retrieves the height of this render context
	     * @return Render context frame height in pixels
	     */
	    virtual int getHeight() = 0;
    };
}



#endif /* IRENDERCONTEXT_H_ */
