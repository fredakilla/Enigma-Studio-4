#include <Windows.h>

#include "../eshared/eshared.hpp"


static eSimpleVtx s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f,  0.0f, 1.0f },
    { 1.0f,  1.0f,  1.0f,  0.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f,  0.0f, 1.0f },
    { 1.0f, -1.0f,  1.0f,  0.0f, 1.0f },
    {-1.0f,  1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f,  1.0f, -1.0f,  0.0f, 1.0f },
    {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
};

static eU16 s_cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
};



eInt WINAPI WinMain(HINSTANCE, HINSTANCE, eChar *, eInt)
{
    ePROFILER_ADD_THIS_THREAD("Main thread");

    eString wndTitle = "StaticGeometryCube";
    eEngine engine;
    eTimer timer;
    eInt nbFrames = 0;
    eF32 lastTime = (eF32)timer.getElapsedMs()*0.001f;

    engine.openWindow(eWF_NONE, eSize(800, 600), nullptr);
    eGfx->setWindowTitle(wndTitle);


    // Init

    eGeometryDx11* m_geo = eGfx->addGeometry(eGEO_STATIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST);

    eSimpleVtx* vp = nullptr;
    eU16* ip = nullptr;

    eGfx->beginLoadGeometry(m_geo, 8, (ePtr*)&vp, 12*3, (ePtr*)&ip);
    {
        eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*8 );
        eMemCopy(ip, &s_cubeTriList[0], sizeof(eU16)*12*3 );

        /*vp[0] = {-1.0f,  1.0f,  1.0f,  0.0f, 1.0f };
        vp[1] = { 1.0f,  1.0f,  1.0f,  0.0f, 1.0f };
        vp[2] = {-1.0f, -1.0f,  1.0f,  0.0f, 1.0f };
        vp[3] = { 1.0f, -1.0f,  1.0f,  0.0f, 1.0f };
        vp[4] = {-1.0f,  1.0f, -1.0f,  0.0f, 1.0f };
        vp[5] = { 1.0f,  1.0f, -1.0f,  0.0f, 1.0f };
        vp[6] = {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f };
        vp[7] = { 1.0f, -1.0f, -1.0f,  0.0f, 1.0f };*/

    }
    eGfx->endLoadGeometry(m_geo);


    ePixelShader* m_psQuad = eGfx->loadPixelShader(eSHADER(ps_quad));
    eVertexShader* m_vsQuad = eGfx->loadVertexShader(eSHADER(vs_quad));

    eTexture2dDx11* tex = eGfx->createChessTexture(64, 64, 16, eCOL_CYAN, eCOL_ORANGE);


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
            eCamera cam(45.0f, (eF32)800/600, 0.1f, 1000.0f);
            eMatrix4x4 mtx;
            mtx.lookAt(eVector3(2,2,-5), eVector3(0,0,0), eVector3(0,1,0));
            cam.setViewMatrix(mtx);
            cam.activate();

            // render geometry
            eGfx->renderGeometry(m_geo);

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

    eGfx->removeGeometry(m_geo);
    eGfx->removeTexture2d(tex);

    return 0;
}