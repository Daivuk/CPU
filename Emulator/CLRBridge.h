#include <cinttypes>

void showFrmMain();
void FrmMain_load(void *pTarget);
void FrmMain_powerOn();
void FrmMain_powerOff();
void FrmMain_reset();
void getRegisters(uint32_t *outRegisters);
void FrmMain_PPUTimer_Tick();
void FrmMain_Closing();
