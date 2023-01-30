#include "Root.h"

namespace ObjectModel
{
    Root::Root() :
        name("unknown"),
        wrapperType(0),
        nameLength(0),
        size(sizeof nameLength + sizeof wrapperType + sizeof size) {}

    Root::~Root()
    {
        std::cout << "Root dtor called\n";
    }

    void Root::setName(std::string name)
    {
        this->name = name;

        // string.size() and string.length() are identical
        // size is here to be consistent with other STL containers
        // and length is here because it's natural to think about strings'
        // size in term of length

        // also not a pointer, so C-like cast is acceptable
        nameLength = (int16_t)name.length();
        size += nameLength;
    }

    int32_t Root::getSize()
    {
        return size;
    }

    std::string Root::getName()
    {
        return name;
    }
}
