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
        Blob,
    };

    struct Change
    {
        Change() : type{ChangeType::None} {}
        ~Change() { if (type == ChangeType::Blob) delete[] blob; }

        Change(const Change&) = delete;
        Change& operator=(const Change&) = delete;

        Change(Change&& c) { std::memcpy(this, &c, sizeof(Change)); c.type = ChangeType::None; }
        Change& operator=(Change&& c) { std::memcpy(this, &c, sizeof(Change)); c.type = ChangeType::None; return *this; }

        ChangeType type;
        std::uint32_t addr;
        union
        {
            std::uint8_t    u8;
            std::uint16_t   u16;
            std::uint32_t   u32;
            struct
            {
                char*       blob;
                std::size_t blobSize;
            };
        };
    };

    void clear() { _changes.clear(); }

    auto begin() const { return _changes.begin(); }
    auto end() const { return _changes.end(); }

    void write8(std::uint32_t addr, std::uint8_t value);
    void write16(std::uint32_t addr, std::uint16_t value);
    void write32(std::uint32_t addr, std::uint32_t value);
    void blob(std::uint32_t addr, const char* src, std::size_t len);

private:
    std::vector<Change> _changes;
};

#endif /* TC_CORE_CHANGESET_H */
