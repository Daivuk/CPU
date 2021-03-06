#include <cassert>
#include <chrono>
#include <Windows.h>

#include "CLRBridge.h"
#include "DV1509.h"
#include "RAM.h"
#include "PPU.h"

RAM *pRAM = nullptr;
Processor *pProcessor = nullptr;
PPU *pPPU = nullptr;
void *pProgram = nullptr;
uint32_t programSize = 0;
volatile bool bRunning = true;
HWND target = 0;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    showFrmMain();

    //while (bRunning)
    //{
    //    MSG msg = {0};
    //    if (PeekMessage(&msg, target, 0, 0, PM_REMOVE))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);

    //        if (msg.message == WM_QUIT)
    //        {
    //            bRunning = false;
    //            break;
    //        }
    //    }

    //    auto before = std::chrono::high_resolution_clock::now();
    //    FrmMain_PPUTimer_Tick();
    //    auto after = std::chrono::high_resolution_clock::now();
    //    auto elapsed = after - before;
    //    auto totalExpected = std::chrono::milliseconds(17);
    //    if (elapsed < totalExpected)
    //    {
    //        auto remaining = totalExpected - elapsed;
    //        std::this_thread::sleep_for(remaining);
    //    }
    //}

    pProcessor->powerOff();

    delete pPPU;
    delete pProcessor;
    delete pRAM;
    delete[] pProgram;

    return 0;
}

void FrmMain_Closing()
{
    bRunning = false;
}

void FrmMain_load(void *pTarget)
{
    FILE *pFic = nullptr;
    fopen_s(&pFic, "../Samples/helloworld.elf", "rb");
    assert(pFic);
    fseek(pFic, 0, SEEK_END);
    programSize = static_cast<uint32_t>(ftell(pFic));
    assert(programSize);
    fseek(pFic, 0, SEEK_SET);
    pProgram = new uint8_t[programSize];
    fread(pProgram, 1, programSize, pFic);
    fclose(pFic);

    pRAM = new RAM(1 << 15);
    pProcessor = new DV1509(pRAM);
    target = (HWND)pTarget;
    pPPU = new PPU(pProcessor, target);
}

void loadProgramToRam()
{
    pRAM->write(0, programSize, pProgram);
}

void FrmMain_powerOn()
{
    loadProgramToRam();
    pProcessor->powerOn();
}

void FrmMain_powerOff()
{
    pProcessor->powerOff();
}

void FrmMain_reset()
{
    pProcessor->powerOff();
    loadProgramToRam();
    pProcessor->powerOn();
}

void getRegisters(uint32_t *outRegisters)
{
    ((DV1509 *)pProcessor)->getRegisters(outRegisters);
}

void FrmMain_PPUTimer_Tick()
{
    if (pProcessor->isRunning())
    {
        pPPU->draw();
    }
}
