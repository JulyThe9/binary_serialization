#pragma once

#include "Types.h"
#include "Root.h"
#include "Core.h"

namespace ObjectModel
{
    class Object : public Root
    {
    private:
        std::vector<Root*> entities;
        int16_t count = 0;
    public:
        Object(const std::string &);
    public:
        void addEntity(Root *const r);
        Root *findByName(const std::string &);
        void pack(std::vector<int8_t>*, int16_t*); 
    };
}