#include <Windows.h>

#include "../eshared/eshared.hpp"

eInt WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, eChar *cmdLine, eInt showCmd)
{
    ePROFILER_ADD_THIS_THREAD("Main thread");

    eEngine engine;

    const eU32 wndFlags = eWF_VSYNC;
    engine.openWindow(wndFlags, eSize(800, 600), nullptr);
    eGfx->setWindowTitle("eEngine");

    eTimer timer;
    eMessage msg = eMSG_IDLE;

    while (msg != eMSG_QUIT)
    {
        eGfx->handleMessages(msg);

        if (msg == eMSG_IDLE)
        {
            const eF32 time = (eF32)timer.getElapsedMs()*0.001f;

            eGfx->beginFrame();

            eGfx->clear(eCM_COLOR, eCOL_BLUE);

            eGfx->endFrame();
        }
    }

    return 0;
}
