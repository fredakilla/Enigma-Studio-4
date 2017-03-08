#include <Windows.h>

#include "../eshared/eshared.hpp"


eInt WINAPI WinMain(HINSTANCE, HINSTANCE, eChar *, eInt)
{
    ePROFILER_ADD_THIS_THREAD("Main thread");

    eString wndTitle = "StaticGeometry";
    eEngine engine;
    eTimer timer;
    eInt nbFrames = 0;
    eF32 lastTime = (eF32)timer.getElapsedMs()*0.001f;

    engine.openWindow(eWF_NONE, eSize(800, 600), nullptr);
    eGfx->setWindowTitle(wndTitle);


    // Init

    eGeometryDx11* m_geoQuad = eGfx->addGeometry(eGEO_STATIC, eVTX_SIMPLE, eGPT_QUADLIST);

    const eRect r(10,10,256,256);

    eSimpleVtx *vp = nullptr;
    eGfx->beginLoadGeometry(m_geoQuad, 4, (ePtr *)&vp);
    {
        vp[0].set(eVector3((eF32)r.left,  (eF32)r.top,    0.0f), eVector2(0, 0));
        vp[1].set(eVector3((eF32)r.left,  (eF32)r.bottom, 0.0f), eVector2(1, 0));
        vp[2].set(eVector3((eF32)r.right, (eF32)r.bottom, 0.0f), eVector2(1, 1));
        vp[3].set(eVector3((eF32)r.right, (eF32)r.top,    0.0f), eVector2(0, 1));
    }
    eGfx->endLoadGeometry(m_geoQuad);

    ePixelShader* m_psQuad = eGfx->loadPixelShader(eSHADER(ps_quad));
    eVertexShader* m_vsQuad = eGfx->loadVertexShader(eSHADER(vs_quad));

    eTexture2dDx11* tex = eGfx->createChessTexture(64, 64, 8, eCOL_CYAN, eCOL_ORANGE);


    // Update

    eMessage msg = eMSG_IDLE;
    while (msg != eMSG_QUIT)
    {
        eGfx->handleMessages(msg);

        if (msg == eMSG_IDLE)
        {
            const eF32 time = (eF32)timer.getElapsedMs()*0.001f;

            eGfx->beginFrame();

            // clear
            eGfx->clear(eCM_ALL, eCOL_BLUE);

            // set render states
            eRenderState &rs = eGfx->freshRenderState();
            rs.targets[0] = eGraphics::TARGET_SCREEN;
            rs.cullMode = eCULL_NONE;
            rs.depthTest = eFALSE;
            rs.viewport.set(1, 1, 800, 600);
            rs.ps = m_psQuad;
            rs.vs = m_vsQuad;
            rs.textures[0] = tex;
            rs.texFlags[0] = eTMF_CLAMP | eTMF_NEAREST;

            // set viewport
            const eCamera cam(0.0f, (eF32)800, 0.0f, (eF32)600, -1.0f, 1.0f);
            cam.activate();

            // render geometry
            eGfx->renderGeometry(m_geoQuad);

            eGfx->endFrame();

            // show fps

            nbFrames++;
            if ( time - lastTime >= 1.0 )
            {
                eString title = wndTitle;
                title += " - ";
                title += eIntToStr( nbFrames );
                title += " fps";

                eGfx->setWindowTitle(title);

                nbFrames = 0;
                lastTime += 1.0f;
            }
        }
    }

    // Clean

    eGfx->removeGeometry(m_geoQuad);
    eGfx->removeTexture2d(tex);

    return 0;
}
