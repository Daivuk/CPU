#include <string>
#include <vector>
#include <iostream>
#include <cinttypes>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <algorithm>
using namespace std;

struct sSection
{
    string name;
    uint32_t position = numeric_limits<uint32_t>::max();
    vector<vector<string>> tokenz;
};
unordered_map<string, sSection *> sectionMap;
vector<sSection *> sections;
sSection *pCurrentSection = nullptr;
string fileToCompile;
string outputFile;
string outputPath;
vector<uint8_t> outputData;
unordered_map<string, uint32_t> labels;
uint32_t pc = 0;

int preprocess(const string &filename);
int compile(const vector<sSection *> &sections);
int parseArguments(int argc, char **argv);
int createElf(const string &filename, const vector<uint8_t> &data);

int main(int argc, char **argv)
{
    auto ret = parseArguments(argc, argv);
    if (ret)
    {
        system("pause");
        return ret;
    }

    ret = preprocess(fileToCompile);
    if (ret)
    {
        system("pause");
        return ret;
    }

    ret = compile(sections);
    if (ret)
    {
        system("pause");
        return ret;
    }

    ret = createElf(outputFile, outputData);
    if (ret) return ret;

    system("pause");
    return 0;
}

int createElf(const string &filename, const vector<uint8_t> &data)
{
    ofstream ficOut(filename, ios::binary);
    if (ficOut.fail())
    {
        cout << "Error creating output file " << filename << endl;
        return 4;
    }
    ficOut.write(reinterpret_cast<const char *>(data.data()), data.size());
    ficOut.close();
    return 0;
}

int parseArguments(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Missing argument" << endl;
        return 1;
    }
    fileToCompile = argv[1];
    outputPath = fileToCompile;
    auto pos = outputPath.find_last_of("\\/");
    if (pos != string::npos)
    {
        outputPath = outputPath.substr(0, pos);
        outputPath += '/';
    }
    return 0;
}

int lineNum = 0;
int throwError(int code, const string &text)
{
    cout << "(" << lineNum << ") Error " << code << ": " << text << endl;
    return code;
}

enum ERROR_CODES : int
{
    ERROR_NONE = 0,
    ERROR_COMPILE,
    ERROR_INVALID_STRING,
    ERROR_INVALID_INPUT_FILE,
    ERROR_INVALID_COMPILER_INSTRUCTION,
    ERROR_INVALID_INSTRUCTION,
    ERROR_SECTION_REDEFINITION,
    ERROR_SECTION,
    ERROR_INVALID_NUMBER,
    ERROR_SECTION_NOT_DEFINED,
    ERROR_NOT_IN_SECTION,
    ERROR_INVALID_SYNTAX,
    ERROR_INVALID_CONSTANT,
    ERROR_DUPLICATE_LABEL,
    ERROR_LABEL_NOT_FOUND,
    ERROR_LABEL_OUT_OF_RANGE,
    ERROR_SPACE,
    ERROR_INCBIN,
    ERROR_OPENING_FILE,
    ERROR_EXPECTED_REGISTER
};

int extractString(string &str)
{
    if (str.size() < 2)
    {
        return ERROR_INVALID_STRING;
    }
    if (str.front() != '\"')
    {
        cout << "Expected \"" << endl;
        return ERROR_INVALID_STRING;
    }
    if (str.back() != '\"')
    {
        cout << "Expected \"" << endl;
        return ERROR_INVALID_STRING;
    }
    str = str.substr(1, str.size() - 2);
    return ERROR_NONE;
}

int extractNumber(uint32_t &number, const string &str)
{
    if (str.size() >= 3)
    {
        if (str[0] == '0' && str[1] == 'x')
        {
            try
            {
                number = stoul(str.substr(2), 0, 16);
                return ERROR_NONE;
            }
            catch (exception e)
            {
                return ERROR_INVALID_NUMBER;
            }
        }
        else if (str[0] == '0' && str[1] == 'b')
        {
            try
            {
                number = stoul(str.substr(2), 0, 2);
                return ERROR_NONE;
            }
            catch (exception e)
            {
                return ERROR_INVALID_NUMBER;
            }
        }
    }
    try
    {
        number = stoul(str);
        return ERROR_NONE;
    }
    catch (exception e)
    {
        return ERROR_INVALID_NUMBER;
    }

    return ERROR_NONE;
}

uint32_t readReg(const string &str)
{
    if (str == "r0") return 0x0;
    if (str == "r1") return 0x1;
    if (str == "r2") return 0x2;
    if (str == "r3") return 0x3;
    if (str == "r4") return 0x4;
    if (str == "r5") return 0x5;
    if (str == "r6") return 0x6;
    if (str == "r7") return 0x7;
    if (str == "r8") return 0x8;
    if (str == "r9") return 0x9;
    if (str == "r10") return 0xa;
    if (str == "r11") return 0xb;
    if (str == "r12") return 0xc;
    if (str == "sp") return 0xd;
    if (str == "lr") return 0xe;
    if (str == "pc") return 0xf;
    return 0x10;
}

uint32_t currentInstruction = 0;

enum eInst : uint32_t
{
    BRK = 0x1F,
    CMP = 0x03,

    ADD = 0x00, ADDS = 0x10,
    SUB = 0x01, SUBS = 0x11,
    MUL = 0x02, MULS = 0x12,
    DIV = 0x03, DIVS = 0x13,
    AND = 0x04, ANDS = 0x14,
    ORR = 0x05, ORRS = 0x15,
    XOR = 0x06, XORS = 0x16,
    LSL = 0x07, LSLS = 0x17,
    LSR = 0x08, LSRS = 0x18,

    MOV = 0x09, MOVS = 0x19,
    NOT = 0x0A, NOTS = 0x1A,

    LDR = 0x0C, LDB = 0x0D, LDH = 0x0E,
    STR = 0x1C, STB = 0x1D, STH = 0x1E,

    JMP = 0x0B, FNC = 0x1B,
};

int constructImmShifter(uint32_t &out, uint32_t number, bool bInv = false)
{
    if (!(number & 0xFFFFFF00))
    {
        out = number;
        return ERROR_NONE;
    }
    else if (!(number & 0xFFFFF00F))
    {
        out = 1 << 8;
        out |= number >> 4;
        return ERROR_NONE;
    }
    else if (!(number & 0xFFFF00FF))
    {
        out = 2 << 8;
        out |= number >> 8;
        return ERROR_NONE;
    }
    else if (!(number & 0xFFF00FFF))
    {
        out = 3 << 8;
        out |= number >> 12;
        return ERROR_NONE;
    }
    else if (!(number & 0xFF00FFFF))
    {
        out = 4 << 8;
        out |= number >> 16;
        return ERROR_NONE;
    }
    else if (!(number & 0xF00FFFFF))
    {
        out = 5 << 8;
        out |= number >> 20;
        return ERROR_NONE;
    }
    else if (!(number & 0x00FFFFFF))
    {
        out = 6 << 8;
        out |= number >> 24;
        return ERROR_NONE;
    }
    if (!bInv)
    {
        auto ret = constructImmShifter(out, ~number, true);
        if (!ret)
        {
            out |= 1 << 11;
        }
        return ret;
    }
    return ERROR_INVALID_CONSTANT;
}

uint32_t constructI(uint32_t number)
{
    return number;
}

uint32_t constructE(uint32_t number)
{
    return number << 1;
}

int constructOpsInst(uint32_t code, const vector<string> &tokens)
{
    if (tokens.size() < 3)
    {
        return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
    }
    if (tokens.size() == 3)
    {
        auto Rd = readReg(tokens[1]);
        if (Rd == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[1]);
        auto Rl = Rd;
        auto Rr = readReg(tokens[2]);
        currentInstruction |= Rd << 20;
        currentInstruction |= Rl << 16;
        if (Rr == 0x10)
        {
            // Immediate
            auto ret = extractNumber(Rr, tokens[2]);
            if (ret) return throwError(ret, string("Failed to parse number ") + tokens[2]);
            uint32_t imm_shifter = 0;
            ret = constructImmShifter(imm_shifter, Rr);
            if (ret) return throwError(ERROR_INVALID_CONSTANT, string("Invalid constant. Can't fix up ") + tokens[2]);
            currentInstruction |= imm_shifter << 4;
            currentInstruction |= constructI(1); // Immediate bit
        }
        else
        {
            // By register
            currentInstruction |= Rr << 12;
        }
    }
    else if (tokens.size() > 3)
    {
        auto Rd = readReg(tokens[1]);
        if (Rd == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[1]);
        auto Rl = readReg(tokens[2]);
        if (Rl == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[2]);
        auto Rr = readReg(tokens[3]);
        currentInstruction |= Rd << 20;
        currentInstruction |= Rl << 16;
        if (Rr == 0x10)
        {
            // Immediate
            auto ret = extractNumber(Rr, tokens[3]);
            if (ret) return throwError(ret, string("Failed to parse number ") + tokens[3]);
            uint32_t imm_shifter = 0;
            ret = constructImmShifter(imm_shifter, Rr);
            if (ret) return throwError(ERROR_INVALID_CONSTANT, string("Invalid constant. Can't fix up ") + tokens[3]);
            currentInstruction |= imm_shifter << 4;
            currentInstruction |= constructI(1); // Immediate bit
        }
        else
        {
            // By register
            currentInstruction |= Rr << 12;
        }
    }
    currentInstruction |= constructE(0);
    currentInstruction |= code << 24;
    return ERROR_NONE;
}

int constructSglInst(uint32_t code, const vector<string> &tokens)
{
    if (tokens.size() < 3)
    {
        return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
    }
    auto Rd = readReg(tokens[1]);
    if (Rd == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[1]);
    auto Rr = readReg(tokens[2]);
    currentInstruction |= Rd << 20;
    if (Rr == 0x10)
    {
        // Immediate
        auto ret = extractNumber(Rr, tokens[2]);
        if (ret) return throwError(ret, string("Failed to parse number ") + tokens[2]);
        uint32_t imm_shifter = 0;
        ret = constructImmShifter(imm_shifter, Rr);
        if (ret) return throwError(ERROR_INVALID_CONSTANT, string("Invalid constant. Can't fix up ") + tokens[2]);
        currentInstruction |= imm_shifter << 4;
        currentInstruction |= constructI(1); // Immediate bit
    }
    else
    {
        // By register
        currentInstruction |= Rr << 12;
    }
    currentInstruction |= constructE(0);
    currentInstruction |= code << 24;
    return ERROR_NONE;
}

int constructMemInst(uint32_t code, const vector<string> &tokens)
{
    if (tokens.size() < 3)
    {
        return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
    }

    auto Rd = readReg(tokens[1]);
    if (Rd == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[1]);
    currentInstruction |= Rd << 20;

    if (tokens[2] == "[")
    {
        // Register
        if (tokens.size() < 5)
        {
            return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
        }
        auto Rm = readReg(tokens[3]);
        if (Rm == 0x10) throwError(ERROR_EXPECTED_REGISTER, "Register expected " + tokens[3]);
        currentInstruction |= Rm << 16;

        if (tokens[4] == "]")
        {
            if (tokens.size() > 5)
            {
                auto Ri = readReg(tokens[5]);
                if (Ri == 0x10)
                {
                    // Immediate increment
                    uint32_t increment;
                    auto ret = extractNumber(increment, tokens[5]);
                    if (ret) throwError(ret, string("Failed to parse number ") + tokens[5]);
                    increment &= 0x1F;
                    currentInstruction |= increment << 3;
                    currentInstruction |= 1 << 1;
                }
                else
                {
                    // Register increment
                    currentInstruction |= Ri << 4;
                }
            }
        }
        else
        {
            if (tokens.size() < 6)
            {
                return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
            }

            auto Ro = readReg(tokens[4]);
            if (Ro == 0x10)
            {
                // Immediate offset
                uint32_t offset;
                auto ret = extractNumber(offset, tokens[4]);
                if (ret) throwError(ret, string("Failed to parse number ") + tokens[4]);
                offset &= 0xFF;
                currentInstruction |= offset << 8;
                currentInstruction |= 1 << 2;
            }
            else
            {
                // Register increment
                currentInstruction |= Ro << 12;
            }
            if (tokens[5] == "]")
            {
                if (tokens.size() > 6)
                {
                    auto Ri = readReg(tokens[6]);
                    if (Ri == 0x10)
                    {
                        // Immediate increment
                        uint32_t increment;
                        auto ret = extractNumber(increment, tokens[6]);
                        if (ret) throwError(ret, string("Failed to parse number ") + tokens[6]);
                        increment &= 0x1F;
                        currentInstruction |= increment << 3;
                        currentInstruction |= 1 << 1;
                    }
                    else
                    {
                        // Register increment
                        currentInstruction |= Ri << 4;
                    }
                }
            }
        }
    }
    else
    {
        // Immediate
        auto label = tokens[2] + ':';
        if (labels.find(label) == labels.end())
        {
            return throwError(ERROR_LABEL_NOT_FOUND, "Label not found " + label);
        }
        auto labelAddr = labels[label];
        auto offset = static_cast<int32_t>(labelAddr)-static_cast<int32_t>(pc);
        const int32_t limit = 0x3FFFF;
        if (offset < -limit || offset > limit)
        {
            return throwError(ERROR_LABEL_OUT_OF_RANGE, "Label out of range " + tokens[1]);
        }
        if (offset < 0)
        {
            offset = -offset;
            offset >>= 2;
            offset &= 0x3FFFF;
            offset |= 0x40000;
        }
        else
        {
            offset >>= 2;
            offset &= 0x3FFFF;
            currentInstruction |= offset << 1;
        }
        currentInstruction |= constructI(1); // Immediate bit
    }

    currentInstruction |= code << 24;

    return ERROR_NONE;
}

int constructJmpInst(uint32_t code, const vector<string> &tokens)
{
    if (tokens.size() < 2)
    {
        return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
    }
    auto Rs = readReg(tokens[1]);
    if (Rs == 0x10)
    {
        // Find the label
        auto label = tokens[1] + ':';
        if (labels.find(label) == labels.end())
        {
            return throwError(ERROR_LABEL_NOT_FOUND, "Label not found " + tokens[1]);
        }
        auto labelAddr = labels[label];
        auto offset = static_cast<int32_t>(labelAddr) - static_cast<int32_t>(pc);
        const int32_t limit = 0x3FFFFF;
        if (offset < -limit || offset > limit)
        {
            return throwError(ERROR_LABEL_OUT_OF_RANGE, "Label out of range " + tokens[1]);
        }
        if (offset < 0)
        {
            offset = -offset;
            offset >>= 2;
            offset &= 0x3FFFFF;
            offset |= 0x400000;
        }
        else
        {
            offset >>= 2;
            offset &= 0x3FFFFF;
            currentInstruction |= offset << 1;
        }
        currentInstruction |= constructI(1); // Immediate bit
    }
    else
    {
        // By register
        currentInstruction |= Rs << 20;
    }
    currentInstruction |= constructE(0);
    currentInstruction |= code << 24;
    return ERROR_NONE;
}

int constructBkrInst(uint32_t code, const vector<string> &tokens)
{
    currentInstruction |= code << 24;
    return ERROR_NONE;
}

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

#define CREATE_COND_INST_FULL(__inst__, __instEnum__, __fn__) \
    {__inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_ALWAYS << 4), tokens); }}, \
    {"!" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_NEVER << 4), tokens); }}, \
    {"==" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_EQ << 4), tokens); }}, \
    {"!=" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_NE << 4), tokens); }}, \
    {">" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_GR << 4), tokens); }}, \
    {">=" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_GE << 4), tokens); }}, \
    {"<" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_LO << 4), tokens); }}, \
    {"<=" __inst__, [](const vector<string> &tokens) -> int {return __fn__(__instEnum__ | (COND_LE << 4), tokens); }}

#define CREATE_COND_INST(__inst__, __fn__) \
    CREATE_COND_INST_FULL(#__inst__, eInst::__inst__, __fn__)

#define CREATE_COND_INST_S(__inst__, __fn__) \
    CREATE_COND_INST_FULL(#__inst__, eInst::__inst__, __fn__), \
    CREATE_COND_INST_FULL(#__inst__ "?", eInst::__inst__ ## S, __fn__)

unordered_map<string, function<int(const vector<string> &)>> instructions =
{
    // Break
    CREATE_COND_INST(BRK, constructBkrInst),

    // Ops
    CREATE_COND_INST_S(ADD, constructOpsInst),
    CREATE_COND_INST_S(SUB, constructOpsInst),
    CREATE_COND_INST_S(MUL, constructOpsInst),
    CREATE_COND_INST_S(DIV, constructOpsInst),
    CREATE_COND_INST_S(AND, constructOpsInst),
    CREATE_COND_INST_S(ORR, constructOpsInst),
    CREATE_COND_INST_S(XOR, constructOpsInst),
    CREATE_COND_INST_S(LSL, constructOpsInst),
    CREATE_COND_INST_S(LSR, constructOpsInst),

    // Singles
    CREATE_COND_INST_S(MOV, constructSglInst),
    CREATE_COND_INST_S(NOT, constructSglInst),
    CREATE_COND_INST(CMP, constructSglInst),

    // Memory (load/store)
    CREATE_COND_INST(LDR, constructMemInst),
    CREATE_COND_INST(LDB, constructMemInst),
    CREATE_COND_INST(LDH, constructMemInst),
    CREATE_COND_INST(STR, constructMemInst),
    CREATE_COND_INST(STB, constructMemInst),
    CREATE_COND_INST(STH, constructMemInst),

    // Jumps
    CREATE_COND_INST(JMP, constructJmpInst),
    CREATE_COND_INST(FNC, constructJmpInst),
};

int compileLine(const vector<string> &tokens)
{
    auto inst = tokens.front();
    transform(inst.begin(), inst.end(), inst.begin(), ::toupper);

    if (inst == ".COMPILER")
    {
        if (tokens.size() < 2)
        {
            return throwError(ERROR_INVALID_COMPILER_INSTRUCTION, "Expected compiler flag");
        }
        auto &flag = tokens[1];
        if (flag == "output")
        {
            if (tokens.size() < 3)
            {
                return throwError(ERROR_INVALID_COMPILER_INSTRUCTION, "Expected filename");
            }
            auto filename = tokens[2];
            auto ret = extractString(filename);
            if (ret) return throwError(ret, string("Failed to parse string ") + filename);
            outputFile = outputPath + filename;
        }
        else if (flag == "section")
        {
            if (tokens.size() < 3)
            {
                return throwError(ERROR_INVALID_COMPILER_INSTRUCTION, "Expected section name");
            }
            auto &sectionName = tokens[2];
            if (sectionMap.find(sectionName) != sectionMap.end())
            {
                return throwError(ERROR_SECTION_REDEFINITION, string("Section redefinition ") + sectionName);
            }
            sSection *pSection = new sSection;
            pSection->name = sectionName;
            if (tokens.size() >= 4)
            {
                // Read a section position
                auto ret = extractNumber(pSection->position, tokens[3]);
                if (ret) return throwError(ret, string("Failed to parse number ") + tokens[3]);
            }
            sections.push_back(pSection);
            sectionMap[pSection->name] = pSection;
        }
        return ERROR_NONE;
    }
    else if (inst == ".SECTION")
    {
        if (tokens.size() < 2)
        {
            return throwError(ERROR_SECTION, "Expected section name");
        }
        auto &sectionName = tokens[1];
        if (sectionMap.find(sectionName) == sectionMap.end())
        {
            return throwError(ERROR_SECTION_NOT_DEFINED, string("Section not defined ") + sectionName);
        }
        pCurrentSection = sectionMap[sectionName];
        return ERROR_NONE;
    }

    if (instructions.find(inst) == instructions.end())
    {
        if (inst.back() != ':') // Make sure it's not a label
        {
            return throwError(ERROR_INVALID_INSTRUCTION, string("Invalid instruction ") + inst);
        }
    }

    if (!pCurrentSection)
    {
        return throwError(ERROR_NOT_IN_SECTION, string("Not in section ") + inst);
    }
    
    pCurrentSection->tokenz.push_back(tokens);
    return ERROR_NONE;
}

bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == ',' || c == '#';
}

int preprocess(const string &filename)
{
    ifstream fic(filename);
    if (fic.fail())
    {
        cout << "Error opening input file " << filename << endl;
        return ERROR_INVALID_INPUT_FILE;
    }

    string token;
    char c;
    vector<string> currentLine;
    bool bInCommentLine = false;
    bool bInCommentBlock = false;
    bool bError = false;
    
    // Read character per character
    while (!fic.eof())
    {
        fic.read(&c, 1);
        if (bInCommentBlock)
        {
            if (bInCommentLine && c == '/')
            {
                bInCommentBlock = false;
            }
            bInCommentLine = false;
            if (c == '*')
            {
                bInCommentLine = true;
            }
            continue;
        }
        if (bInCommentLine)
        {
            bInCommentLine = false;
            if (c == '/')
            {
                fic.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            else if (c == '*')
            {
                bInCommentBlock = true;
                continue;
            }
        }
        if (c == '\n')
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            if (!currentLine.empty())
            {
                if (compileLine(currentLine)) bError = true;
                currentLine.clear();
            }
            ++lineNum;
            continue;
        }
        else if (c == '[' || c == ']')
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            currentLine.push_back(string() + c);
            continue;
        }
        else if (isSpace(c))
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            continue;
        }
        else if (c == '/')
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            bInCommentLine = true;
            continue;
        }
        else if (c == ';')
        {
            fic.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        token += c;
    }

    fic.close();
    return bError ? ERROR_COMPILE : ERROR_NONE;
}

void addInstruction(uint32_t instruction)
{
    outputData.push_back(instruction >> 24);
    outputData.push_back(instruction >> 16);
    outputData.push_back(instruction >> 8);
    outputData.push_back(instruction);
    pc += 4;
}

void addInt(uint32_t instruction)
{
    outputData.push_back(instruction >> 24);
    outputData.push_back(instruction >> 16);
    outputData.push_back(instruction >> 8);
    outputData.push_back(instruction);
    pc += 4;
}

void addByte(uint32_t instruction)
{
    outputData.push_back(instruction);
    pc += 1;
}

void addShort(uint32_t instruction)
{
    outputData.push_back(instruction >> 8);
    outputData.push_back(instruction);
    pc += 2;
}

int compile(const vector<sSection *> &sections)
{
    string inst;

    // Search for labels first, and create a map to address of them
    pc = 0;
    for (auto pSection : sections)
    {
        if (pSection->position != numeric_limits<uint32_t>::max())
        {
            pc = pSection->position;
        }
        else
        {
            pSection->position = pc;
        }
        for (auto it = pSection->tokenz.begin(); it != pSection->tokenz.end();)
        {
            auto &tokens = *it;
            auto &label = tokens.front();
            if (label.back() == ':')
            {
                // That's a label
                if (labels.find(label) != labels.end())
                {
                    return throwError(ERROR_DUPLICATE_LABEL, string("Label duplication ") + label);
                }
                labels[label] = pc;
                tokens.erase(tokens.begin());
                if (tokens.empty())
                {
                    it = pSection->tokenz.erase(it);
                    continue;
                }
            }

            inst = tokens.front();
            transform(inst.begin(), inst.end(), inst.begin(), ::toupper);
            if (inst == ".INT")
            {
                pc += (tokens.size() - 1) * 4;
            }
            else if (inst == ".SHORT")
            {
                pc += (tokens.size() - 1) * 2;
            }
            else if (inst == ".BYTE")
            {
                pc += (tokens.size() - 1) * 1;
            }
            else if (inst == ".SPACE")
            {
                if (tokens.size() < 2) return throwError(ERROR_SPACE, "Missing number after .SPACE");
                uint32_t number;
                auto ret = extractNumber(number, tokens[1]);
                if (ret) return throwError(ret, string("Failed to parse number ") + tokens[1]);
                pc += number;
            }
            else if (inst == ".INCBIN")
            {
                if (tokens.size() < 2) return throwError(ERROR_INCBIN, "Missing filename after .INCBIN");
                auto filename = tokens[1];
                auto ret = extractString(filename);
                if (ret) return throwError(ret, string("Failed to parse string ") + filename);
                filename = outputPath + filename;
                ifstream fic(filename, ifstream::ate | ifstream::binary);
                if (fic.fail())
                {
                    return throwError(ERROR_OPENING_FILE, "Can't open file " + filename);
                }
                pc += static_cast<uint32_t>(fic.tellg());
                fic.close();
            }
            else
            {
                pc += 4;
            }
            ++it;
        }
    }

    // Generate the code
    pc = 0;
    for (auto pSection : sections)
    {
        // Check if we have to add padding
        while (pSection->position > pc)
        {
            addByte(0);
        }

        for (auto &tokens : pSection->tokenz)
        {
            currentInstruction = 0;
            inst = tokens.front();
            transform(inst.begin(), inst.end(), inst.begin(), ::toupper);

            if (inst == ".INT")
            {
                for (size_t i = 1; i < tokens.size(); ++i)
                {
                    uint32_t number;
                    auto ret = extractNumber(number, tokens[i]);
                    if (ret) return throwError(ret, string("Failed to parse number ") + tokens[i]);
                    addInt(number);
                }
            }
            else if (inst == ".SHORT")
            {
                for (size_t i = 1; i < tokens.size(); ++i)
                {
                    uint32_t number;
                    auto ret = extractNumber(number, tokens[i]);
                    if (ret) return throwError(ret, string("Failed to parse number ") + tokens[i]);
                    addShort(number);
                }
            }
            else if (inst == ".BYTE")
            {
                for (size_t i = 1; i < tokens.size(); ++i)
                {
                    uint32_t number;
                    auto ret = extractNumber(number, tokens[i]);
                    if (ret) return throwError(ret, string("Failed to parse number ") + tokens[i]);
                    addByte(number);
                }
            }
            else if (inst == ".SPACE")
            {
                if (tokens.size() < 2) return throwError(ERROR_SPACE, "Missing number after .SPACE");
                uint32_t number;
                auto ret = extractNumber(number, tokens[1]);
                if (ret) return throwError(ret, string("Failed to parse number ") + tokens[1]);
                for (auto i = number; i; --i)
                {
                    addByte(0);
                }
            }
            else if (inst == ".INCBIN")
            {
                if (tokens.size() < 2) return throwError(ERROR_INCBIN, "Missing filename after .INCBIN");
                auto filename = tokens[1];
                auto ret = extractString(filename);
                if (ret) return throwError(ret, string("Failed to parse string ") + filename);
                filename = outputPath + filename;
                ifstream fic(filename, ifstream::ate | ifstream::binary);
                if (fic.fail())
                {
                    return throwError(ERROR_OPENING_FILE, "Can't open file " + filename);
                }
                auto size = fic.tellg();
                fic.seekg(0);
                if (size)
                {
                    char *pData = new char[static_cast<uint32_t>(size)];
                    fic.read(pData, size);
                    for (size_t i = 0; i < size; ++i)
                    {
                        addByte(pData[i]);
                    }
                }
                fic.close();
            }
            else
            {
                auto ret = instructions[inst](tokens);
                if (ret) return ret;
                addInstruction(currentInstruction);
            }
        }
    }

    return ERROR_NONE;
}
