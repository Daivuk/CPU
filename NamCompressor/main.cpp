#include <algorithm>
#include <cinttypes>
#include <stdio.h>
#include <string>
#include <unordered_set>

using namespace std;

vector<uint8_t> out;
int bit = 0;

void write(int val, int bits)
{
    while (bits)
    {
        if (bit % 8 == 0)
        {
            out.push_back(0);
        }
        uint8_t *pByte = &out.back();

        *pByte <<= 1;
        *pByte |= val & 0x1;
        val <<= 1;
        bits--;
        ++bit;
    }
}

int main(int argc, char **args)
{
    if (argc < 2) return 1;
    string filename = args[1];

    FILE *pFic = nullptr;
    fopen_s(&pFic, filename.c_str(), "rb");
    if (!pFic) return 2;
    uint8_t data[960];
    fread(data, 1, 960, pFic);
    fclose(pFic);
    
    unordered_set<uint8_t> uniques;
    for (auto &b : data)
    {
        uniques.insert(b);
    }

    int bitCount = 0;
    auto uniqueSize = uniques.size();
    while (uniqueSize)
    {
        uniqueSize >>= 1;
        bitCount++;
    }

    int total = 0;
    write((int)uniqueSize, 8);
    int table[256] = {-1};
    int index = 0;
    for (auto &b : uniques)
    {
        write(b, bitCount);
        table[b] = index++;
    }
    total += (int)out.size();
    
    out.clear();
    bit = 0;
    for (int j = 4; j < 30; ++j)
    {
        int totalRLE = 0;
        {
            auto prev = data[j * 32];
            write(0, 1);
            write(table[prev], bitCount);
            for (int i = 1; i < 32; ++i)
            {
                auto &b = data[j * 32 + i];
                if (b == prev)
                {
                    write(1, 1);
                    continue;
                }
                write(0, 1);
                write(table[b], bitCount);
                prev = (int)b;
            }
            totalRLE = (int)out.size();
            printf("RLE(%i): %i", j, totalRLE);
            out.clear();
            bit = 0;
        }
        int totalINC = 0;
        {
            auto prev = data[j * 32];
            write(6, 3);
            write(table[prev], bitCount);
            for (int i = 1; i < 32; ++i)
            {
                auto &b = data[j * 32 + i];
                if (b == prev)
                {
                    write(2, 2);
                    continue;
                }
                if (b == prev + 1)
                {
                    write(3, 2);
                    continue;
                }
                write(0, 1);
                write(table[b], bitCount);
                prev = (int)b;
            }
            totalINC = (int)out.size();
            if (totalINC < totalRLE)
            {
                printf("\tINC: %i *", totalINC);
            }
            else
            {
                printf("\tINC: %i  ", totalINC);
            }
            out.clear();
            bit = 0;
        }
        int totalDEC = 0;
        {
            auto prev = data[j * 32];
            write(2, 2);
            write(table[prev], bitCount);
            for (int i = 1; i < 32; ++i)
            {
                auto &b = data[j * 32 + i];
                if (b == prev)
                {
                    write(0, 1);
                    continue;
                }
                if (b == 0)
                {
                    write(3, 2);
                    prev = 0;
                    continue;
                }
                write(2, 2);
                write(table[b], bitCount);
                prev = (int)b;
            }
            totalDEC = (int)out.size();
            if (totalDEC < totalRLE && totalDEC < totalINC)
            {
                printf("\tDEC: %i *", totalDEC);
            }
            else
            {
                printf("\tDEC: %i  ", totalDEC);
            }
            out.clear();
            bit = 0;
        }
        int totalZERO = 0;
        {
            auto prev = data[j * 32];
            write(7, 3);
            write(table[prev], bitCount);
            for (int i = 1; i < 32; ++i)
            {
                auto &b = data[j * 32 + i];
                if (b == 0)
                {
                    write(0, 1);
                    continue;
                }
                write(1, 1);
                write(table[b], bitCount);
                prev = (int)b;
            }
            totalZERO = (int)out.size();
            if (totalZERO < totalRLE && totalZERO < totalINC && totalZERO < totalDEC)
            {
                printf("\tZER: %i *\n", totalZERO);
            }
            else
            {
                printf("\tZER: %i\n", totalZERO);
            }
            out.clear();
            bit = 0;
        }
        total += min<>(totalRLE, min<>(totalINC, min<>(totalZERO, totalDEC)));
    }
    printf("total: %i\n", total);

    system("pause");
    return 0;
}
