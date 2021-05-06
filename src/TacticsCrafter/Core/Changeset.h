#ifndef TC_CORE_CHANGESET_H
#define TC_CORE_CHANGESET_H

#include <vector>
#include <cstdint>

class Changeset
{
public:
    enum class ChangeType
    {
        None = 0,
        Write8,
        Write16,
        Write32,
    };

    struct Change
    {
        ChangeType type;
        std::uint32_t addr;
        union
        {
            std::uint8_t    u8;
            std::uint16_t   u16;
            std::uint32_t   u32;
        };
    };

    auto begin() const { return _changes.begin(); }
    auto end() const { return _changes.end(); }

    void write8(std::uint32_t addr, std::uint8_t value);
    void write16(std::uint32_t addr, std::uint16_t value);
    void write32(std::uint32_t addr, std::uint32_t value);

private:
    std::vector<Change> _changes;
};

#endif /* TC_CORE_CHANGESET_H */
