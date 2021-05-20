#include <cstring>
#include <cctype>
#include <array>
#include <TacticsCrafter/Core/Assembler.h>
#include <TacticsCrafter/Core/State.h>

#define EN_NORMAL   0x00
#define EN_JUMP     0x01
#define EN_SPECIAL  0x02
#define EN_REGIMM   0x03
#define EN_SPECIAL2 0x04
#define EN_SPECIAL3 0x05
#define EN_PSEUDO   0x06

#define AR_NONE             0x00
#define AR_IMM              0x01
#define AR_IMMJUMP          0x02
#define AR_IMMBRANCH        0x03
#define AR_RS               0x04
#define AR_RT               0x05
#define AR_RD               0x06
#define AR_SA               0x07
#define AR_RS_OFF           0x08
#define AR_RT_RAW           0x09

/* Only used internally for li/la refs */
#define AR_IMMHI            0x0f

#define AR_DEFAULT_RT       0x10
#define AR_DEFAULT_RD       0x20
#define AR_DEFAULT_SHIFT    0x30
#define AR_DEFAULT_ARITH    0x40
#define AR_DEFAULT_JALR     0x50

namespace
{

struct OpData
{
    const char*     mnemo;
    std::uint8_t    op;
    std::uint8_t    encoding;
    std::uint8_t    args[3];
};

const char* const kRegisters[32] = {
    "zero", "at", "v0", "v1",
    "a0",   "a1", "a2", "a3",
    "t0",   "t1", "t2", "t3",
    "t4",   "t5", "t6", "t7",
    "s0",   "s1", "s2", "s3",
    "s4",   "s5", "s6", "s7",
    "t8",   "t9", "k0", "k1",
    "gp",   "sp", "fp", "ra",
};

const OpData kOpData[] = {
    /* Regular instructions */
    { "j",     002, EN_JUMP,    { AR_IMMJUMP } },
    { "jal",   003, EN_JUMP,    { AR_IMMJUMP } },
    { "beq",   004, EN_NORMAL,  { AR_RS, AR_RT, AR_IMMBRANCH } },
    { "bne",   005, EN_NORMAL,  { AR_RS, AR_RT, AR_IMMBRANCH } },
    { "blez",  006, EN_NORMAL,  { AR_RS, AR_IMMBRANCH } },
    { "bgtz",  007, EN_NORMAL,  { AR_RS, AR_IMMBRANCH } },
    { "addi",  010, EN_NORMAL,  { AR_RT, AR_RS | AR_DEFAULT_RT, AR_IMM } },
    { "addiu", 011, EN_NORMAL,  { AR_RT, AR_RS | AR_DEFAULT_RT, AR_IMM } },
    { "slti",  012, EN_NORMAL,  { AR_RT, AR_RS, AR_IMM } },
    { "sltiu", 013, EN_NORMAL,  { AR_RT, AR_RS, AR_IMM } },
    { "andi",  014, EN_NORMAL,  { AR_RT, AR_RS | AR_DEFAULT_RT, AR_IMM } },
    { "ori",   015, EN_NORMAL,  { AR_RT, AR_RS | AR_DEFAULT_RT, AR_IMM } },
    { "xori",  016, EN_NORMAL,  { AR_RT, AR_RS | AR_DEFAULT_RT, AR_IMM } },
    { "lui",   017, EN_NORMAL,  { AR_RT, AR_IMM } },
    { "beql",  024, EN_NORMAL,  { AR_RS, AR_RT, AR_IMMBRANCH } },
    { "bnel",  025, EN_NORMAL,  { AR_RS, AR_RT, AR_IMMBRANCH } },
    { "blezl", 026, EN_NORMAL,  { AR_RS, AR_IMMBRANCH } },
    { "bgtzl", 027, EN_NORMAL,  { AR_RS, AR_IMMBRANCH } },
    { "lb",    040, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lh",    041, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lwl",   042, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lw",    043, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lbu",   044, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lhu",   045, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "lwr",   046, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "sb",    050, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "sh",    051, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "swl",   052, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "sw",    053, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "swr",   056, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "cache", 057, EN_NORMAL,  { AR_RT_RAW, AR_RS_OFF } },
    { "ll",    060, EN_NORMAL,  { AR_RT, AR_RS_OFF } },
    { "pref",  063, EN_NORMAL,  { AR_RT_RAW, AR_RS_OFF } },
    { "sc",    070, EN_NORMAL,  { AR_RT, AR_RS_OFF } },

    /* SPECIAL */
    { "sll",     000, EN_SPECIAL, { AR_RD, AR_RT | AR_DEFAULT_RD, AR_SA } },
    { "srl",     002, EN_SPECIAL, { AR_RD, AR_RT | AR_DEFAULT_RD, AR_SA } },
    { "sra",     003, EN_SPECIAL, { AR_RD, AR_RT | AR_DEFAULT_RD, AR_SA } },
    { "sllv",    004, EN_SPECIAL, { AR_RD, AR_RT, AR_RS | AR_DEFAULT_SHIFT } },
    { "srlv",    006, EN_SPECIAL, { AR_RD, AR_RT, AR_RS | AR_DEFAULT_SHIFT } },
    { "srav",    007, EN_SPECIAL, { AR_RD, AR_RT, AR_RS | AR_DEFAULT_SHIFT } },
    { "jr",      010, EN_SPECIAL, { AR_RS } },
    { "jalr",    011, EN_SPECIAL, { AR_RD, AR_RS | AR_DEFAULT_JALR } },
    { "movz",    012, EN_SPECIAL, { AR_RD, AR_RS, AR_RT } },
    { "movn",    013, EN_SPECIAL, { AR_RD, AR_RS, AR_RT } },
    { "syscall", 014, EN_SPECIAL, { AR_NONE } },
    { "break",   015, EN_SPECIAL, { AR_NONE } },
    { "sync",    017, EN_SPECIAL, { AR_NONE } },
    { "mfhi",    020, EN_SPECIAL, { AR_RD } },
    { "mthi",    021, EN_SPECIAL, { AR_RS } },
    { "mflo",    022, EN_SPECIAL, { AR_RD } },
    { "mtlo",    023, EN_SPECIAL, { AR_RS } },
    { "mult",    030, EN_SPECIAL, { AR_RS, AR_RT } },
    { "multu",   031, EN_SPECIAL, { AR_RS, AR_RT } },
    { "div",     032, EN_SPECIAL, { AR_RS, AR_RT } },
    { "divu",    033, EN_SPECIAL, { AR_RS, AR_RT } },
    { "add",     040, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "addu",    041, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "sub",     042, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "subu",    043, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "and",     044, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "or",      045, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "xor",     046, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "nor",     047, EN_SPECIAL, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "slt",     052, EN_SPECIAL, { AR_RD, AR_RS, AR_RT } },
    { "sltu",    053, EN_SPECIAL, { AR_RD, AR_RS, AR_RT } },
    { "tge",     060, EN_SPECIAL, { AR_RS, AR_RT } },
    { "tgeu",    061, EN_SPECIAL, { AR_RS, AR_RT } },
    { "tlt",     062, EN_SPECIAL, { AR_RS, AR_RT } },
    { "tltu",    063, EN_SPECIAL, { AR_RS, AR_RT } },
    { "teq",     064, EN_SPECIAL, { AR_RS, AR_RT } },
    { "tne",     066, EN_SPECIAL, { AR_RS, AR_RT } },

    /* REGIMM */
    { "bltz",    000, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bgez",    001, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bltzl",   002, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bgezl",   003, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "tgei",    010, EN_REGIMM, { AR_RS, AR_IMM } },
    { "tgeiu",   011, EN_REGIMM, { AR_RS, AR_IMM } },
    { "tlti",    012, EN_REGIMM, { AR_RS, AR_IMM } },
    { "tltiu",   013, EN_REGIMM, { AR_RS, AR_IMM } },
    { "teqi",    014, EN_REGIMM, { AR_RS, AR_IMM } },
    { "tnei",    016, EN_REGIMM, { AR_RS, AR_IMM } },
    { "bltzal",  020, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bgezal",  021, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bltzall", 022, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },
    { "bgezall", 023, EN_REGIMM, { AR_RS, AR_IMMBRANCH } },

    /* SPECIAL2 */
    { "madd",    000, EN_SPECIAL2, { AR_RS, AR_RT } },
    { "maddu",   001, EN_SPECIAL2, { AR_RS, AR_RT } },
    { "mul",     002, EN_SPECIAL2, { AR_RD, AR_RS, AR_RT | AR_DEFAULT_ARITH } },
    { "msub",    004, EN_SPECIAL2, { AR_RS, AR_RT } },
    { "msubu",   005, EN_SPECIAL2, { AR_RS, AR_RT } },
    { "clz",     040, EN_SPECIAL2, { AR_RD, AR_RS } },
    { "clo",     041, EN_SPECIAL2, { AR_RD, AR_RS } },

    /* SPECIAL3 */

    /* Pseudo-instructions */
    { "nop",      0, EN_PSEUDO, { AR_NONE } },
    { "li",       1, EN_PSEUDO, { AR_RT, AR_IMM } },
    { "la",       1, EN_PSEUDO, { AR_RT, AR_IMM } },
    { "move",     2, EN_PSEUDO, { AR_RD, AR_RT } },
    { "not",      3, EN_PSEUDO, { AR_RD, AR_RT | AR_DEFAULT_RD } },
    { "neg",      4, EN_PSEUDO, { AR_RD, AR_RT | AR_DEFAULT_RD } },
    { "negu",     5, EN_PSEUDO, { AR_RD, AR_RT | AR_DEFAULT_RD } },
    { "b",        6, EN_PSEUDO, { AR_IMMBRANCH } },
    { "bl",       7, EN_PSEUDO, { AR_IMMBRANCH } },
    { "bal",      8, EN_PSEUDO, { AR_IMMBRANCH } },
    { "ball",     9, EN_PSEUDO, { AR_IMMBRANCH } },
    { "beqz",    10, EN_PSEUDO, { AR_RS, AR_IMMBRANCH } },
    { "beqzl",   11, EN_PSEUDO, { AR_RS, AR_IMMBRANCH } },
    { "bnez",    12, EN_PSEUDO, { AR_RS, AR_IMMBRANCH } },
    { "bnezl",   13, EN_PSEUDO, { AR_RS, AR_IMMBRANCH } },
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

bool isIdentifierLeadingChar(char c)
{
    return std::isalpha(c) || c == '_';
}

bool isIdentifierChar(char c)
{
    return isIdentifierLeadingChar(c) || std::isdigit(c);
}

}

Assembler::Assembler(State& state)
: _state{state}
{

}

void Assembler::reset()
{
    _line = 1;
    _cursor = 0;
    _error = "";
    _labels.clear();
    _refs.clear();
    _code.clear();
}

bool Assembler::run(std::uint32_t addr, const char* src)
{
    reset();

    _addr = addr;
    _src = src;
    _srcLen = std::strlen(src);

    for (;;)
    {
        skipWS();

        if (parseLabel())
            continue;

        if (parseInstruction())
            continue;

        if (!_error.empty())
            return false;

        if (!parseEOF())
        {
            _error = "Expected end-of-input";
            return false;
        }
        return fixRefs();
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

bool Assembler::parseLabel()
{
    std::size_t cursor = _cursor;

    if (!isIdentifierLeadingChar(_src[cursor++]))
        return false;
    while (isIdentifierChar(_src[cursor]))
        cursor++;
    if (_src[cursor] != ':')
        return false;

    Label l;
    l.str = _src + _cursor;
    l.len = cursor - _cursor;
    _labels[l] = _addr;
    _cursor = cursor + 1;
    return true;
}

bool Assembler::parseInstruction()
{
    char mnemo[32];
    std::uint32_t   imm{};
    std::uint32_t   tmp32{};
    std::uint32_t   instr{};
    std::uint8_t    rs{};
    std::uint8_t    rt{};
    std::uint8_t    rd{};
    std::uint8_t    sa{};
    std::uint8_t*   reg;

    /* Do we have an instruction? */
    if (!parseIdentifier(mnemo, sizeof(mnemo)))
        return false;
    skipWS();

    /* We do! Look up which one */
    const OpData* op = lookupInstr(mnemo);
    if (!op)
    {
        _error = std::string("Unknown instruction: ") + mnemo;
        return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        switch (op->args[i] & 0xf)
        {
        case AR_NONE:
            break;
        case AR_IMM:
        case AR_IMMJUMP:
            if (!parseImmediate(&imm, op->args[i] & 0xf))
            {
                _error = "Expected immediate";
                return false;
            }
            skipWS();
            break;
        case AR_IMMBRANCH:
            if (!parseImmediate(&imm, op->args[i] & 0xf))
            {
                _error = "Expected immediate branch target";
                return false;
            }
            skipWS();
            imm = ((imm - (_addr + 4)) >> 2) & 0xffff;
            break;
        case AR_RS:
        case AR_RT:
        case AR_RD:
            switch (op->args[i] & 0xf)
            {
            case AR_RS:
                reg = &rs;
                break;
            case AR_RT:
                reg = &rt;
                break;
            case AR_RD:
                reg = &rd;
                break;
            }
            if (!parseRegister(reg))
            {
                switch (op->args[i] & 0xf0)
                {
                case AR_DEFAULT_RT:
                    *reg = rt;
                    continue;
                case AR_DEFAULT_RD:
                    *reg = rd;
                    continue;
                case AR_DEFAULT_SHIFT:
                    rs = rt;
                    rt = rd;
                    continue;
                case AR_DEFAULT_ARITH:
                    rt = rs;
                    rs = rd;
                    continue;
                case AR_DEFAULT_JALR:
                    rs = rd;
                    rd = 31;
                    continue;
                case 0:
                    _error = "Expected register";
                    return false;
                }
            }
            skipWS();
            break;
        case AR_RS_OFF:
            if (!parseRegisterOffset(&rs, &imm))
            {
                _error = "Expected register-offset";
                return false;
            }
            skipWS();
            break;
        case AR_RT_RAW:
            if (!parseImmediateNoSymbolic(&tmp32))
            {
                _error = "Expected numeric value";
                return false;
            }
            skipWS();
            rt = tmp32 & 0x3f;
            break;
        case AR_SA:
            if (!parseImmediateNoSymbolic(&tmp32))
            {
                _error = "Expected shift amount";
                return false;
            }
            skipWS();
            sa = tmp32 & 0x3f;
            break;
        }

        /* Consume commas */
        if (i < 2 && op->args[i + 1] != AR_NONE)
        {
            /* TODO: Handle this better (e.g. optional args) */
            parseChar(',');
            skipWS();
        }
    }

    /* Encode the instruction */
    switch (op->encoding)
    {
    case EN_NORMAL:
        instr = ((op->op & 0x3f) << 26) | (rs << 21) | (rt << 16) | (imm & 0xffff);
        break;
    case EN_JUMP:
        instr = ((op->op & 0x3f) << 26) | ((imm >> 2) & 0x3ffffff);
        break;
    case EN_SPECIAL:
        instr = (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | (op->op & 0x3f);
        break;
    case EN_REGIMM:
        instr = (001 << 26) | (rs << 21) | (op->op << 16) | (imm & 0xffff);
        break;
    case EN_SPECIAL2:
        instr = (034 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (op->op & 0x3f);
        break;
    case EN_PSEUDO:
        switch (op->op)
        {
        case 0: /* nop */
            instr = 0;
            break;
        case 1: /* li, la */
            instr = (004 << 26) | (imm & 0xffff);
            if (imm & 0xffff0000)
            {
                /* If a forward ref was generated, we need to split it */
                if (!_refs.empty() && _refs.back().addr == _addr)
                {
                    LabelRef refHi = _refs.back();
                    LabelRef refLo = refHi;
                    refHi.type = AR_IMMHI;
                    refLo.addr += 4;
                    refLo.index++;

                    _refs.back() = refHi;
                    _refs.push_back(refLo);
                }

                /* Split into lui + ori */
                _code.push_back((017 << 26) | (rt << 16) | (imm >> 16));
                _addr += 4;
                instr = (015 << 26) | (rt << 21) | (rt << 16) | (imm & 0xffff);
            }
            else
            {
                /* ori with r0 */
                instr = (015 << 26) | (rt << 16) | (imm & 0xffff);
            }
            break;
        case 2: /* move */
            /* Implemented as a zero-shift sll */
            instr = (rt << 16) | (rd << 11);
            break;
        case 3: /* not */
            /* Implemented as nor with r0 */
            instr = (rt << 16) | (rd << 11) | 047;
            break;
        case 4: /* neg */
            /* Implemented as sub with r0 */
            instr = (rt << 16) | (rd << 11) | 042;
            break;
        case 5: /* negu */
            /* Implemented as subu with r0 */
            instr = (rt << 16) | (rd << 11) | 043;
            break;
        case 6: /* b */
            /* Implemented as bgezal r0 */
            instr = (004 << 26) | (imm & 0xffff);
            break;
        case 7: /* bl */
            /* Implemented as beql r0, r0 */
            instr = (024 << 26) | (imm & 0xffff);
            break;
        case 8: /* bal */
            /* Implemented as bgezal r0 */
            instr = (001 << 26) | (021 << 16) | (imm & 0xffff);
            break;
        case 9: /* ball */
            /* Implemented as bgezall r0 */
            instr = (001 << 26) | (023 << 16) | (imm & 0xffff);
            break;
        case 10: /* beqz */
            /* Implemented as beq r0 */
            instr = (004 << 26) | (rs << 21) | (imm & 0xffff);
            break;
        case 11: /* beqzl */
            /* Implemented as beql r0 */
            instr = (024 << 26) | (rs << 21) | (imm & 0xffff);
            break;
        case 12: /* bnez */
            /* Implemented as bne r0 */
            instr = (005 << 26) | (rs << 21) | (imm & 0xffff);
            break;
        case 13: /* bnezl */
            /* Implemented as bnel r0 */
            instr = (025 << 26) | (rs << 21) | (imm & 0xffff);
            break;
        }
        break;
    }

    _code.push_back(instr);
    _addr += 4;
    return true;
}

bool Assembler::parseImmediate(std::uint32_t* dst, int refType)
{
    return parseImmediateNoSymbolic(dst) || parseImmediateSymbolic(dst, refType);
}

bool Assembler::parseImmediateNoSymbolic(std::uint32_t* dst)
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

bool Assembler::parseImmediateSymbolic(std::uint32_t* dst, int refType)
{
    std::size_t cursor = _cursor;
    std::uint8_t dummy;
    char buffer[256];

    /* Register names are not valid labels */
    if (parseRegister(&dummy))
    {
        _cursor = cursor;
        return false;
    }

    if (!parseIdentifier(buffer, sizeof(buffer)))
        return false;

    Label l;
    l.str = _src + cursor;
    l.len = std::strlen(buffer);
    auto it = _labels.find(l);
    if (it == _labels.end())
    {
        /* Maybe a symbol from lua? */
        auto it2 = _state.symbols.find(std::string(l.str, l.len));
        if (it2 != _state.symbols.end())
        {
            *dst = it2->second;
            return true;
        }

        /* Forward reference - store it for later */
        LabelRef ref;
        ref.type = refType;
        ref.index = _code.size();
        ref.addr = _addr;
        ref.label = l;
        _refs.push_back(ref);

        /* Large positive value for worst case li/la expansion */
        *dst = 0x7fffffff;
        return true;
    }
    else
    {
        *dst = it->second;
    }
    return true;
}

bool Assembler::parseIdentifier(char* dst, std::size_t len)
{
    std::size_t size{};
    std::size_t i = _cursor;
    char c;

    c = _src[i++];
    if (!isIdentifierLeadingChar(c))
        return false;
    dst[size++] = c;

    while (size < len)
    {
        c = _src[i++];
        if (isIdentifierChar(c))
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

bool Assembler::parseRegister(std::uint8_t* dst)
{
    for (int i = 0; i < 32; ++i)
    {
        const char* r = kRegisters[i];
        std::size_t rLen = std::strlen(r);

        if (std::strncmp(_src + _cursor, r, rLen) == 0)
        {
            _cursor += rLen;
            *dst = i;
            return true;
        }
    }

    return false;
}

bool Assembler::parseRegisterOffset(std::uint8_t* dstReg, std::uint32_t* dstOff)
{
    std::size_t cursor = _cursor;
    std::uint8_t r;
    std::uint32_t off;

    if (parseImmediateNoSymbolic(&off))
    {
        if (!parseChar('(')) goto reject;
        if (!parseRegister(&r)) goto reject;
        if (!parseChar(')')) goto reject;
    }
    else
    {
        off = 0;
        if (!parseRegister(&r)) goto reject;
    }

    *dstReg = r;
    *dstOff = off;
    return true;

reject:
    _cursor = cursor;
    return false;
}

bool Assembler::parseChar(char c)
{
    if (_src[_cursor] == c)
    {
        _cursor++;
        return true;
    }
    return false;
}

bool Assembler::parseEOF()
{
    return _cursor == _srcLen;
}

bool Assembler::fixRefs()
{
    std::uint32_t mask;
    std::uint32_t value;

    for (const auto& r : _refs)
    {
        auto it = _labels.find(r.label);
        if (it == _labels.end())
        {
            _error = std::string("Unknown reference: ") + r.label.str;
            return false;
        }


        value = it->second;
        switch (r.type)
        {
        case AR_IMM:
            mask = 0xffff;
            break;
        case AR_IMMBRANCH:
            mask = 0xffff;
            value = ((value - (r.addr + 4)) >> 2);
            break;
        case AR_IMMJUMP:
            mask = 0x3ffffff;
            value >>= 2;
            break;
        case AR_IMMHI:
            mask = 0xffff;
            value >>= 16;
            break;
        }
        _code[r.index] = (value & mask) | (_code[r.index] & ~mask);
    }

    return true;
}
