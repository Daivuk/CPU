#include <cassert>
#include <memory>

#include "RAM.h"

RAM::RAM(uint32_t size)
    : m_size(size)
{
    m_pData = new uint8_t[m_size];
}

RAM::~RAM()
{
    if (m_pData)
    {
        delete[] m_pData;
    }
}

void RAM::setClockSpeed(uint64_t clockSpeed)
{
    m_clockSpeed = clockSpeed;
}

void *RAM::getPointer(uint32_t address) const
{
    assert(address >= 0 && address <= m_size - 1); // Out of range
    return (uint8_t *)m_pData + address;
}

uint8_t RAM::read8(uint32_t address) const
{
    assert(address >= 0 && address <= m_size - 1); // Out of range
    return *((uint8_t *)m_pData + address);
}

uint16_t RAM::read16(uint32_t address) const
{
    assert(address >= 0 && address <= m_size - 2); // Out of range
    return *(uint16_t *)((uint8_t *)m_pData + address);
}

uint32_t RAM::read32(uint32_t address) const
{
    assert(address >= 0 && address <= m_size - 4); // Out of range
    return *(uint32_t *)((uint8_t *)m_pData + address);
}

uint64_t RAM::read64(uint32_t address) const
{
    assert(address >= 0 && address <= m_size - 8); // Out of range
    return *(uint64_t *)((uint8_t *)m_pData + address);
}

void RAM::write(uint32_t address, uint32_t size, void *pData)
{
    assert(address >= 0 && address <= m_size - size); // Out of range
    memcpy((uint8_t *)m_pData + address, pData, size);
}

void RAM::write8(uint32_t address, uint8_t val)
{
    assert(address >= 0 && address <= m_size - 1); // Out of range
    *((uint8_t *)m_pData + address) = val;
}

void RAM::write16(uint32_t address, uint16_t val)
{
    assert(address >= 0 && address <= m_size - 2); // Out of range
    *(uint16_t *)((uint8_t *)m_pData + address) = val;
}

void RAM::write32(uint32_t address, uint32_t val)
{
    assert(address >= 0 && address <= m_size - 4); // Out of range
    *(uint32_t *)((uint8_t *)m_pData + address) = val;
}

void RAM::write64(uint32_t address, uint64_t val)
{
    assert(address >= 0 && address <= m_size - 8); // Out of range
    *(uint64_t *)((uint8_t *)m_pData + address) = val;
}
