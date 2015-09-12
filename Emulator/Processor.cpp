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
    m_pThread = new std::thread(std::bind(&Processor::run, this));
}

void Processor::powerOff()
{
    if (m_pThread)
    {
        m_bRunning = false;
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
}

void Processor::setClockSpeed(uint64_t clockSpeed)
{
    m_clockSpeed = clockSpeed;
}

void Processor::run()
{
    while (m_bRunning)
    {
        if (!tick())
        {
            m_bRunning = false;
            break;
        }
    }
}
