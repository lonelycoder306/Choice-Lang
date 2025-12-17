#pragma once
#include "common.h"
#include <iostream>
#include <string>

struct VarEntry
{
    std::string name;
    ui8 scope;

    VarEntry() = default;
    VarEntry(std::string_view name, ui8 scope);
    bool operator==(const VarEntry& other) const;
    friend std::ostream& operator<<(std::ostream& os, const VarEntry& entry);
};

Hash hashVarEntry(const VarEntry& entry);
struct VarHasher
{
    Hash operator()(const VarEntry& entry) const
    {
        return hashVarEntry(entry);
    }
};