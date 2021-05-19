#include <TacticsCrafter/Core/Changeset.h>

void Changeset::write8(std::uint32_t addr, std::uint8_t value)
{
    Change c;

    c.type = ChangeType::Write8;
    c.addr = addr;
    c.u8 = value;

    _changes.push_back(std::move(c));
}

void Changeset::write16(std::uint32_t addr, std::uint16_t value)
{
    Change c;

    c.type = ChangeType::Write16;
    c.addr = addr;
    c.u16 = value;

    _changes.push_back(std::move(c));
}

void Changeset::write32(std::uint32_t addr, std::uint32_t value)
{
    Change c;

    c.type = ChangeType::Write32;
    c.addr = addr;
    c.u32 = value;

    _changes.push_back(std::move(c));
}

void Changeset::blob(std::uint32_t addr, const char* src, std::size_t len)
{
    Change c;

    c.type = ChangeType::Blob;
    c.addr = addr;
    c.blob = new char[len];
    std::memcpy(c.blob, src, len);
    c.blobSize = len;

    _changes.push_back(std::move(c));
}
