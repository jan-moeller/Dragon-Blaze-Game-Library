//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2014 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#include "DBGL/Platform/Module/Module.h"

namespace dbgl
{
    Module::Module(std::string path) : m_path{path}
    {
    }

    Module::~Module()
    {
        if(m_handle)
        {
#ifdef __linux__
            internal_os::dlclose(m_handle);
#elif __WIN32
            internal_os::FreeLibrary(m_handle);
#endif
        }
    }

    bool Module::load()
    {
#ifdef __linux__
        m_handle = internal_os::dlopen(m_path.c_str(), RTLD_LAZY);
#elif __WIN32
        m_handle = internal_os::LoadLibrary(m_path.c_str());
#endif
	if (!m_handle)
	    return false;
	else
	    return true;
    }

    void* Module::getFunction(std::string name)
    {
#ifdef __linux__
        return internal_os::dlsym(m_handle, name.c_str());
#elif __WIN32
	return internal_os::GetProcAddress(m_handle, name.c_str());
#endif
    }
}
