#include "Processor.h"

class RAM;

class DV1509 : public Processor
{
public:
    DV1509(RAM *pRAM);
    virtual ~DV1509();

    void getRegisters(uint32_t *outRegisters) const;
    void IRQ(uint32_t addr);

protected:
    virtual void init();
    virtual bool tick();

private:
    static const auto REGISTER_COUNT = 16;
    static const auto SP = REGISTER_COUNT - 3;
    static const auto LR = REGISTER_COUNT - 2;
    static const auto PC = REGISTER_COUNT - 1;

    uint32_t m_registers[REGISTER_COUNT];
    uint32_t m_compare = 0;
    uint32_t m_carryBit = 0;
};
