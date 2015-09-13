#include <chrono>
#include <thread>

class RAM;

class Processor
{
public:
    Processor(RAM *pRAM);
    virtual ~Processor();

    bool isRunning() const { return m_bRunning; }

    void powerOn();
    void powerOff();
    void setClockSpeed(uint64_t clockSpeed);
    virtual bool tick() = 0;
    virtual void IRQ(uint32_t addr) {}

    RAM *getRAM() const { return m_pRAM; }

protected:
    virtual void init() = 0;

private:
    bool m_bRunning = false;
    RAM *m_pRAM = nullptr;
    uint64_t m_clockSpeed = 1000000; // Default to 1mhz
};
