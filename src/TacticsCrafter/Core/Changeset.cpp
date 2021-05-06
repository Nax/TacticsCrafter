#include <TacticsCrafter/Core/Changeset.h>

void Changeset::write8(std::uint32_t addr, std::uint8_t value)
{
    Change c;

    c.type = ChangeType::Write8;
    c.addr = addr;
    c.u8 = value;

    _changes.push_back(c);
}

void Changeset::write16(std::uint32_t addr, std::uint16_t value)
{
    Change c;

    c.type = ChangeType::Write16;
    c.addr = addr;
    c.u16 = value;

    _changes.push_back(c);
}

void Changeset::write32(std::uint32_t addr, std::uint32_t value)
{
    Change c;

    c.type = ChangeType::Write32;
    c.addr = addr;
    c.u32 = value;

    _changes.push_back(c);
}
