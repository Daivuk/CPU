#include <cinttypes>

class RAM
{
public:
    RAM(uint32_t size);
    virtual ~RAM();

    void setClockSpeed(uint64_t clockSpeed);

    uint8_t read8(uint32_t address) const;
    uint16_t read16(uint32_t address) const;
    uint32_t read32(uint32_t address) const;
    uint64_t read64(uint32_t address) const;

    void write(uint32_t address, uint32_t size, void *pData);
    void write8(uint32_t address, uint8_t val);
    void write16(uint32_t address, uint16_t val);
    void write32(uint32_t address, uint32_t val);
    void write64(uint32_t address, uint64_t val);

private:
    void *m_pData = nullptr;
    uint32_t m_size = 0;
    uint64_t m_clockSpeed = 1000000;
};
