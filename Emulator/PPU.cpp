#include "PPU.h"
#include "Processor.h"
#include "RAM.h"

#define SCREEN_W 256
#define SCREEN_H 240

#define VIDEO_ADDR 0x3FC0
#define ATR_W 16
#define ATR_H 16
#define NAM_W 32
#define NAM_H 30

#define TILE_W 8
#define TILE_H 8

uint16_t NESpalette8[] = {
    0x1D << 2, 0x1D << 2, 0x1D << 2, /* Value 0 */
    0x09 << 2, 0x06 << 2, 0x23 << 2, /* Value 1 */
    0x00 << 2, 0x00 << 2, 0x2A << 2, /* Value 2 */
    0x11 << 2, 0x00 << 2, 0x27 << 2, /* Value 3 */
    0x23 << 2, 0x00 << 2, 0x1D << 2, /* Value 4 */
    0x2A << 2, 0x00 << 2, 0x04 << 2, /* Value 5 */
    0x29 << 2, 0x00 << 2, 0x00 << 2, /* Value 6 */
    0x1F << 2, 0x02 << 2, 0x00 << 2, /* Value 7 */
    0x10 << 2, 0x0B << 2, 0x00 << 2, /* Value 8 */
    0x00 << 2, 0x11 << 2, 0x00 << 2, /* Value 9 */
    0x00 << 2, 0x14 << 2, 0x00 << 2, /* Value 10 */
    0x00 << 2, 0x0F << 2, 0x05 << 2, /* Value 11 */
    0x06 << 2, 0x0F << 2, 0x17 << 2, /* Value 12 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 13 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 14 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 15 */
    0x2F << 2, 0x2F << 2, 0x2F << 2, /* Value 16 */
    0x00 << 2, 0x1C << 2, 0x3B << 2, /* Value 17 */
    0x08 << 2, 0x0E << 2, 0x3B << 2, /* Value 18 */
    0x20 << 2, 0x00 << 2, 0x3C << 2, /* Value 19 */
    0x2F << 2, 0x00 << 2, 0x2F << 2, /* Value 20 */
    0x39 << 2, 0x00 << 2, 0x16 << 2, /* Value 21 */
    0x36 << 2, 0x0A << 2, 0x00 << 2, /* Value 22 */
    0x32 << 2, 0x13 << 2, 0x03 << 2, /* Value 23 */
    0x22 << 2, 0x1C << 2, 0x00 << 2, /* Value 24 */
    0x00 << 2, 0x25 << 2, 0x00 << 2, /* Value 25 */
    0x00 << 2, 0x2A << 2, 0x00 << 2, /* Value 26 */
    0x00 << 2, 0x24 << 2, 0x0E << 2, /* Value 27 */
    0x00 << 2, 0x20 << 2, 0x22 << 2, /* Value 28 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 29 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 30 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 31 */
    0x3F << 2, 0x3F << 2, 0x3F << 2, /* Value 32 */
    0x0F << 2, 0x2F << 2, 0x3F << 2, /* Value 33 */
    0x17 << 2, 0x25 << 2, 0x3F << 2, /* Value 34 */
    0x33 << 2, 0x22 << 2, 0x3F << 2, /* Value 35 */
    0x3D << 2, 0x1E << 2, 0x3F << 2, /* Value 36 */
    0x3F << 2, 0x1D << 2, 0x2D << 2, /* Value 37 */
    0x3F << 2, 0x1D << 2, 0x18 << 2, /* Value 38 */
    0x3F << 2, 0x26 << 2, 0x0E << 2, /* Value 39 */
    0x3C << 2, 0x2F << 2, 0x0F << 2, /* Value 40 */
    0x20 << 2, 0x34 << 2, 0x04 << 2, /* Value 41 */
    0x13 << 2, 0x37 << 2, 0x12 << 2, /* Value 42 */
    0x16 << 2, 0x3E << 2, 0x26 << 2, /* Value 43 */
    0x00 << 2, 0x3A << 2, 0x36 << 2, /* Value 44 */
    0x1E << 2, 0x1E << 2, 0x1E << 2, /* Value 45 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 46 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 47 */
    0x3F << 2, 0x3F << 2, 0x3F << 2, /* Value 48 */
    0x2A << 2, 0x39 << 2, 0x3F << 2, /* Value 49 */
    0x31 << 2, 0x35 << 2, 0x3F << 2, /* Value 50 */
    0x35 << 2, 0x32 << 2, 0x3F << 2, /* Value 51 */
    0x3F << 2, 0x31 << 2, 0x3F << 2, /* Value 52 */
    0x3F << 2, 0x31 << 2, 0x36 << 2, /* Value 53 */
    0x3F << 2, 0x2F << 2, 0x2C << 2, /* Value 54 */
    0x3F << 2, 0x36 << 2, 0x2A << 2, /* Value 55 */
    0x3F << 2, 0x39 << 2, 0x28 << 2, /* Value 56 */
    0x38 << 2, 0x3F << 2, 0x28 << 2, /* Value 57 */
    0x2A << 2, 0x3C << 2, 0x2F << 2, /* Value 58 */
    0x2C << 2, 0x3F << 2, 0x33 << 2, /* Value 59 */
    0x27 << 2, 0x3F << 2, 0x3C << 2, /* Value 60 */
    0x31 << 2, 0x31 << 2, 0x31 << 2, /* Value 61 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 62 */
    0x00 << 2, 0x00 << 2, 0x00 << 2, /* Value 63 */
};

uint32_t NESpalette[256];

PPU::PPU(Processor *pProcessor, HWND hWnd)
    : m_pProcessor(pProcessor)
    , m_hWnd(hWnd)
{
    m_pRAM = m_pProcessor->getRAM();
    m_pScreenData = new uint32_t[256 * 256];// SCREEN_W * SCREEN_H];
    
    // Build palette
    for (int i = 0; i < 64; ++i)
    {
        auto r = (uint32_t)NESpalette8[i * 3 + 0];
        auto g = (uint32_t)NESpalette8[i * 3 + 1];
        auto b = (uint32_t)NESpalette8[i * 3 + 2];
        uint32_t color = 0xFF000000 | (b << 16) | (g << 8) | r;
        NESpalette[i + 0 * 64] = color;
        NESpalette[i + 1 * 64] = color;
        NESpalette[i + 2 * 64] = color;
        NESpalette[i + 3 * 64] = color;
    }

    // Initialize OpenGL
    GLuint PixelFormat;
    static PIXELFORMATDESCRIPTOR pfd =  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW |            // Format Must Support Window
        PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,               // Must Support Double Buffering
        PFD_TYPE_RGBA,                  // Request An RGBA Format
        32,                             // Select Our Color Depth
        0, 0, 0, 0, 0, 0,               // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                     // Accumulation Bits Ignored
        16,                             // 16Bit Z-Buffer (Depth Buffer)
        0,                              // No Stencil Buffer
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                 // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                         // Layer Masks Ignored
    };
    hDC = GetDC(hWnd);
    PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    RECT rect;
    GetClientRect(hWnd, &rect);
    m_screenWidth = rect.right - rect.left;
    m_screenHeight = rect.bottom - rect.top;

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_screenWidth, m_screenHeight, 0, -999, 999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Create the frame buffer texture
    glGenTextures(1, &glTexture_frameBuffer);
    glBindTexture(GL_TEXTURE_2D, glTexture_frameBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

PPU::~PPU()
{
    delete[] m_pScreenData;
}

void PPU::draw()
{
    bool bPPUEnabled = m_pRAM->read32(VIDEO_ADDR + 0) ? true : false;

    RECT rect;
    GetClientRect(m_hWnd, &rect);
    m_screenWidth = rect.right - rect.left;
    m_screenHeight = rect.bottom - rect.top;

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_screenWidth, m_screenHeight, 0, -999, 999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTexture_frameBuffer);

    static uint32_t offsetX = 0;
    static uint32_t offsetY = 0;

    if (bPPUEnabled)
    {
        auto VIDEO_SCAN_LINE_IRQ = m_pRAM->read32(VIDEO_ADDR + 4);
        auto VIDEO_VSYNC_IRQ = m_pRAM->read32(VIDEO_ADDR + 8);
        auto pVIDEO_BG_PATTERN = (uint8_t *)m_pRAM->getPointer(m_pRAM->read32(VIDEO_ADDR + 12));
        auto pVIDEO_SP_PATTERN = (uint8_t *)m_pRAM->getPointer(m_pRAM->read32(VIDEO_ADDR + 16));
        auto pVIDEO_BG_NAM = (uint8_t *)m_pRAM->getPointer(m_pRAM->read32(VIDEO_ADDR + 20));
        auto pVIDEO_BG_ATR = pVIDEO_BG_NAM + 960;
        uint8_t* pVIDEO_BG_NAMS[4] = {
            pVIDEO_BG_NAM,
            pVIDEO_BG_NAM + 1024,
            pVIDEO_BG_NAM + 2048,
            pVIDEO_BG_NAM + 3072
        };
        auto VIDEO_BG_OFFSET_X = m_pRAM->read32(VIDEO_ADDR + 24);
        auto VIDEO_BG_OFFSET_Y = m_pRAM->read32(VIDEO_ADDR + 28);
        auto pVIDEO_BG_PAL = (uint8_t *)m_pRAM->getPointer(VIDEO_ADDR + 32);

        // Trigger VBlank IRQ
        m_pProcessor->IRQ(VIDEO_VSYNC_IRQ);
        for (uint32_t i = 0; i < 17000 - SCREEN_H * 32; ++i)
        {
            m_pProcessor->tick();
        }

        // Do scan lines!
        uint32_t *ptr = m_pScreenData;
        uint32_t tileId;
        for (uint32_t yCpt = 0; yCpt < SCREEN_H; ++yCpt)
        {
            // Trigger scan line IRQ
            m_pProcessor->IRQ(VIDEO_SCAN_LINE_IRQ);
            for (uint32_t i = 0; i < 32; ++i)
            {
                m_pProcessor->tick();
            }
            VIDEO_BG_OFFSET_X = m_pRAM->read32(VIDEO_ADDR + 24);
            VIDEO_BG_OFFSET_Y = m_pRAM->read32(VIDEO_ADDR + 28);
            auto y = yCpt + VIDEO_BG_OFFSET_Y;
            auto tileY = y / TILE_H;
            auto screenY = ((tileY / NAM_H) % 2) * 2;
            y %= TILE_H * NAM_H;
            tileY = y / TILE_H;
            auto inTileY = y - tileY * TILE_H;
            for (uint32_t x = VIDEO_BG_OFFSET_X; x < SCREEN_W + VIDEO_BG_OFFSET_X; ++x, ++ptr)
            {
                auto tileX = x / TILE_W;
                auto inTileX = x - tileX * TILE_W;

                pVIDEO_BG_NAM = pVIDEO_BG_NAMS[screenY + (tileX / NAM_W) % 2];
                pVIDEO_BG_ATR = pVIDEO_BG_NAM + 960;

                tileId = pVIDEO_BG_NAM[(tileY % NAM_H) * NAM_W + tileX % NAM_W];

                auto bits1 = pVIDEO_BG_PATTERN[tileId * 16 + inTileY];
                auto bits2 = pVIDEO_BG_PATTERN[tileId * 16 + inTileY + 8];

                auto color = 
                    (((bits1 >> (7 - inTileX)) & 0x1)) |
                    (((bits2 >> (7 - inTileX)) & 0x1) << 1);

                auto attribX = x / (TILE_W * 4);
                auto attribY = y / (TILE_H * 4);
                auto inAttribX = (x - attribX * (TILE_W * 4)) / (TILE_W * 2);
                auto inAttribY = (y - attribY * (TILE_H * 4)) / (TILE_H * 2);

                auto attribId = (attribY % 8) * 8 + attribX % 8;
                auto attribLine = pVIDEO_BG_ATR[attribId];
                auto attribBitShift = (inAttribY % 8) * 4 + (inAttribX % 8) * 2;
                auto attribPal = (attribLine >> attribBitShift) & 0x3;
                attribPal *= 4;

                *ptr = NESpalette[pVIDEO_BG_PAL[attribPal + color]];
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pScreenData);
    }
    else
    {
        for (uint32_t i = 0; i < 17000; ++i)
        {
            m_pProcessor->tick();
        }
    }

    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0);
    glTexCoord2f(0, (float)SCREEN_H / 256.f);
    glVertex2i(0, m_screenHeight);
    glTexCoord2f(1, (float)SCREEN_H / 256.f);
    glVertex2i(m_screenWidth, m_screenHeight);
    glTexCoord2f(1, 0);
    glVertex2i(m_screenWidth, 0);
    glEnd();
    
    SwapBuffers(hDC);
}
