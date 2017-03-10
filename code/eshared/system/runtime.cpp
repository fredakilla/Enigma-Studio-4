/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   This file is part of
 *       ______        _                             __ __
 *      / ____/____   (_)____ _ ____ ___   ____ _   / // /
 *     / __/  / __ \ / // __ `// __ `__ \ / __ `/  / // /_
 *    / /___ / / / // // /_/ // / / / / // /_/ /  /__  __/
 *   /_____//_/ /_//_/ \__, //_/ /_/ /_/ \__,_/     /_/.   
 *                    /____/                              
 *
 *   Copyright © 2003-2012 Brain Control, all rights reserved.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "types.hpp"
#include "runtime.hpp"

#if defined(eRELEASE) && defined(ePLAYER)
ePtr eCDECL operator new(size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

ePtr eCDECL operator new [](size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void eCDECL operator delete(ePtr ptr)
{
    HeapFree(GetProcessHeap(), 0, ptr);
}

void eCDECL operator delete [] (ePtr ptr)
{
    HeapFree(GetProcessHeap(), 0, ptr);
}
#else
#undef new
#undef delete
#include <crtdbg.h>
#include <malloc.h>
#undef new
#undef delete

static eU64 g_allocedMem = 0;
static eU64 g_allocCount = 0;

ePtr operator new(size_t size, const eChar *file, eU32 line)
{
#ifdef eDEBUG
    ePtr ptr = _malloc_dbg(size, _NORMAL_BLOCK, file, line);
#else
    ePtr ptr = malloc(size);
#endif

#ifdef eEDITOR
    g_allocedMem += _msize(ptr);
    g_allocCount++;
#endif

    return ptr;
}

ePtr eCDECL operator new [](size_t size, const eChar *file, eU32 line)
{
    return ::operator new(size, file, line);
}

ePtr eCDECL operator new(size_t size)
{
    return ::operator new(size, "", 0);
}

void eCDECL operator delete(ePtr ptr)
{
    if (!ptr)
        return;

#ifdef eEDITOR
    g_allocedMem -= _msize(ptr);
    g_allocCount--;
#endif

#ifdef eDEBUG
    _free_dbg(ptr, _NORMAL_BLOCK);
#else
    free(ptr);
#endif
}

void eCDECL operator delete [] (ePtr ptr)
{
    ::operator delete(ptr);
}

void eCDECL operator delete (ePtr ptr, const eChar *file, eU32 line)
{
    ::operator delete [] (ptr);
}

void eCDECL operator delete [] (ePtr ptr, const eChar *file, eU32 line)
{
    ::operator delete(ptr);
}
#endif

ePtr eAllocAlignedAndZero(size_t size,size_t alignment)
{
    // allocate memory size+alignment+sizeof(eU32) bytes.
    ePtr p0 = new eU8[size+alignment+sizeof(size_t)];
    eMemSet(p0,0,size+alignment+sizeof(size_t));

    // find aligned memory address as multiples of alignment
    const size_t addr = (size_t)p0+alignment+sizeof(size_t);
    ePtr p1 = (ePtr)(addr-(addr%alignment));

    // store address returned by malloc 4 byte above
    // pointer to be returned (useful when freeing)
    *((size_t *)p1-1) = (size_t)p0;
    return p1;
}

void eFreeAligned(ePtr ptr)
{
    if (ptr)
    {
        // find address of original allocation
        ePtr realPtr = (ePtr)(*((size_t *)ptr-1));
        eDeleteArray(realPtr);
    }
}

#ifndef ePLAYER
eLogHandler g_logHandler = nullptr;
ePtr g_logHandlerParam = nullptr;

void eSetLogHandler(eLogHandler logHandler, ePtr param)
{
    g_logHandler = logHandler;
    g_logHandlerParam = param;
}
#endif

void eWriteToLog(const eChar *msg)
{
#ifdef eEDITOR
    if (g_logHandler)
        g_logHandler(msg, g_logHandlerParam);
#endif

	// always output message to console
    OutputDebugString(msg);
    OutputDebugString("\n");
}

#ifndef ePLAYER
eU64 eGetAllocatedMemory()
{
    return g_allocedMem;
}

eU64  eGetTotalVirtualMemory()
{
    MEMORYSTATUSEX mse;
    eMemSet(&mse, 0, sizeof(mse));
    mse.dwLength = sizeof(mse);
    GlobalMemoryStatusEx(&mse);
    return mse.ullTotalVirtual;
}
#endif

void eLeakDetectorStart()
{
#ifdef eDEBUG
    const eInt curFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    _CrtSetDbgFlag(curFlags|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
#endif
}

void eLeakDetectorStop()
{
#ifdef eDEBUG
    const eInt oldFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    _CrtSetDbgFlag(oldFlags & ~(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF));
#endif
}

#ifdef eDEBUG
// returns if user pressed the "try again" button
eBool eShowAssertion(const eChar *expr, const eChar *file, eU32 line)
{
    eChar program[256];
    GetModuleFileName(GetModuleHandle(nullptr), program, 256);

    eChar text[1024];
    eStrCopy(text, "Debug assertion failed!\n\nProgram: ");
    eStrAppend(text, program);
    eStrAppend(text, "\nFile: ");
    eStrAppend(text, file);
    eStrAppend(text, "\nLine: ");
    eStrAppend(text, eIntToStr(line));
    eStrAppend(text, "\n\nExpression: ");
    eStrAppend(text, expr);
    eStrAppend(text, "\n\nPress retry to debug the application, cancel to stop execution\n"
                     "and continue to ignore assertion.");

    const eInt button = MessageBox(nullptr, text, "Enigma - Assertion",
                                   MB_TASKMODAL|MB_ICONERROR|MB_CANCELTRYCONTINUE);

    if (button == IDTRYAGAIN)
        return eTRUE;
    else if (button == IDCANCEL)
        eFatal(-1);

    return eFALSE;
}
#endif

void eShowError(const eChar *error)
{
    MessageBox(nullptr, error, "Enigma - Error", MB_TASKMODAL|MB_ICONERROR);
}

void eFatal(eU32 exitCode)
{
    ExitProcess(exitCode);
}

ePtr eMemRealloc(ePtr ptr, size_t oldLength, size_t newLength)
{
    if(newLength <= oldLength && ptr)
        return ptr;

    return realloc(ptr,newLength);

    /*// reallocation necessary?
    if (newLength <= oldLength && ptr)
        return ptr;

    ePtr newPtr = new eU8[newLength];

    if (ptr)
    {
        eU8 *bptr = (eU8 *)ptr;
        eMemCopy(newPtr, bptr, oldLength);
        eDeleteArray(bptr);
    }

    return newPtr;*/
}

void eMemSet(ePtr dst, eU8 val, size_t count)
{
    memset(dst, val, count);
}

void eMemCopy(ePtr dst, eConstPtr src, size_t count)
{
    memcpy(dst,src,count);
/*
#ifdef eUSE_MMX
    memcpy(dst, src, count);
#else
    eU8 *pd = (eU8 *)dst;
    const eU8 *ps = (eU8 *)src;

    while (count--)
        *pd++ = *ps++;
#endif*/
}

void eMemMove(ePtr dst, eConstPtr src, size_t count)
{
    memmove(dst, src, count);

    /*const eU8 *psrc = (const eU8 *)src;
    eU8 *pdst = (eU8 *)dst;

    if (dst <= src || pdst >= psrc+count)
    {
        // non-overlapping buffers, so copy from
        // lower addresses to higher addresses
        while (count--)
            *pdst++ = *psrc++;
    }
    else
    {
        // overlapping buffers, so copy from
        // higher addresses to lower addresses
        pdst = pdst+count-1;
        psrc = psrc+count-1;

        while (count--)
            *pdst-- = *psrc--;
    }*/
}

eBool eMemEqual(eConstPtr mem0, eConstPtr mem1, size_t count)
{
    return !memcmp(mem0, mem1, count);

   /* const eU8 *ptr0 = (eU8 *)mem0;
    const eU8 *ptr1 = (eU8 *)mem1;

    for(size_t i=0; i<count; i++)
        if (ptr0[i] != ptr1[i])
            return eFALSE;

    return eTRUE;*/
}

void eStrClear(eChar *str)
{
    str[0] = '\0';
}

void eStrCopy(eChar *dst, const eChar *src)
{
    while (*dst++ = *src++);
}

void eStrNCopy(eChar *dst, const eChar *src, eU32 count)
{
    // copy string characters
    while (count && (*dst++ = *src++))
        count--;

    // pad out with zeros
    if (count)
        eMemSet(dst, '\0', count-1);
}

eChar * eStrClone(const eChar *str)
{
    eChar *clone = new eChar[eStrLength(str)+1];
    eStrCopy(clone, str);
    return clone;
}

eU32 eStrLength(const eChar *str)
{
    const eChar *eos = str;
    while (*eos++);
    return (eU32)(eos-str-1);
}

eChar * eStrAppend(eChar *dst, const eChar *src)
{
    // find end of source string
    eChar *pd = dst;

    while (*pd)
        pd++;

    eStrCopy(pd, src);
    return dst;
}

// compares two strings and returns an integer to
// indicate whether the first is less than the second,
// the two are equal, or whether the first is greater
// than the second. comparison is done byte by byte on
// an unsigned basis which is to say that null (0) is
// less than any other character (1-255).
//
// returns -1 if first string < second string.
// returns  0 if first string = second string.
// returns +1 if first string > second string.
eInt eStrCompare(const eChar *str0, const eChar *str1)
{
    eInt res = 0;

    while (!(res = *(eU8 *)str0-*(eU8 *)str1) && *str1)
    {
        str0++;
        str1++;
    }

    if (res < 0)
        res = -1;
    else if (res > 0)
        res = 1;

    return res;
}

eChar * eStrUpper(eChar *str)
{
    const eU32 strLen = eStrLength(str);
    for (eU32 i=0; i<strLen; i++)
    {
        eChar &c = str[i];
        if (c >= 'a' && c <= 'z')
            c -= 32;
    }

    return str;
}

eChar * eIntToStr(eInt val)
{
    // remember if integer is negative and
    // if it is, make it positive
    const eBool negative = (val < 0);

    if (negative)
        val = -val;

    // 12 spaces are enough for 32-bit decimal
    // (10 digits + 1 null terminator byte +
    // eventually a sign character)
    static eChar str[12];

    eChar *cp = str+sizeof(str)-1;
    *cp = '\0';

    do
    {
        *(--cp) = val%10+'0';
        val /= 10;
    }
    while (val > 0);

    // prepend negative sign character
    if (negative)
        *(--cp) = '-';

    return cp; 
}

#if !defined(ePLAYER) || !defined(eRELEASE)
#include <sstream>

eChar * eFloatToStr(eF32 val)
{
    static eChar str[20];
    std::ostringstream ss;
    ss << val;
    eStrCopy(str, ss.str().c_str());
    return str;
}
#endif

eInt eStrToInt(const eChar *str)
{
    eASSERT(eStrLength(str) > 0);

    const eBool neg = (str[0] == '-');
    eChar c;
    eInt val = 0;

    while ((c = *str++) != '\0' && eIsDigit(c))
        val = val*10+(c-'0');

    return (neg ? -val : val);
}

eF32 eStrToFloat(const eChar *str)
{
    eASSERT(eStrLength(str) > 0);

    eF32 val = 0.0f;
    eInt sign = 1;

    if (*str == '+')
        str++;
    else if (*str == '-')
    {
        sign = -1;
        str++;
    }

    while (eIsDigit(*str))
        val = val*10+(*str++)-'0';

    if (*str == '.')
    {
        str++;
        eF32 dec = 1.0f;

        while (eIsDigit(*str))
        {
            dec = dec/10.0f;
            val = val+((*str++)-'0')*dec;
        }
    }

    return val*sign;
}

eBool eIsAlphaNumeric(eChar c)
{
	return eIsAlpha(c) || eIsDigit(c);
}

eBool eIsAlpha(eChar c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

eBool eIsDigit(eChar c)
{
    return (c >= '0' && c <= '9');
}

// seed value of the random number generator
static eU32 g_seed = 1;
static eU32 g_threadId = 0;

void eRandomize(eU32 seed)
{
    // seed may not be 0
    g_seed = seed+1;

    // if seed is zero because of an
    // overflow set seed to 1
    if (!g_seed)
        g_seed = 1;
}

eU32 eRandomSeed()
{
    return GetTickCount();
}

eU32 eRandom()
{
    return eRandom(g_seed);
}

// park-Miller random number generation
// ("minimal standard" random generator).
// random numbers are in range 1,..,m-1.
// calculates (a*16807 mod 0x7fffffff).
eU32 eRandom(eU32 &seed)
{
    eU32 lo = 16807*(seed&0xffff);
    eU32 hi = 16807*(seed>>16);
    lo += (hi&0x7fff)<<16;
    hi >>= 15;
    lo += hi;
    lo = (lo&0x7FFFFFFF)+(lo>>31);
    seed = lo;
    return seed;
}

// should be much more precise with large b
inline double fastPrecisePow(double a,double b) 
{
    // calculate approximation with fraction of the exponent
    int e = (int)b;
    union {
        double d;
        int x[2];
    } u ={ a };
    u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;

    // exponentiation by squaring with the exponent's integer part
    // double r = u.d makes everything much slower, not sure why
    double r = 1.0;
    while(e) {
        if(e & 1) {
            r *= a;
        }
        a *= a;
        e >>= 1;
    }

    return r * u.d;
}

inline double fastPow(double a,double b) 
{
    union {
        double d;
        int x[2];
    } u ={ a };
    u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

// faster pow based on code by agner fog
// code taken from chaos/farbrausch
eF64 ePow64(eF64 a, eF64 b)
{
    //return pow(a,b);
    return pow(a,b);
}

eF32 ePow(eF32 base, eF32 exp)
{
    return powf(base, exp);
    //return (eF32)ePow64(base, exp);  
    //return fastPow(base,exp);
    //return (eF32)fastPrecisePow((eF32)base,(eF32)exp);

}

eF32 eSinH(eF32 x)
{
    const eF32 v = eExp(x);
    return 0.5f*(v-1.0f/v);
}

eF32 eCosH(eF32 x)
{
    const eF32 v = eExp(x);
    return 0.5f*(v+1.0f/v);
}

eF32 eTanH(eF32 x)
{
    const eF32 v = eExp(2.0f*x);
    return (v-1.0f)/(v+1.0f);
}

// arcus sine with -1 <= x <= 1
eF32 eASin(eF32 x)
{
	return (eF32)asinf(x);
}

// arcus cosine with -1 <= x <= 1
eF32 eACos(eF32 x)
{
	return (eF32)acosf(x);
}

eF32 eExp(eF32 x)
{
	return (eF32)expf(x);
}

// rounds down towards -inf (e.g. floor(-2.2) = -3)
eInt eFloor(eF32 x)
{
	return (eInt)floorf(x);
}

// rounds up towards +inf (e.g. ceil(-2.2) = -2)
eInt eCeil(eF32 x)
{
	return (eInt)ceilf(x);
}

eBool eIsNumber(eF32 x)
{
   return (!eIsNan(x)) && (x <= eF32_MAX && x >= -eF32_MAX); 
}

// returns base 10 logarithm (x > 0)
eF32 eLog10(eF32 x)
{
	return (eF32)log10f(x);
}

eF32 eLog2(eF32 x)
{
	return (eF32)log2f(x);
}

// returns base e logarithm (x > 0)
eF32 eLn(eF32 x)
{
	return (eF32)logf(x);
}

eF32 eSin(eF32 x)
{
	return (float)sinf(x);
}

eF32 eCos(eF32 x)
{
	return (float)cosf(x);
}

eF32 eTan(eF32 x)
{
	return (float)tanf(x);
}

eF32 eCot(eF32 x)
{
    eASSERT(!eIsFloatZero(eTan(x)));
    return 1.0f/eTan(x);
}

eF32 eATan(eF32 x)
{
	return (float)atanf(x);
}

eF32 eATan2(eF32 y, eF32 x)
{
	return (float)atan2f(y,x);
}

eF32 eATanh(eF32 x)
{
    return 0.5f*eLn((1.0f+x)/(1.0f-x));
}

void eSinCos(eF32 x, eF32 &sine, eF32 &cosine)
{
	sine = sinf(x);
	cosine = cosf(x);
}

eF32 eSqrt(eF32 x)
{
	return (float)sqrtf(x);
}

eF32 eInvSqrt(eF32 x)
{
    eASSERT(x >= 0.0f);
    return 1.0f/eSqrt(x);
}

// returns the floating point remainder of a
// and b (so a=i*b+f, with f being returned
// and i is an integer value).
eF32 eMod(eF32 a, eF32 b)
{
	return fmodf(a, b);
}

eBool eIsFloatZero(eF32 x)
{
    return (eAbs(x) < eALMOST_ZERO);
}

eBool eAreFloatsEqual(eF32 x, eF32 y)
{
    return eIsFloatZero(x-y);
}

eBool eIsNan(eF32 x)
{
   volatile float temp = x; // avoid compiler optimization
   return (temp != x);
}

eF32 eDegToRad(eF32 degrees)
{
    return degrees/360.0f*eTWOPI;
}

eF32 eRadToDeg(eF32 radians)
{
    return radians/eTWOPI*360.0f;
}

// returns wether or not the given
// pointer is properly aligned
eBool eIsAligned(eConstPtr data, eU32 alignment)
{
    // check that the alignment is a power-of-two
    eASSERT((alignment&(alignment-1)) == 0); 
    return (((eU32)data&(alignment-1)) == 0);
}

// hashing functions for integers. can be used
// to build more complex hashing functions for
// different data types.
eU32 eHashInt(eInt key)
{
    eU32 hash = (eU32)key;
    hash = (hash^61)^(hash>>16);
    hash = hash+(hash<<3);
    hash = hash^(hash>>4);
    hash = hash*0x27d4eb2d;
    hash = hash^(hash>>15);
    return hash;
}

// Implements the DJB2 hash function as first
// reported by Dan Bernstein.
eU32 eHashStr(const eChar *str)
{
    eU32 hash = 5381;
    eChar c;
    
    while (c = *str++)
        hash = ((hash<<5)+hash)+c; // does a hash*33+c

    return hash;
}

eF32 eRound(eF32 x)
{
    return roundf(x);
}

eU32 eRoundToMultiple(eU32 x, eU32 multiple)
{
    eASSERT(multiple > 0);

    const eU32 remainder = x%multiple;

    if (remainder == 0)
        return x;
    else
        return x+multiple-remainder;
}

// rounds up or down towards zero
// (truncates fractional part)
eInt eTrunc(eF32 x)
{
	return (eInt)trunc(x);
}

eInt eRandom(eInt min, eInt max)
{
    return eRandom()%(max-min)+min;
}

eInt eRandom(eInt min, eInt max, eU32 &seed)
{
    return eRandom(seed)%(max-min)+min;
}

// returns a random number in range [1/MAX_RAND,1-1/MAX_RAND]
eF32 eRandomF()
{
    return (eF32)eRandom()/(eF32)eMAX_RAND;
}

eF32 eRandomF(eU32 &seed)
{
    return (eF32)eRandom(seed)/(eF32)eMAX_RAND;
}

eF32 eRandomF(eF32 min, eF32 max)
{
    return eRandomF()*(max-min)+min;
}

eF32 eRandomF(eF32 min, eF32 max, eU32 &seed)
{
    return eRandomF(seed)*(max-min)+min;
}

eU32 eNextPowerOf2(eU32 x)
{
    x--;
    x = (x>>1) | x;
    x = (x>>2) | x;
    x = (x>>4) | x;
    x = (x>>8) | x;
    x = (x>>16) | x;
    x++;
    return x;
}

eBool eIsPowerOf2(eU32 x)
{
    return !(x&(x-1));
}

eBool eClosedIntervalsOverlap(eInt start0, eInt end0, eInt start1, eInt end1)
{
    return (start1 <= end0 && start0 <= end1);
}

//---------------------------------------------------------------------------------------------
// print short text using std::cout using printf style
// use this print function to redirect text to log editor
//---------------------------------------------------------------------------------------------
eChar* ePrintMsg(const eChar *fmt, ...)
{
    static eChar buffer[255] = "";

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf(buffer, fmt, argptr);
    va_end(argptr);

    eWriteToLog(buffer);

    return buffer;
}
