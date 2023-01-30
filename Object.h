#pragma once

#include <iostream>

#include "Types.h"
#include "Root.h"
#include "Core.h"

namespace ObjectModel
{
    class Object : public Root
    {
    private:
        // TODO: EITHER write a destructor and delete entities OR
        // replace all raw pointers with smart pointers
        std::vector<Root*> entities;
        int16_t count = 0;
    public:
        Object(const std::string &);
        ~Object();
    public:
        void addEntity(Root *const r);
        Root *findByName(const std::string &);
        void pack(std::vector<int8_t>*, int16_t*) override;
    };
}