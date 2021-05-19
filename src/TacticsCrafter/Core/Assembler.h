#ifndef TC_CORE_ASSEMBLER_H
#define TC_CORE_ASSEMBLER_H

#include <vector>
#include <cstdint>
#include <cstddef>

struct State;
class Assembler
{
public:
    Assembler(State& state);
    bool run(std::uint32_t addr, const char* src);
    const auto& code() const { return _code; }

private:
    void skipWS();
    bool parseInstruction();
    bool parseIdentifier(char* dst, std::size_t len);
    bool parseImmediate(std::uint32_t* dst);
    bool parseRegister(std::uint8_t* dst);
    bool parseRegisterOffset(std::uint8_t* dstReg, std::uint32_t* dstOff);
    bool parseChar(char c);
    bool parseEOF();

    State& _state;

    int             _line;
    std::uint32_t   _addr;
    const char*     _src;
    std::size_t     _srcLen;
    std::size_t     _cursor;
    char*           _error;

    std::vector<std::uint32_t>  _code;
};

#endif /* TC_CORE_ASSEMBLER_H */
