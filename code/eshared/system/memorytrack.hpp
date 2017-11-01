/*
Copyright (c) 2002, 2008 Curtis Bartley
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the
distribution.

- Neither the name of Curtis Bartley nor the names of any other
contributors may be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MEMORYTRACK_HPP
#define MEMORYTRACK_HPP

#include "compiler.hpp"

#ifdef eDEFINE_MEMORYTRACK

// include here all files that need to be included before debug new macro (stl implementation)

#if eCONFIG_OS == eCONF_OS_LINUX
    #include <cxxabi.h>
#endif

#if eCONFIG_OS == eCONF_OS_WINDOWS
    #include <Windows.h>
    #include <DbgHelp.h>
#endif

#include <typeinfo>

class MemStamp
{
public:        // member variables
    char const * const filename;
    int const lineNum;
public:        // construction/destruction
    MemStamp(char const *filename, int lineNum)
        : filename(filename), lineNum(lineNum) { }
    ~MemStamp() { }
};

/* memory allocation and stamping prototypes */

void *TrackMalloc(size_t size);
void TrackFree(void *p);
void TrackStamp(void *p, const MemStamp &stamp, char const *typeName);
void TrackDumpBlocks();
void TrackListMemoryUsage();
size_t GetNumBlocks();
size_t GetSizeBlocks();

/* operator * (MemStamp, ptr) */

template <class T> inline T *operator*(const MemStamp &stamp, T *p)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    /*
    char undecorated_name[1024];
    if (!UnDecorateSymbolName(typeid(T).name(), undecorated_name, sizeof(undecorated_name) / sizeof(*undecorated_name), UNDNAME_COMPLETE))
    {
        assert(0);
    }
    const char *realname = undecorated_name;
    TrackStamp(p, stamp, realname);
    */
    TrackStamp(p, stamp, typeid(T).name());

#elif eCONFIG_OS == eCONF_OS_LINUX
    int status;
    const char *realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    assert(status == 0);
    TrackStamp(p, stamp, realname);
#endif

    return p;
}

/* redfine new macro */

#define MEMTRACK_NEW MemStamp(__FILE__, __LINE__) * new
#define new MEMTRACK_NEW

#endif

#endif    // MEMORYTRACK_HPP
