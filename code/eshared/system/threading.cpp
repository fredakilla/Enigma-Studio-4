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

#include "threading.hpp"

#if eCONFIG_OS == eCONF_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

#include "system.hpp"

eThread::eThread(eInt flags, eThreadFunc threadFunc) :
    m_prio((eThreadPriority)(flags&(~eTHCF_SUSPENDED))),
    m_threadFunc(threadFunc)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    const eU32 tf = (flags&eTHCF_SUSPENDED ? CREATE_SUSPENDED : 0);
    m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_threadTrunk, this, tf, (LPDWORD)&m_tid);
    eASSERT(m_handle);
#else
#endif

#ifdef eEDITOR
    m_ctx.thread = this;
    m_ctx.tid = m_tid;
#ifdef eUSE_PROFILER
    m_ctx.profThread = nullptr;
#endif
#endif

    setPriority(m_prio);
}

eThread::~eThread()
{
    join();
}

void eThread::sleep(eU32 ms)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    Sleep(ms);
#else
#endif
}

void eThread::join()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    WaitForSingleObject((HANDLE)m_handle, INFINITE);
    CloseHandle((HANDLE)m_handle);
    m_handle = nullptr;
#else
#endif
}

void eThread::yield()
{
    sleep(1);
}

void eThread::resume()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    ResumeThread(m_handle);
#else
#endif
}

void eThread::suspend()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    SuspendThread(m_handle);
#else
#endif
}

void eThread::terminate(eU32 exitCode)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    TerminateThread(m_handle, exitCode);
#else
#endif
}

void eThread::setPriority(eThreadPriority prio)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    struct ClassPriority
    {
        eU32 cls;
        eU32 prio;
    }
    cp[] =
    {
        {BELOW_NORMAL_PRIORITY_CLASS, THREAD_PRIORITY_LOWEST},        // low
        {NORMAL_PRIORITY_CLASS,       THREAD_PRIORITY_NORMAL},        // normal
        {HIGH_PRIORITY_CLASS,         THREAD_PRIORITY_TIME_CRITICAL}, // high
    };

    SetPriorityClass(m_handle, cp[prio].cls);
    SetThreadPriority(m_handle, cp[prio].prio);
    m_prio = prio;
#else
#endif
}

eU32 eThread::getId() const
{
    return m_tid;
}

eThreadPriority eThread::getPriority() const
{
    return m_prio;
}

#ifdef eEDITOR
static eThreadCtx g_mainThreadCtx;
static eTHREADLOCAL eThreadCtx *g_curThreadCtx = nullptr;

eThreadCtx & eThread::getThisContext()
{
    return (g_curThreadCtx ? *g_curThreadCtx : g_mainThreadCtx);
}

eThreadCtx & eThread::getContext()
{
    return m_ctx;
}

const eThreadCtx & eThread::getContext() const
{
    return m_ctx;
}
#endif

eU32 eThread::operator () ()
{
    eASSERT(m_threadFunc);
    return m_threadFunc(this);
}

eU32 eThread::_threadTrunk(ePtr arg)
{
    eThread *thread = (eThread *)arg;
#ifdef eEDITOR
    g_curThreadCtx = &thread->m_ctx;
#endif
    return (*thread)();
}

eMutex::eMutex() :
    m_locked(eFALSE)
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    CRITICAL_SECTION *cs = new CRITICAL_SECTION;
    InitializeCriticalSection(cs);
    m_handle = (ePtr)cs;
#else

    //create mutex attribute variable
    pthread_mutexattr_t mAttr;

    // setup recursive mutex for mutex attribute
    pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);

    // Use the mutex attribute to create the mutex
    pthread_mutex_init(&m_handle, &mAttr);

    // Mutex attribute can be destroy after initializing the mutex variable
    pthread_mutexattr_destroy(&mAttr);

#endif
}

eMutex::~eMutex()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    eASSERT(!m_locked);
    CRITICAL_SECTION *cs = (CRITICAL_SECTION *)m_handle;
    DeleteCriticalSection(cs);
    eDelete(cs);
#else
    eASSERT(!m_locked);
    pthread_mutex_destroy(&m_handle);
#endif
}

void eMutex::enter()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    EnterCriticalSection((CRITICAL_SECTION *)m_handle);
    eASSERT(!m_locked);
    m_locked = eTRUE;
#else
    pthread_mutex_lock(&m_handle);
    eASSERT(!m_locked);
    m_locked = eTRUE;
#endif
}

void eMutex::tryEnter()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    if (TryEnterCriticalSection((CRITICAL_SECTION *)m_handle))
        m_locked = eTRUE;
#else
    if(pthread_mutex_trylock(&m_handle) == 0)
        m_locked = eTRUE;
#endif
}

void eMutex::leave()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    eASSERT(m_locked);
    m_locked = eFALSE;
    LeaveCriticalSection((CRITICAL_SECTION *)m_handle);
#else
    eASSERT(m_locked);
    m_locked = eFALSE;
    pthread_mutex_unlock(&m_handle);
#endif
}

eBool eMutex::isLocked() const
{
    return m_locked;
}
