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

#include "system.hpp"

eU64  eTimer::m_freq   = 0;
eBool eTimer::m_inited = eFALSE;



eTimer::eTimer() :
    m_histIndex(0),
    m_histCount(0)
{
    _initialize();
#if eCONFIG_OS == eCONF_OS_WINDOWS
    QueryPerformanceCounter((LARGE_INTEGER *)&m_startTime);
#else
    //m_startTime.tv_nsec = m_startTime.tv_sec = 0;
    //clock_gettime(CLOCK_REALTIME, &m_startTime);
    //m_startTime.tv_sec = m_startTime.tv_usec = 0;

    gettimeofday(&m_startTime, NULL);
#endif
}

eF32 eTimer::restart()
{
    const eF32 elapsedMs = getElapsedMs();
    m_elapsedHist[m_histIndex] = elapsedMs;
    m_histIndex = (m_histIndex+1)%eELEMENT_COUNT(m_elapsedHist);
    m_histCount = eMin((eU32)m_histCount+1,(eU32)eELEMENT_COUNT(m_elapsedHist));

#if eCONFIG_OS == eCONF_OS_WINDOWS
    QueryPerformanceCounter((LARGE_INTEGER *)&m_startTime);
#else
    //clock_gettime(CLOCK_REALTIME, &m_startTime);

    gettimeofday(&m_startTime, NULL);
#endif

    return elapsedMs;
}

eF32 eTimer::getElapsedMs() const
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    eU64 curTime;
    QueryPerformanceCounter((LARGE_INTEGER *)&curTime);
    return (eF32)((eF64)(curTime-m_startTime)/(eF64)m_freq*1000.0);
#else
    //struct timespec curTime;
    //clock_gettime(CLOCK_REALTIME, &curTime);
    //return (eF32)((eF64)(curTime.tv_sec - m_startTime.tv_sec)/(eF64)m_freq*1000.0);

    // compute time in microseconds
    eU64 usec;
    timeval currentTime, diff;
    gettimeofday(&currentTime, NULL);
    timersub(&currentTime, &m_startTime, &diff);
    usec = (eU64)diff.tv_sec * 1e06 + diff.tv_usec;

    // return in milliseconds
    return (eF32)(usec)/1000.0f;
#endif
}

eF32 eTimer::getAvgElapsedMs() const
{
    eF32 avg = 0.0f;

    if (m_histCount > 0)
    {
        for (eU32 i=0; i<m_histCount; i++)
            avg += m_elapsedHist[i];

        avg /= (eF32)m_histCount;
    }

    return eMax(avg, 0.1f); // minimum 1 micro-second to avoid division-by-zero
}

eU32 eTimer::getTimeMs()
{
    return eFtoL((eF32)((eF64)getTickCount()/(eF64)m_freq*1000.0));
}

eU64 eTimer::getTickCount()
{
#if eCONFIG_OS == eCONF_OS_WINDOWS
    eU64 curTime;
    QueryPerformanceCounter((LARGE_INTEGER *)&curTime);
    return curTime;
#else
    //struct timespec curTime;
    //clock_gettime(CLOCK_REALTIME, &curTime);
    //return (eU64)curTime.tv_sec;

    timeval curTime;
    gettimeofday(&curTime, NULL);
    return eDtoULL(curTime.tv_usec);
#endif
}

eU64 eTimer::getFrequency()
{
    _initialize(); // make sure the frequency is valid
    return m_freq;
}

void eTimer::_initialize()
{
    if (!m_inited)
    {
#if eCONFIG_OS == eCONF_OS_WINDOWS
        const BOOL res = QueryPerformanceFrequency((LARGE_INTEGER *)&m_freq);
        eASSERT(res);
#else
        //struct timespec freq;
        //clock_getres(CLOCK_REALTIME, &freq);
        m_freq = 1000000L;
#endif
        m_inited = eTRUE;
    }
}





