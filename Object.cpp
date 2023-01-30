#include "Object.h"

namespace ObjectModel
{
    Object::Object(const std::string &name)
    {
        setName(name);
        wrapperType = static_cast<int8_t>(WrapperTypes::OBJECT);
        size += sizeof(count);
    }

    Object::~Object()
    {
        std::cout << "Object dtor called\n";
        for (size_t i = 0; i < entities.size(); i++)
        {
            delete entities[i];
        }
        entities.clear();
    }

    void Object::addEntity(Root *const r)
    {
        entities.push_back(r);
        count++;
        size += r->getSize();
    }

    Root* Object::findByName(const std::string &name)
    {
        for (auto ent : entities)
        {
            if (ent->getName() == name)
            {
                return ent;
            }
        }
        return nullptr;
    }

    void Object::pack(std::vector<int8_t>* buffer, int16_t* iterator)
    {
        Core::encode<std::string>(buffer, iterator, name);
        Core::encode<int16_t>(buffer, iterator, nameLength);
        Core::encode<int8_t>(buffer, iterator, wrapperType);
        Core::encode<int16_t>(buffer, iterator, count);

        for (Root *ent : entities)
        {
            ent->pack(buffer, iterator);
        }

        Core::encode<int32_t>(buffer, iterator, size);
    }
}