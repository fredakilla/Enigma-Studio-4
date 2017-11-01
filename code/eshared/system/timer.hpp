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

#ifndef TIMER_HPP
#define TIMER_HPP

#include "compiler.hpp"

#if eCONFIG_OS == eCONF_OS_WINDOWS          // Windows system specific
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#elif  eCONFIG_OS == eCONF_OS_LINUX         // Unix based system specific
    #include <sys/time.h>
#endif

class eTimer
{
public:
    eTimer();

    eF32            restart();
    eF32            getElapsedMs() const;
    eF32            getAvgElapsedMs() const;

public:
    static eU32     getTimeMs();
    static eU64     getTickCount();
    static eU64     getFrequency();

private:
    static void     _initialize();

private:
    static eU64     m_freq;
    static eBool    m_inited;

private:
    eF32            m_elapsedHist[60];
    eU32            m_histIndex;
    eU32            m_histCount;

#if eCONFIG_OS == eCONF_OS_WINDOWS
    eU64            m_startTime;
#else
    //struct timespec m_startTime;
    timeval         m_startTime;
#endif



};

#endif // TIMER_HPP

