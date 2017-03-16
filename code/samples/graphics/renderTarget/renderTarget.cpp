#include <Windows.h>

#include "../eshared/eshared.hpp"


static eSimpleVtx s_cubeVertices[24] =
{
    {-1.0f,  1.0f,  1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f, 0.0f },
    {-1.0f, -1.0f,  1.0f,  0.0f, 1.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, 1.0f },
    {-1.0f,  1.0f, -1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 0.0f },
    {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f,  1.0f,  1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f, 0.0f },
    {-1.0f,  1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f,  0.0f, 0.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, 0.0f },
    {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f,  1.0f, 1.0f },
    { 1.0f, -1.0f,  1.0f,  0.0f, 0.0f },
    { 1.0f,  1.0f,  1.0f,  1.0f, 0.0f },
    { 1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f,  0.0f, 0.0f },
    {-1.0f,  1.0f,  1.0f,  1.0f, 0.0f },
    {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
    {-1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
};

static const eU16 s_cubeIndices[36] =
{
     0,  2,  1,
     1,  2,  3,
     4,  5,  6,
     5,  7,  6,
     8, 10,  9,
     9, 10, 11,
    12, 13, 14,
    13, 15, 14,
    16, 18, 17,
    17, 18, 19,
    20, 21, 22,
    21, 23, 22,
};




// callback func use to fill geometry
void _fillGeoBuffers(eGeometry *geo, ePtr param)
{
    eSimpleVtx* vp = nullptr;
    eU16* ip = nullptr;

    eGfx->beginLoadGeometry(geo, 24, (ePtr*)&vp, 36, (ePtr*)&ip);
    {
        eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*24 );
        eMemCopy(ip, &s_cubeIndices[0], sizeof(eU16)*36 );
    }
    eGfx->endLoadGeometry(geo);
}


eInt WINAPI WinMain(HINSTANCE, HINSTANCE, eChar *, eInt)
{
    ePROFILER_ADD_THIS_THREAD("Main thread");

    eString wndTitle = "DynamicGeometry";
    eEngine engine;
    eTimer timer;
    eInt nbFrames = 0;
    eF32 lastTime = (eF32)timer.getElapsedMs()*0.001f;

    engine.openWindow(eWF_NONE, eSize(800, 600), nullptr);
    eGfx->setWindowTitle(wndTitle);




    // Init --------------------------------------------------------------------------------------------------


    // create quad
    eGeometryDx11* m_geoQuad = eGfx->addGeometry(eGEO_STATIC, eVTX_SIMPLE, eGPT_QUADLIST);

    const eRect r(0,0,256,256);

    eSimpleVtx *vp = nullptr;
    eGfx->beginLoadGeometry(m_geoQuad, 4, (ePtr *)&vp);
    {
        /*
         *  CCW
         *  0----2
         *  |  / |
         *  | /  |
         *  1----3
         *
        */

        /*vp[0].set(eVector3((eF32)r.left,  (eF32)r.top,    0.0f), eVector2(0, 0));
        vp[1].set(eVector3((eF32)r.left,  (eF32)r.bottom, 0.0f), eVector2(0, 1));
        vp[2].set(eVector3((eF32)r.right, (eF32)r.top,    0.0f), eVector2(1, 0));
        vp[3].set(eVector3((eF32)r.right, (eF32)r.bottom, 0.0f), eVector2(1, 1));*/

       /*
        * CW
        *  0----1
        *  |  / |
        *  | /  |
        *  2----3
       */

        vp[0].set(eVector3((eF32)r.left,  (eF32)r.top,    0.0f), eVector2(0, 0));
        vp[1].set(eVector3((eF32)r.right, (eF32)r.top,    0.0f), eVector2(1, 0));
        vp[2].set(eVector3((eF32)r.left,  (eF32)r.bottom, 0.0f), eVector2(0, 1));
        vp[3].set(eVector3((eF32)r.right, (eF32)r.bottom, 0.0f), eVector2(1, 1));

    }
    eGfx->endLoadGeometry(m_geoQuad);







    // create cube geometry
    eGeometryDx11* m_geo =  eGfx->addGeometry(eGEO_DYNAMIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST, _fillGeoBuffers);

    // create shaders for simple geometry (pos, uv)
    eVertexShader* m_vsQuad = eGfx->loadVertexShader(eSHADER(vs_quad));
    ePixelShader* m_psQuad = eGfx->loadPixelShader(eSHADER(ps_quad));

    // create texture
    eTexture2dDx11* tex = eGfx->createChessTexture(64, 64, 16, eColor(0,255,0,100), eCOL_ORANGE);

    // create render target
    eTexture2dDx11* m_rtDiffuse = eGfx->addTexture2d(256, 256, eTEX_TARGET, eTFO_ARGB16F);








    // Update --------------------------------------------------------------------------------------------------

    eMessage msg = eMSG_IDLE;
    while (msg != eMSG_QUIT)
    {
        eGfx->handleMessages(msg);

        if (msg == eMSG_IDLE)
        {
            const eF32 time = (eF32)timer.getElapsedMs()*0.001f;

            // Set view 0 default viewport.
            //bgfx::setViewRect(0, 0, 0, uint16_t(800), uint16_t(600) );

            // begin frame
            eGfx->beginFrame();

            // clear



            eGfx->clear(eCM_ALL, eCOL_BLUE);

            // render cube
            for(int i=0; i<5; i++)
            {
                /*bgfx::setViewClear(RTVIEW_ID
                            , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                            , 0x30303055, 1.0f, 0
                );



                bgfx::setViewRect(RTVIEW_ID, 0, 0, 128, 128);
                bgfx::setViewFrameBuffer(RTVIEW_ID, m_rtDiffuse->frameBufferHandle);*/
                //bgfx::setViewTransform(1, eGfx->getViewMatrix(), eGfx->getProjMatrix());





                // set render states
                eRenderState &rs = eGfx->freshRenderState();
                rs.targets[1] = m_rtDiffuse; //*/eGraphics::TARGET_SCREEN;
                rs.cullMode = eCULL_NONE;
                rs.depthTest = eTRUE;
                rs.viewport.set(0, 0, 800, 600);
                rs.ps = m_psQuad;
                rs.vs = m_vsQuad;
                rs.textures[0] = tex;
                rs.texFlags[0] = eTMF_CLAMP | eTMF_NEAREST;


                // set viewport
                eCamera cam(45.0f, (eF32)800/600, 0.1f, 1000.0f);
                eMatrix4x4 mtx;
                mtx.lookAt(eVector3(0,8,-10), eVector3(0,0,0), eVector3(0,1,0));
                cam.setViewMatrix(mtx);
                eTransform trans;
                trans.rotate(eQuat(eVector3(time, time, time)));
                trans.translate(eVector3(-5+3*i,0,0));
                cam.activate(trans.getMatrix());

                // render cube geo
                eGfx->renderGeometry(m_geo);
            }





            // render quad
            {
                // set render states
                /*eRenderState &rs = eGfx->freshRenderState();
                rs.targets[0] = eGraphics::TARGET_SCREEN;
                rs.cullMode = eCULL_NONE;
                rs.depthTest = eTRUE;
                rs.viewport.set(0, 0, 800, 600);
                rs.ps = m_psQuad;
                rs.vs = m_vsQuad;
                rs.textures[0] = m_rtDiffuse;
                rs.texFlags[0] = eTMF_CLAMP | eTMF_NEAREST;


                 /*eRenderState &rs = eGfx->getRenderState();
                rs.targets[0] = eGraphics::TARGET_SCREEN;
                rs.textures[0] = m_rtDiffuse;
                rs.cullMode = eCULL_NONE;*/

               /*const eCamera cam(0.0f, (eF32)800, 0.0f, (eF32)600, -1.0f, 1.0f);
                cam.activate();

                eGfx->renderGeometry(m_geoQuad);*/

                //eGfx->clear(eCM_ALL, eCOL_RED);

                /*eRenderState &rs = eGfx->getRenderState();
                rs.targets[0] = eGraphics::TARGET_SCREEN;
                eRenderer->renderQuad(eRect(0,0,256,256), eSize(800,600), m_rtDiffuse);
*/
                // copy to screen
                eRenderState &rs = eGfx->freshRenderState();
                rs.targets[0] = eGraphics::TARGET_SCREEN;
                eRenderer->renderQuad(eRect(10, 10, m_rtDiffuse->width, m_rtDiffuse->height), eGfx->getWndSize(), m_rtDiffuse);


            }





            // end frame
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
    eGfx->removeGeometry(m_geoQuad);
    eGfx->removeTexture2d(tex);
    eGfx->removeTexture2d(m_rtDiffuse);


    return 0;
}
