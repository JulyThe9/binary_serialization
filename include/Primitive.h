#pragma once

#include <vector>

#include "Types.h"
#include "Root.h"
#include "Core.h"

namespace ObjectModel
{
    class Primitive : public Root
    {
    private:
        int8_t type = 0;
        std::vector<int8_t>* data = nullptr;
    private:
        // this (private constructor) means that noone can create this object
        // except for public static methods of this class
        Primitive();
    public:
        ~Primitive();
    public:
        // TODO: Is it? According to "Named Constructor Idiom", this is a so called
        // named constructor, meaning that it resolves the problem of regular constructors
        // of the same name as the class, which become error-prone when there are many of those
        template <typename T>
        static Primitive *create(const std::string &name, const PrimitiveTypes type, const T value)
        {
            Primitive *p = new Primitive();
            p->setName(name);
            p->wrapperType = static_cast<int8_t>(WrapperTypes::PRIMITIVE);
            p->type = static_cast<int8_t>(type);
            p->data = new std::vector<int8_t>(sizeof value);
            
            p->size += (sizeof value); 

            int16_t iter = 0;
            
            //TODO: Core::encode<T>(p->data, &iter, value); // TODO: why not this one?
            Core::template encode(p->data, &iter, value);

            return p;
        }

        void pack(std::vector<int8_t>*, int16_t*) override;
    };
}