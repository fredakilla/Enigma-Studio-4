#ifndef COMPILER_HPP
#define COMPILER_HPP

// Compile time code detection
// see: http://sourceforge.net/p/predef/wiki/Home/

//---------------------------------------------------------------------------------------------
// Detect Operating System
// Supported OS : Windows, Linux.
//---------------------------------------------------------------------------------------------

#define eCONF_OS_WINDOWS    10
#define eCONF_OS_LINUX      11

#ifdef _WIN32
    // System Windows (32/64 bits)
    #define eCONFIG_OS  eCONF_OS_WINDOWS
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    // System Linux
    #define eCONFIG_OS  eCONF_OS_LINUX
#else
    #error Operating System not detected !
#endif

//---------------------------------------------------------------------------------------------
// Detect Processor
// Supported Proc : x86, x86_64.
//---------------------------------------------------------------------------------------------

#define eCONF_PROC_x32    20
#define eCONF_PROC_x64    21

#ifdef __i386
    // x86, 32 bits
    #define eCONFIG_PROC    eCONF_PROC_x32
#elif defined(__x86_64__) || defined(_M_X64)
    // x86_64, 64 bits
    #define eCONFIG_PROC    eCONF_PROC_x64
#else
    #error Processor type not detected !
#endif


//---------------------------------------------------------------------------------------------
// Compiler specific definitions
// Supported : MSVC, GCC
//---------------------------------------------------------------------------------------------

#define eCONF_COMPILER_MSC      30
#define eCONF_COMPILER_GCC      31

#ifdef _MSC_VER // Microsoft Visual Studio

    #define eCONFIG_COMPILER        eCONF_COMPILER_MSC
    #define eFASTCALL               __fastcall
    #define eFORCEINLINE            __forceinline
    #define eINLINE                 __inline
    #define eNORETURN               __declspec(noreturn)
    #define eALIGN16                __declspec(align(16))
    #define eNAKED                  __declspec(naked)
    #define eTHREADLOCAL            __declspec(thread)
    #define eCALLBACK               __stdcall
    #define eCDECL                  __cdecl
    inline void eDebugBreak()       { __debugbreak(); }

    typedef unsigned __int64        eU64TYPE;
    typedef __int64                 eS64TYPE;

#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER)) // GNU GCC/G++

    #define eCONFIG_COMPILER        eCONF_COMPILER_GCC
    #define eFASTCALL               __fastcall
    #define eFORCEINLINE            __attribute__((always_inline))
    #define eINLINE                 inline
    #define eNORETURN
    #define eALIGN16                __attribute__ ((aligned(16)))
    #define eNAKED
    #define eTHREADLOCAL
    #define eCALLBACK
    #define eCDECL
    inline void eDebugBreak()       {  __builtin_trap(); }

    #include <inttypes.h>
    typedef uint64_t                eU64TYPE;
    typedef int64_t                 eS64TYPE;

#else // other compiler not defined yet, implement me

    #error Compiler not detected !

#endif

#endif // COMPILER_HPP

