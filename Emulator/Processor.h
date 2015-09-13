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

    RAM *getRAM() const { return m_pRAM; }

protected:
    virtual void init() = 0;
    virtual bool tick() = 0;

private:
    void run();

    volatile bool m_bRunning = false;
    RAM *m_pRAM = nullptr;
    std::thread *m_pThread = nullptr;
    uint64_t m_clockSpeed = 1000000; // Default to 1mhz
};
