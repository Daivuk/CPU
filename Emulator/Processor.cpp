#include "Processor.h"
#include "RAM.h"

Processor::Processor(RAM *pRAM)
    : m_pRAM(pRAM)
{
}

Processor::~Processor()
{
    powerOff();
}

void Processor::powerOn()
{
    if (m_bRunning) return;
    init();
    m_bRunning = true;
}

void Processor::powerOff()
{
    m_bRunning = false;
}

void Processor::setClockSpeed(uint64_t clockSpeed)
{
    m_clockSpeed = clockSpeed;
}
