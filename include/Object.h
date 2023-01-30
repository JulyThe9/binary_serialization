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
        std::vector<Root*> entities;
        int16_t count = 0;
    public:
        Object(const std::string &);
        ~Object();
    public:
        void addEntity(Root *r);
        // TODO: ideally should be a hashmap
        Root *findByName(const std::string &);
        void pack(std::vector<int8_t> *, int16_t *) override;
    };
}