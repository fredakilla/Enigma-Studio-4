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
    {-1.0f,  1.0f,  1.0f,  0,      0 },
    { 1.0f,  1.0f,  1.0f,  1,      0 },
    {-1.0f, -1.0f,  1.0f,  0, 1.0f },
    { 1.0f, -1.0f,  1.0f,  1.0f, 1.0f },
    {-1.0f,  1.0f, -1.0f,  0,      0 },
    { 1.0f,  1.0f, -1.0f,  1.0f,      0 },
    {-1.0f, -1.0f, -1.0f,  0, 1.0f },
    { 1.0f, -1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f,  1.0f,  1.0f,       0,      0 },
    { 1.0f,  1.0f,  1.0f,  1.0f,      0 },
    {-1.0f,  1.0f, -1.0f,       0, 1.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f,       0,      0 },
    { 1.0f, -1.0f,  1.0f,  1.0f,      0 },
    {-1.0f, -1.0f, -1.0f,       0, 1.0f },
    { 1.0f, -1.0f, -1.0f,  1.0f, 1.0f },
    { 1.0f, -1.0f,  1.0f,       0,      0 },
    { 1.0f,  1.0f,  1.0f,  1.0f,      0 },
    { 1.0f, -1.0f, -1.0f,       0, 1.0f },
    { 1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f,       0,      0 },
    {-1.0f,  1.0f,  1.0f,  1.0f,      0 },
    {-1.0f, -1.0f, -1.0f,       0, 1.0f },
    {-1.0f,  1.0f, -1.0f,  1.0f, 1.0f },
};

static const uint16_t s_cubeIndices[36] =
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












void _fillGeoBuffers(eGeometry *geo, ePtr param)
{
    eTimer* timer = (eTimer*)param;
    const eF32 time = (eF32)timer->getElapsedMs()*0.001f;

    eSimpleVtx* vp = nullptr;
    eU16* ip = nullptr;

    eGfx->beginLoadGeometry(geo, 24, (ePtr*)&vp, 36, (ePtr*)&ip);
    {
        //eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*8 );
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
            dest.pos.x = src.pos.x * (1.0f + 0.1f * eSin(phase));
            dest.pos.y = src.pos.y * (1.0f + 0.2f * eCos(phase + 60.0f));
            dest.pos.z = src.pos.z * (1.0f + 0.3f * eSin(phase + 120.0f));
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




    // Init

    eVertexShader* m_vsQuad = eGfx->loadVertexShader(eSHADER(vs_quad));
    ePixelShader* m_psQuad = eGfx->loadPixelShader(eSHADER(ps_quad));

    eGeometryDx11* m_geo = eGfx->addGeometry(eGEO_DYNAMIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST, _fillGeoBuffers, &timer);
    //eGeometryDx11* m_geo = eGfx->addGeometry(eGEO_DYNAMIC | eGEO_IB16, eVTX_SIMPLE, eGPT_TRILIST);




    /*eSimpleVtx* vp = nullptr;
    eU16* ip = nullptr;

    eGfx->beginLoadGeometry(m_geo, 8, (ePtr*)&vp, 14, (ePtr*)&ip);
    {
        eMemCopy(vp, &s_cubeVertices[0], sizeof(eSimpleVtx)*8 );
        eMemCopy(ip, &s_cubeTriStrip[0], sizeof(eU16)*14 );
    }
    eGfx->endLoadGeometry(m_geo);*/





    //eTexture2dDx11* tex = eGfx->createChessTexture(64, 64, 16, eCOL_CYAN, eCOL_ORANGE);

    const eColor colors[2] = {eCOL_CYAN, eCOL_ORANGE};
    eArray<eColor> data(64*64);

    for (eU32 y=0, index=0; y<64; y++)
        for (eU32 x=0; x<64; x++)
            data[index++] = colors[(x/16+y/16)%2];

    bgfx::TextureFormat::Enum TF[] = { bgfx::TextureFormat::RGBA8, bgfx::TextureFormat::RGBA16, bgfx::TextureFormat::RGBA16F,
                                 bgfx::TextureFormat::RGBA32F, bgfx::TextureFormat::R16F, bgfx::TextureFormat::RG16F,
                                    bgfx::TextureFormat::RG32F, bgfx::TextureFormat::R8
                               };

    enum eTextureFormat
    {
        eTFO_ARGB8,
        eTFO_ARGB16,
        eTFO_ARGB16F,
        eTFO_DEPTH32F,
        eTFO_R16F,
        eTFO_R32F,
        eTFO_GR16F,
        eTFO_GR32F,
        eTFO_R8,
    };


    const bgfx::Memory* mem = bgfx::makeRef(&data[0], 64*64*sizeof(eU32));


    bgfx::TextureHandle m_textureColor = bgfx::createTexture2D((eU16)64, (eU16)64, (bool)false, 0, TF[eTFO_ARGB8], 0, mem );
    bgfx::UniformHandle s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Int1);




    // Update

    eMessage msg = eMSG_IDLE;
    while (msg != eMSG_QUIT)
    {
        eGfx->handleMessages(msg);

        if (msg == eMSG_IDLE)
        {

            const eF32 time = (eF32)timer.getElapsedMs()*0.001f;

            // Set view 0 default viewport.
            //bgfx::setViewRect(0, 0, 0, uint16_t(800), uint16_t(600) );


            eGfx->beginFrame();

            // clear
            eGfx->clear(eCM_ALL, eCOL_BLUE);

            // set render states
            eRenderState &rs = eGfx->freshRenderState();
            rs.targets[0] = eGraphics::TARGET_SCREEN;
            rs.cullMode = eCULL_NONE;
            rs.depthTest = eFALSE;
            rs.viewport.set(0, 0, 800, 600);
            rs.ps = m_psQuad;
            rs.vs = m_vsQuad;
            //rs.textures[0] = tex;
            rs.texFlags[0] = eTMF_CLAMP | eTMF_NEAREST;

            // set viewport
            eCamera cam(45.0f, (eF32)800/600, 0.1f, 1000.0f);
            eMatrix4x4 mtx;
            mtx.lookAt(eVector3(2,2,-5), eVector3(0,0,0), eVector3(0,1,0));
            cam.setViewMatrix(mtx);
            cam.activate();


            bgfx::setTexture(0, s_texColor, m_textureColor);

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
    //eGfx->removeTexture2d(tex);

    return 0;
}
