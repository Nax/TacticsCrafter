#ifndef TC_CORE_ASSEMBLER_H
#define TC_CORE_ASSEMBLER_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <map>

struct State;
class Assembler
{
public:
    Assembler(State& state);
    void reset();
    bool run(std::uint32_t addr, const char* src);
    const auto& code() const { return _code; }

private:
    struct Label
    {
        const char* str;
        std::size_t len;

        bool operator==(const Label& other) const { return (len == other.len) && (std::strncmp(str, other.str, len) == 0); }
        bool operator<(const Label& other) const { return (len == other.len) ? (std::strncmp(str, other.str, len) < 0) : (len < other.len); }
    };

    struct LabelRef
    {
        std::size_t     index;
        std::uint32_t   addr;
        int             type;
        Label           label;
    };

    void skipWS();
    bool parseLabel();
    bool parseInstruction();
    bool parseIdentifier(char* dst, std::size_t len);
    bool parseImmediate(std::uint32_t* dst, int refType);
    bool parseImmediateNoSymbolic(std::uint32_t* dst);
    bool parseImmediateSymbolic(std::uint32_t* dst, int refType);
    bool parseRegister(std::uint8_t* dst);
    bool parseRegisterOffset(std::uint8_t* dstReg, std::uint32_t* dstOff);
    bool parseChar(char c);
    bool parseEOF();
    bool fixRefs();

    State& _state;

    int             _line;
    std::uint32_t   _addr;
    const char*     _src;
    std::size_t     _srcLen;
    std::size_t     _cursor;
    char*           _error;

    std::map<Label, std::uint32_t>  _labels;
    std::vector<LabelRef>           _refs;
    std::vector<std::uint32_t>      _code;
};

#endif /* TC_CORE_ASSEMBLER_H */
