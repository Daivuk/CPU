#include <string>
#include <vector>
#include <iostream>
#include <cinttypes>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <algorithm>
using namespace std;

int compile(const string &filename);
int parseArguments(int argc, char **argv);
int createElf(const string &filename, const vector<uint8_t> &data);

struct sSection
{
    string name;
    uint32_t position = numeric_limits<uint32_t>::max();
    vector<uint8_t> code;
};
unordered_map<string, sSection *> sectionMap;
vector<sSection *> sections;
sSection *pCurrentSection = nullptr;
string fileToCompile;
string outputFile;
string outputPath;
vector<uint8_t> outputData;

int main(int argc, char **argv)
{
    auto ret = parseArguments(argc, argv);
    if (ret)
    {
        system("pause");
        return ret;
    }

    ret = compile(fileToCompile);
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
    ERROR_INVALID_SYNTAX
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

int readReg(const string &str)
{
}

uint32_t currentInstruction = 0;

uint32_t constructOpsCode(uint32_t code)
{
    return code << 24;
}

uint32_t constructSglCode(uint32_t code)
{
    return (code << 24) | 0x8000000;
}

unordered_map<string, function<int(const vector<string> &)>> instructions =
{
    {"BRK", [](const vector<string> &tokens) -> int
    {
        currentInstruction |= constructOpsCode(0x1F);
        return ERROR_NONE;
    }
    },
    {"MOV", [](const vector<string> &tokens) -> int
    {
        if (tokens.size() < 3)
        {
            return throwError(ERROR_INVALID_SYNTAX, "Invalid syntax");
        }
        auto Rd = readReg(tokens[1]);
        auto Rr = readReg(tokens[2]);
        currentInstruction |= constructSglCode(0x00);
        return ERROR_NONE;
    }
    },
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
        return throwError(ERROR_INVALID_INSTRUCTION, string("Invalid instruction ") + inst);
    }

    if (!pCurrentSection)
    {
        return throwError(ERROR_NOT_IN_SECTION, string("Not in section ") + inst);
    }
    
    return instructions[inst](tokens);
}

bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

int compile(const string &filename)
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
        else if (isSpace(c))
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            continue;
        }
        else if (c == '/' && !fic.eof())
        {
            if (!token.empty())
            {
                currentLine.push_back(token);
                token.clear();
            }
            bInCommentLine = true;
            continue;
        }
        token += c;
    }

    fic.close();
    return bError ? ERROR_COMPILE : ERROR_NONE;
}
