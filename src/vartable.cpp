#include "../include/vartable.h"
#include <cstring>

// Pasting the function here.
// Using Jenkins' one-at-a-time function.
static Hash hashBytes(const uint8_t* bytes, size_t size)
{
    Hash hash = 0;

    for (size_t i = 0; i < size; i++)
    {
        hash += bytes[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

VarEntry::VarEntry(std::string_view name, ui8 scope) :
    name(std::string(name)), scope(scope) {}

bool VarEntry::operator==(const VarEntry& other) const
{
    return ((this->scope == other.scope) // For short-circuit evaluation.
            && (this->name == other.name));
}

std::ostream& operator<<(std::ostream& os, const VarEntry& entry)
{
    os << "{" << entry.name << ", " << entry.scope << "}";
    return os;
}

Hash hashVarEntry(const VarEntry& entry)
{
    size_t nameSize = entry.name.size();
    ui8* bytes = new ui8[nameSize + sizeof(entry.scope)];
    std::memcpy(bytes, entry.name.data(), nameSize);
    std::memcpy(bytes + nameSize, &entry.scope, sizeof(entry.scope));
    Hash hash = hashBytes(bytes, nameSize + sizeof(entry.scope));
    delete[] bytes;
    return hash;
}