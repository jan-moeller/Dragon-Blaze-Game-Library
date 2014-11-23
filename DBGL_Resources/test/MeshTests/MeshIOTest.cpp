//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#include "DBGL/Core/Test/Test.h"
#include "DBGL/Core/Utility/MeshUtility.h"
#include "DBGL/Core/Math/Utility.h"
#include "DBGL/Resources/Mesh/MeshIO.h"
#include "DBGL/Platform/Mesh/IMesh.h"
#include "DBGL/Platform/Platform.h"
#include "DBGL/Platform/Implementation/OpenGL33.h"

using namespace dbgl;
using namespace std;

namespace dbgl_test_MeshIO
{
    IWindow* g_wnd = nullptr;
}

TEST_INITIALIZE(MeshIO)
{
    // Init graphics context
    Platform::init<OpenGL33>();
    dbgl_test_MeshIO::g_wnd = Platform::get()->createWindow();
}

TEST_TERMINATE(MeshIO)
{
    delete dbgl_test_MeshIO::g_wnd;
    Platform::destroy();
}

TEST(MeshIO,obj)
{
    // MeshIO object
    MeshIO io {};
    if (!io.addFormat("plugins\\Mesh\\OBJ\\libDBGL_OBJ.dll"))
	FAIL();
    else
    {
	auto mesh = io.load("plugins\\Mesh\\OBJ\\test.obj");
	ASSERT(mesh);
	ASSERT(mesh->getVertexCount() == 8);
    }
}

