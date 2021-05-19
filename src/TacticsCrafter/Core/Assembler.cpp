#include <cstring>
#include <cctype>
#include <array>
#include <TacticsCrafter/Core/Assembler.h>

#define EN_JUMP     0x01

#define AR_NONE     0x00
#define AR_IMM      0x01
#define AR_REG      0x02

namespace
{

struct OpData
{
    const char*     mnemo;
    std::uint8_t    op;
    std::uint8_t    encoding;
    std::uint8_t    args[3];
};

const OpData kOpData[] = {
    { "j",   002, EN_JUMP, { AR_IMM, AR_NONE, AR_NONE } },
    { "jal", 003, EN_JUMP, { AR_IMM, AR_NONE, AR_NONE } },
};

const OpData* lookupInstr(const char* mnemo)
{
    static const std::size_t opCount = sizeof(kOpData) / sizeof(*kOpData);

    for (size_t i = 0; i < opCount; ++i)
    {
        if (std::strcmp(kOpData[i].mnemo, mnemo) == 0)
            return kOpData + i;
    }
    return nullptr;
}

}

Assembler::Assembler(State& state)
: _state{state}
{

}

bool Assembler::run(std::uint32_t addr, const char* src)
{
    _line = 1;
    _addr = addr;
    _src = src;
    _srcLen = std::strlen(src);
    _cursor = 0;

    for (;;)
    {
        skipWS();
        if (parseInstruction())
            continue;

        return parseEOF();
    }
}

void Assembler::skipWS()
{
    for (;;)
    {
        char c = _src[_cursor];
        if (c != ' ' && c != '\t' && c != '\f' && c != '\r' && c != '\n')
            break;
        if (c == '\n')
            _line++;
        _cursor++;
    }
}

bool Assembler::parseInstruction()
{
    char mnemo[32];
    std::uint32_t imm{};
    std::uint32_t instr{};

    /* Do we have an instruction? */
    if (!parseIdentifier(mnemo, sizeof(mnemo)))
        return false;
    skipWS();

    /* We do! Look up which one */
    const OpData* op = lookupInstr(mnemo);
    if (!op)
        return false;

    for (int i = 0; i < 3; ++i)
    {
        switch (op->args[i])
        {
        case AR_NONE:
            break;
        case AR_IMM:
            if (!parseImmediate(&imm))
                return false;
            skipWS();
            break;
        }
    }

    /* Encode the instruction */
    switch (op->encoding)
    {
    case EN_JUMP:
        instr = ((op->op & 0x3f) << 26) | ((imm >> 2) & 0x3ffffff);
        break;
    }

    _code.push_back(instr);
    _addr += 4;
    return true;
}

bool Assembler::parseImmediate(std::uint32_t* dst)
{
    std::int64_t v{};
    std::uint8_t base{10};
    std::size_t i = _cursor;
    bool        neg{};

    /* Check for an actual number */
    if (!(_src[i] == '-' || std::isdigit(_src[i])))
        return false;

    /* Check for a negative number */
    if (_src[i] == '-')
    {
        i++;
        neg = true;
    }

    /* Check for a base prefix */
    if (_src[i] == '0')
    {
        if (_src[i + 1] == 'b')
        {
            i += 2;
            base = 2;
        }
        else if (_src[i + 1] == 'o')
        {
            i += 2;
            base = 8;
        }
        if (_src[i + 1] == 'd')
        {
            i += 2;
            base = 10;
        }
        if (_src[i + 1] == 'x')
        {
            i += 2;
            base = 16;
        }
    }

    /* Parse the number */
    for (;;)
    {
        char c = std::tolower(_src[i]);
        int n{-1};

        if (c >= '0' && c <= '9')
            n = c - '0';
        else if (c >= 'a' && c <= 'f')
            n = c - 'a' + 10;

        if (n < 0 || n >= base)
            break;

        i++;
        v = v * base + n;
    }

    if (neg)
        v *= -1;

    _cursor = i;
    *dst = v & 0xffffffff;
    return true;
}

bool Assembler::parseIdentifier(char* dst, std::size_t len)
{
    std::size_t size{};
    std::size_t i = _cursor;
    char c;

    c = _src[i++];
    if (!std::isalpha(c))
        return false;
    dst[size++] = c;

    while (size < len)
    {
        c = _src[i++];
        if (std::isalnum(c))
        {
            dst[size++] = c;
        }
        else
        {
            dst[size] = 0;
            _cursor += size;
            return true;
        }

    }
    return false;
}

bool Assembler::parseEOF()
{
    return _cursor == _srcLen;
}
