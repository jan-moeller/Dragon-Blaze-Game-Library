//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#ifndef MESH_H_
#define MESH_H_

#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "System/Log/Log.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"

namespace dbgl
{
    /**
     * @brief Represents a 3D model which can be rendered to the screen
     */
    class Mesh
    {
	public:
	    /**
	     * @brief File types which can be loaded
	     */
	    enum Type
	    {
		OBJ, //!< OBJ
	    };

	    /**
	     * @brief Clean up memory
	     */
	    ~Mesh();
	    /**
	     * @return A const reference to the indices list
	     */
	    std::vector<unsigned short> const& getIndices() const;
	    /**
	     * @return A const reference to the vertices list
	     */
	    std::vector<Vector3<GLfloat>> const& getVertices() const;
	    /**
	     * @return A const reference to the normals list
	     */
	    std::vector<Vector3<GLfloat>> const& getNormals() const;
	    /**
	     * @return A const reference to the UVs list
	     */
	    std::vector<Vector2<GLfloat>> const& getUVs() const;
	    /**
	     * @return A const reference to the tangents list
	     */
	    std::vector<Vector3<GLfloat>> const& getTangents() const;
	    /**
	     * @return A const reference to the bitangents list
	     */
	    std::vector<Vector3<GLfloat>> const& getBitangents() const;
	    /**
	     * @brief Loads a mesh from hard disk
	     * @warning The allocated memory needs to be freed manually!
	     * @param path Path of the file
	     * @param type File type to load
	     * @param generateTangentBase Flag indicating if tangents
	     * 	      and bitangents should be generated
	     * @param sendToGPU automatically sends the loaded mesh to gpu
	     * @return The loaded mesh or NULL if something went wrong
	     */
	    static Mesh* load(const std::string path, const Type type,
		    bool generateTangentBase = false, bool sendToGPU = true);
	    /**
	     * @brief Creates a mesh that represents a simple triangle
	     * @warning The allocated memory needs to be freed manually!
	     * @param generateTangentBase Flag indicating if tangents
	     * 	      and bitangents should be generated
	     * @param sendToGPU automatically sends the loaded mesh to gpu
	     * @return The triangle mesh
	     */
	    static Mesh* makeTriangle(bool generateTangentBase = false, bool sendToGPU = true);
	    /**
	     * @brief Creates a mesh that represents a simple plane
	     * @warning The allocated memory needs to be freed manually!
	     * @param generateTangentBase Flag indicating if tangents
	     * 	      and bitangents should be generated
	     * @param sendToGPU automatically sends the loaded mesh to gpu
	     * @return The plane mesh
	     */
	    static Mesh* makePlane(bool generateTangentBase = false, bool sendToGPU = true);
	    /**
	     * @brief Creates a mesh that represents a cube
	     * @warning The allocated memory needs to be freed manually!
	     * @param generateTangentBase Flag indicating if tangents
	     * 	      and bitangents should be generated
	     * @param sendToGPU automatically sends the loaded mesh to gpu
	     * @return The cube mesh
	     */
	    static Mesh* makeCube(bool generateTangentBase = false, bool sendToGPU = true);
	    /**
	     * @brief Creates a mesh that represents a four-sided pyramid
	     * @warning The allocated memory needs to be freed manually!
	     * @param generateTangentBase Flag indicating if tangents
	     * 	      and bitangents should be generated
	     * @param sendToGPU automatically sends the loaded mesh to gpu
	     * @return The pyramid mesh
	     */
	    static Mesh* makePyramid(bool generateTangentBase = false, bool sendToGPU = true);
	private:
	    /**
	     * @brief Private constructor; use factory methods!
	     */
	    Mesh();
	    /**
	     * @brief Updates GL buffers, resends vertices, uvs and normals
	     */
	    void updateBuffers();
	    /**
	     * @brief Checks, if the mesh has a vertex with the passed coordinates
	     * 	      (or at least very, very similar)
	     * @param coords Coordinates to search for
	     * @return Index of the vertex or size if not found
	     */
	    unsigned int getVertexIndex(Vec3f const& coords);
	    /**
	     * @brief Generates tangents and bitangents from this mesh's normals and UVs
	     */
	    void generateTangentBasis();
	    /**
	     * @brief Generates a gl buffer
	     * @return Buffer identifier or GL_INVALID_VALUE if something went wrong
	     * @warning The generated buffer needs to be deleted using glDeleteBuffers
	     */
	    static GLuint generateBuffer();
	    /**
	     * @brief Fills a buffer with data
	     * @param buffer Buffer to fill
	     * @param target Type of buffer, e.g. GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
	     * @param size Buffer size
	     * @param data Data to store
	     * @param usage Expected usage, e.g. GL_STATIC_DRAW
	     * @return Buffer identifier
	     */
	    static void fillBuffer(GLuint buffer, GLenum target,
		    GLsizeiptr size, const GLvoid* data, GLenum usage);
	    /**
	     * @brief Loads a mesh in OBJ file format
	     * @param path Path of the file
	     * @return The loaded mesh
	     */
	    static Mesh* loadOBJ(const std::string path, bool sendToGPU);

	    std::vector<unsigned short> m_indices;
	    GLuint m_indexBuffer = GL_INVALID_VALUE;
	    std::vector<Vector3<GLfloat>> m_vertices;
	    GLuint m_vertexBuffer = GL_INVALID_VALUE;
	    std::vector<Vector3<GLfloat>> m_normals;
	    GLuint m_normalBuffer = GL_INVALID_VALUE;
	    std::vector<Vector2<GLfloat>> m_uv;
	    GLuint m_uvBuffer = GL_INVALID_VALUE;
	    std::vector<Vector3<GLfloat>> m_tangents;
	    GLuint m_tangentBuffer = GL_INVALID_VALUE;
	    std::vector<Vector3<GLfloat>> m_bitangents;
	    GLuint m_bitangentBuffer = GL_INVALID_VALUE;

	    friend class RenderContext;
	    // So the render context can render
    };
}

#endif /* MESH_H_ */
