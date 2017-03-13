#include <Windows.h>

#include "../eshared/eshared.hpp"

/*

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

static const uint16_t s_cubeTriStrip[] =
{
    0, 1, 2,
    3,
    7,
    1,
    5,
    0,
    4,
    2,
    6,
    7,
    4,
    5,
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

*/


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
    const eTimer* timer = (eTimer*)param;
    const eF32 time = (eF32)timer->getElapsedMs()*0.001f;

    eSimpleVtx* vp = nullptr;
    eU16* ip = nullptr;

    eGfx->beginLoadGeometry(geo, 24, (ePtr*)&vp, 36, (ePtr*)&ip);
    {
        //eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*24 );
        //eMemCopy(ip, &s_cubeTriStrip[0], sizeof(eU16)*14 );
         eMemCopy(ip, &s_cubeIndices[0], sizeof(eU16)*36 );

        // or

        /*vp[0] = {-1.0f,  1.0f,  1.0f,  0.0f, 1.0f };
        vp[1] = { 1.0f,  1.0f,  1.0f,  0.0f, 1.0f };
        vp[2] = {-1.0f, -1.0f,  1.0f,  0.0f, 1.0f };
        vp[3] = { 1.0f, -1.0f,  1.0f,  0.0f, 1.0f };
        vp[4] = {-1.0f,  1.0f, -1.0f,  0.0f, 1.0f };
        vp[5] = { 1.0f,  1.0f, -1.0f,  0.0f, 1.0f };
        vp[6] = {-1.0f, -1.0f, -1.0f,  0.0f, 1.0f };
        vp[7] = { 1.0f, -1.0f, -1.0f,  0.0f, 1.0f };*/

        // or

        eF32 startPhase = time;
        eU32 numVertices = geo->maxVerts;
        for (eU32 j = 0; j < numVertices; ++j)
        {
            eF32 phase = startPhase * 10;
            eSimpleVtx& src = s_cubeVertices[j];
            eSimpleVtx& dest = *reinterpret_cast<eSimpleVtx*>(vp + j);

            // pos
            dest.pos.x = src.pos.x * (1.0f + 0.1f * eSin(phase));
            dest.pos.y = src.pos.y * (1.0f + 0.2f * eCos(phase + 60.0f));
            dest.pos.z = src.pos.z * (1.0f + 0.3f * eSin(phase + 120.0f));

            // uv
            dest.uv.u = s_cubeVertices[j].uv.u;
            dest.uv.v = s_cubeVertices[j].uv.v;
        }
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


    bool _fillWithCallBack = true;


    // Init --------------------------------------------------------------------------------------------------


    // create quad
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







    // create geometry

    eGeometryDx11* m_geo = nullptr;
    if(_fillWithCallBack)
    {
        // use callback function to fill geometry

        m_geo = eGfx->addGeometry(eGEO_DYNAMIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST, _fillGeoBuffers, &timer);
    }
    else
    {
        // fill geometry here

        m_geo = eGfx->addGeometry(eGEO_DYNAMIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST);

        eSimpleVtx* vp = nullptr;
        eU16* ip = nullptr;

        eGfx->beginLoadGeometry(m_geo, 24, (ePtr*)&vp, 36, (ePtr*)&ip);
        {
            eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*24 );
            eMemCopy(ip, &s_cubeIndices[0], sizeof(eU16)*36 );
        }
        eGfx->endLoadGeometry(m_geo);
    }


    // create shaders

    eVertexShader* m_vsQuad = eGfx->loadVertexShader(eSHADER(vs_quad));
    ePixelShader* m_psQuad = eGfx->loadPixelShader(eSHADER(ps_quad));



    // create texture

    eTexture2dDx11* tex = eGfx->createChessTexture(64, 64, 16, eCOL_CYAN, eCOL_ORANGE);
    eTexture2dDx11* tex2 = eGfx->createChessTexture(64, 64, 8, eCOL_DARKGRAY, eCOL_RED);

    ///const eColor colors[2] = {eCOL_CYAN, eCOL_ORANGE};
    ///eArray<eColor> data(64*64);
    ///
    ///for (eU32 y=0, index=0; y<64; y++)
    ///    for (eU32 x=0; x<64; x++)
    ///        data[index++] = colors[(x/16+y/16)%2];
    ///
    ///bgfx::TextureFormat::Enum TF[] = { bgfx::TextureFormat::RGBA8, bgfx::TextureFormat::RGBA16, bgfx::TextureFormat::RGBA16F,
    ///                             bgfx::TextureFormat::RGBA32F, bgfx::TextureFormat::R16F, bgfx::TextureFormat::RG16F,
    ///                                bgfx::TextureFormat::RG32F, bgfx::TextureFormat::R8
    ///                           };
    ///
    ///
    ///
    ///
    ///const bgfx::Memory* mem = bgfx::makeRef(&data[0], 64*64*sizeof(eU32));
    ///
    ///
    ///bgfx::TextureHandle m_textureColor = bgfx::createTexture2D((eU16)64, (eU16)64, (bool)false, 0, TF[eTFO_ARGB8], 0, mem );
    ///bgfx::UniformHandle s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Int1);




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
            {
                // set render states
                eRenderState &rs = eGfx->freshRenderState();
                rs.targets[0] = eGraphics::TARGET_SCREEN;
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
                mtx.lookAt(eVector3(2,2,-5), eVector3(0,0,0), eVector3(0,1,0));
                cam.setViewMatrix(mtx);
                eMatrix4x4 model;
                //model.identity();
                model.fromQuat(eQuat(eVector3(0,0,1), time));
                cam.activate(model);

                // render cube

                ///bgfx::setTexture(0, s_texColor, m_textureColor);
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
                rs.textures[0] = tex;
                rs.texFlags[0] = eTMF_CLAMP | eTMF_NEAREST;*/

                eRenderState &rs = eGfx->getRenderState();
                rs.textures[0] = tex2;

                // set viewport
                const eCamera cam(0.0f, (eF32)800, 0.0f, (eF32)600, -1.0f, 1.0f);
                cam.activate();

                eGfx->renderGeometry(m_geoQuad);
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
    eGfx->removeTexture2d(tex2);

    return 0;
}
