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

    void Root::setName(const std::string &name)
    {
        this->name = name;
        nameLength = (int16_t)name.length();
        size += nameLength;
    }

    const int32_t Root::getSize() const
    {
        return size;
    }

    const std::string &Root::getName() const
    {
        return name;
    }
}
