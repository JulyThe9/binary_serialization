#include "Primitive.h"

namespace ObjectModel
{
    Primitive::Primitive()
    {
        size += sizeof(type);
    }

    Primitive::~Primitive()
    {
        std::cout << "Primitive dtor called\n";    
        delete data;
    }

    void Primitive::pack(std::vector<int8_t> *buffer, int16_t *iterator)
    {
        assert(buffer && iterator);
        Core::encode<std::string>(buffer, iterator, name);
        Core::encode<int16_t>(buffer, iterator, nameLength);
        Core::encode<int8_t>(buffer, iterator, wrapperType);
        Core::encode<int8_t>(buffer, iterator, type);
        Core::encode<int8_t>(buffer, iterator, *data);
        Core::encode<int32_t>(buffer, iterator, size);
    }
}