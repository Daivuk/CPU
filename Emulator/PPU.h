#if WIN32
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif

#include <cinttypes>

class Processor;
class RAM;

class PPU
{
public:
    PPU(Processor *pProcessor, HWND target);
    virtual ~PPU();

    virtual void draw();

private:
    RAM *m_pRAM;
    Processor *m_pProcessor;
    int m_screenWidth;
    int m_screenHeight;
    GLuint glTexture_frameBuffer = 0;
    HGLRC hRC = nullptr;  // Permanent Rendering Context
    HDC hDC = nullptr;  // Private GDI Device Context
    uint32_t *m_pScreenData = nullptr;
};
