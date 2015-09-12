#include <cassert>

#include "DV1509.h"
#include "RAM.h"

DV1509::DV1509(RAM *pRAM)
    : Processor(pRAM)
{
}

DV1509::~DV1509()
{
}

void DV1509::init()
{
    // 1 Mhz
    setClockSpeed(1000000);

    // Initialize registers to 0
    for (auto i = 0; i < REGISTER_COUNT; ++i)
    {
        m_registers[i] = 0x00000000;
    }

    // Initialize flags
    m_compare = 0;
    m_carryBit = 0;
}

void DV1509::getRegisters(uint32_t *outRegisters) const
{
    memcpy(outRegisters, m_registers, sizeof(m_registers));
}

enum eInst : uint32_t
{
    ADD = 0x00,     // 00000
    ADC = 0x00,     // 00000 E
    SUB = 0x01,     // 00001
    SBC = 0x01,     // 00001 E
    MUL = 0x02,     // 00010
    DIV = 0x03,     // 00011
    AND = 0x04,     // 00100
    ORR = 0x05,     // 00101
    XOR = 0x06,     // 00110 
    LSL = 0x07,     // 00111
    RSL = 0x07,     // 00111
    LSR = 0x08,     // 01000
    RSR = 0x08,     // 01000 E
    MOV = 0x09,     // 01001
    NOT = 0x0A,     // 01010
    JMP = 0x0B,     // 01011
    LDR = 0x0C,     // 01100
    POP = 0x0C,     // 01100 E
    LDB = 0x0D,     // 01101
    LDH = 0x0E,     // 01110
    CMP = 0x0F,     // 01111
    ADDS = 0x10,    // 10000
    ADCS = 0x10,    // 10000 E
    SUBS = 0x11,    // 10001
    SBCS = 0x11,    // 10001 E
    MULS = 0x12,    // 10010
    DIVS = 0x13,    // 10011
    ANDS = 0x14,    // 10100
    ORRS = 0x15,    // 10101
    XORS = 0x16,    // 10110
    LSLS = 0x17,    // 10111
    RSLS = 0x17,    // 10111 E
    LSRS = 0x18,    // 11000
    RSRS = 0x18,    // 11000 E
    MOVS = 0x19,    // 11001
    NOTS = 0x1A,    // 11010
    FNC = 0x1B,     // 11011
    STR = 0x1C,     // 11100
    PSH = 0x1C,     // 11100 E
    STB = 0x1D,     // 11101
    STH = 0x1E,     // 11110
    BRK = 0x1F,     // 11111
};

enum eCond : uint32_t
{
    COND_ALWAYS = 0x0,
    COND_NEVER = 0x1,
    COND_EQ = 0x2,
    COND_NE = 0x3,
    COND_GR = 0x4,
    COND_GE = 0x5,
    COND_LO = 0x6,
    COND_LE = 0x7
};

bool DV1509::tick()
{
    // Fetch instruction
    auto instruction = getRAM()->read32(m_registers[PC]);

    // Get condition and test
    auto condition = instruction >> 29;
    auto bPass = true;
    switch (condition)
    {
        case COND_ALWAYS:
            break;
        case COND_NEVER:
            bPass = false;
            break;
        case COND_EQ:
            bPass = m_compare == COND_EQ;
            break;
        case COND_NE:
            bPass = m_compare == COND_LO || m_compare == COND_GR;
            break;
        case COND_GR:
            bPass = m_compare == COND_GR;
            break;
        case COND_GE:
            bPass = m_compare == COND_GR || m_compare == COND_EQ;
            break;
        case COND_LO:
            bPass = m_compare == COND_LO;
            break;
        case COND_LE:
            bPass = m_compare == COND_LO || m_compare == COND_EQ;
            break;
    }
    if (bPass)
    {
        // Get instruction code
        auto instCode = (instruction >> 24) & 0x1F;
        if ((instCode & 0xF) <= NOT ||
            instCode == CMP) // Ops
        {
            bool bCompareWithZeroAfter = (instruction & 0x10) ? true : false;
            bool bExtended = (instruction & 0x2) ? true : false;
            bool bImmediate = (instruction & 0x1) ? true : false;
            uint32_t Rd = (instruction >> 20) & 0xF;
            uint32_t Rl = (instruction >> 16) & 0xF;
            uint32_t rVal;
            if (bImmediate)
            {
                auto shifter = (instruction >> 12) & 0xF;
                auto bNeg = (shifter & 0x8) ? true : false;
                auto shift = shifter & 0x7;
                auto immediate = (instruction >> 4) & 0xFF;
                rVal = immediate << (shift * 4);
                if (bNeg) rVal = ~rVal;
            }
            else
            {
                rVal = m_registers[(instruction >> 12) & 0xF];
            }
            auto valueBefore = m_registers[Rd];
            if (!bExtended)
            {
                switch (instCode & 0xF)
                {
                    case ADD:
                        m_registers[Rd] = m_registers[Rl] + rVal;
                        m_carryBit = (m_registers[Rd] < valueBefore) ? 1 : 0;
                        break;
                    case SUB:
                        m_registers[Rd] = m_registers[Rl] - rVal;
                        m_carryBit = (m_registers[Rd] > valueBefore) ? 1 : 0;
                        break;
                    case MUL:
                        m_registers[Rd] = m_registers[Rl] * rVal;
                        m_carryBit = (m_registers[Rd] < valueBefore) ? 1 : 0;
                        break;
                    case DIV:
                        m_registers[Rd] = m_registers[Rl] / rVal;
                        m_carryBit = 0;
                        break;
                    case AND:
                        m_registers[Rd] = m_registers[Rl] & rVal;
                        m_carryBit = 0;
                        break;
                    case ORR:
                        m_registers[Rd] = m_registers[Rl] | rVal;
                        m_carryBit = 0;
                        break;
                    case XOR:
                        m_registers[Rd] = m_registers[Rl] ^ rVal;
                        m_carryBit = 0;
                        break;
                    case LSL:
                        m_carryBit = m_registers[Rl] >> 31;
                        m_registers[Rd] = m_registers[Rl] << rVal;
                        break;
                    case LSR:
                        m_carryBit = m_registers[Rl] & 0x1;
                        m_registers[Rd] = m_registers[Rl] >> rVal;
                        break;
                    case MOV:
                        m_registers[Rd] = rVal;
                        m_carryBit = 0;
                        break;
                    case NOT:
                        m_registers[Rd] = ~rVal;
                        m_carryBit = 0;
                        break;
                    case CMP:
                        if (m_registers[Rd] == rVal) m_compare = COND_EQ;
                        else if (m_registers[Rd] < rVal) m_compare = COND_LO;
                        else m_compare = COND_GR;
                        break;
                    default:
                        assert(false);
                        return false;
                }
            }
            else
            {
                switch (instCode & 0xF)
                {
                    case ADC:
                        m_registers[Rd] = m_registers[Rl] + rVal + m_carryBit;
                        m_carryBit = (m_registers[Rd] < valueBefore) ? 1 : 0;
                        break;
                    case SBC:
                        m_registers[Rd] = m_registers[Rl] - rVal - m_carryBit;
                        m_carryBit = (m_registers[Rd] > valueBefore) ? 1 : 0;
                        break;
                    case RSL:
                        m_carryBit = m_registers[Rl] & 0x1;
                        m_registers[Rd] = m_registers[Rl] >> rVal;
                        m_registers[Rd] ^= m_carryBit << 31;
                        break;
                    case RSR:
                        m_carryBit = m_registers[Rl] >> 31;
                        m_registers[Rd] = m_registers[Rl] << rVal;
                        m_registers[Rd] ^= m_carryBit;
                        break;
                    default:
                        assert(false);
                        return false;
                }
            }
            if (bCompareWithZeroAfter)
            {
                if (m_registers[Rd] == 0) m_compare = COND_EQ;
                else m_compare = COND_GR;
            }
        }
        else if (instCode == JMP || instCode == FNC)
        {
            bool bSavePC = (instCode & 0x10) ? true : false;
            auto bImmediate = instruction & 0x1;
            uint32_t dstAddr = 0;
            if (bImmediate)
            {
                dstAddr = (instruction >> 0x1) & 0x3FFFFF;
                dstAddr <<= 2;
                auto bNeg = ((instruction >> 0x1) & 0x700000) ? true : false;
                if (bNeg)
                {
                    dstAddr = m_registers[PC] - dstAddr;
                }
                else
                {
                    dstAddr = m_registers[PC] + dstAddr;
                }
            }
            else
            {
                auto Rs = (instruction >> 20) & 0xF;
                dstAddr = m_registers[Rs];
            }
            if (bSavePC)
            {
                m_registers[LR] = m_registers[PC];
            }
            m_registers[PC] = dstAddr;
            return true;
        }
        else if (instCode == LDR || instCode == LDB || instCode == LDH ||
                 instCode == STR || instCode == STB || instCode == STH)
        {
            bool bStore = (instruction & 0x10000000) ? true : false;
            auto count = instCode & 0x3;
            auto Rd = (instruction >> 20) & 0xF;
            bool bMultiple = (instruction & 0xF) == 0x8;
            if (bMultiple)
            {
                auto bits = instruction >> 4;
                if (bStore)
                {
                    for (uint32_t i = 0; i < 16; ++i)
                    {
                        if (bits & 0x1)
                        {
                            m_registers[SP] -= 4;
                            getRAM()->write32(m_registers[SP], m_registers[i]);
                        }
                        bits >>= 1;
                    }
                }
                else
                {
                    for (uint32_t i = 0; i < 16; ++i)
                    {
                        if (bits & 0x8000)
                        {
                            m_registers[15 - i] = getRAM()->read32(m_registers[SP]);
                            m_registers[SP] += 4;
                        }
                        bits <<= 1;
                    }
                }
            }
            else
            {
                bool bImmediate = (instruction & 0x1) ? true : false;
                if (bImmediate)
                {
                    auto dstAddr = (instruction >> 0x1) & 0x3FFFF;
                    dstAddr <<= 2;
                    auto bNeg = ((instruction >> 0x1) & 0x70000) ? true : false;
                    if (bNeg)
                    {
                        dstAddr = m_registers[PC] - dstAddr;
                    }
                    else
                    {
                        dstAddr = m_registers[PC] + dstAddr;
                    }
                    if (bStore)
                    {
                        assert(false);
                        return false;
                    }
                    else
                    {
                        switch (count)
                        {
                            case 0:
                                m_registers[Rd] = dstAddr;
                                break;
                            case 1:
                                m_registers[Rd] = static_cast<uint32_t>(dstAddr);
                                break;
                            case 2:
                                m_registers[Rd] = static_cast<uint32_t>(dstAddr);
                                break;
                        }
                    }
                }
                else
                {
                    uint32_t offset = 0;
                    uint32_t increment = 0;
                    auto bImmediateOffset = (instruction & 0x4) ? true : false;
                    auto bImmediateIncrement = (instruction & 0x2) ? true : false;
                    if (bImmediateOffset)
                    {
                        offset = (instruction >> 8) & 0xFF;
                    }
                    else
                    {
                        auto Ro = (instruction >> 12) & 0xF;
                        offset = m_registers[Ro];
                    }
                    if (bImmediateIncrement)
                    {
                        increment = (instruction >> 3) & 0x1F;
                    }
                    else
                    {
                        auto Ri = (instruction >> 4) & 0xF;
                        increment = m_registers[Ri];
                    }
                    auto Rm = (instruction >> 16) & 0xF;
                    auto dstAddr = m_registers[Rm] + offset;
                    if (bStore)
                    {
                        switch (count)
                        {
                            case 0:
                                getRAM()->write32(dstAddr, m_registers[Rd]);
                                break;
                            case 1:
                                getRAM()->write8(dstAddr, static_cast<uint8_t>(m_registers[Rd]));
                                break;
                            case 2:
                                getRAM()->write16(dstAddr, static_cast<uint16_t>(m_registers[Rd]));
                                break;
                        }
                    }
                    else
                    {
                        switch (count)
                        {
                            case 0:
                                m_registers[Rd] = getRAM()->read32(dstAddr);
                                break;
                            case 1:
                                m_registers[Rd] = static_cast<uint32_t>(getRAM()->read8(dstAddr));
                                break;
                            case 2:
                                m_registers[Rd] = static_cast<uint32_t>(getRAM()->read16(dstAddr));
                                break;
                        }
                    }
                    m_registers[Rm] += increment;
                }
            }
        }
        else if (instCode == BRK)
        {
            return false;
        }
        else
        {
            assert(false);
            return false;
        }
    }

    m_registers[PC] += 4; // Move PC to the next instruction

    return true;
}
