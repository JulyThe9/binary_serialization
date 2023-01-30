#pragma once

#include <vector>

#include "Types.h"
#include "Root.h"
#include "Core.h"

namespace ObjectModel
{
    class Array : public Root
    {
    private:
        int8_t type = 0;
        int16_t count = 0;
        std::vector<int8_t>* data = nullptr;
    private:
        Array();
    public:
        ~Array();
    public:
        template <typename T>
        // vector instead of arr, because we don't know the size at compile time
        static Array *createArray(const std::string &name, const PrimitiveTypes type, const std::vector<T> &value)
        {
            Array *arr = new Array();
            arr->setName(name);
            arr->wrapperType = static_cast<int8_t>(WrapperTypes::ARRAY);
            arr->type = static_cast<int8_t>(type);    
        
            arr->count = value.size();

            arr->data = new std::vector<int8_t>(value.size() * sizeof (T));
            arr->size += (value.size() * sizeof(T));

            int16_t iter = 0;
            Core::template encode(arr->data, &iter, value);

            return arr;
        }
        // TODO: Why does this not cause 'already defined' without inline?
        static Array *createString(std::string name, const std::string &value)
        {
            Array *str = new Array();
            str->setName(name);
            str->wrapperType = static_cast<int8_t>(WrapperTypes::STRING);
            // NOTE: sizeof(char) is guaranteed by the standard (§5.3.3/1) to be 1 byte
            // (like signed char and unsigned char), so it's alright to hard-code to I8
            str->type = static_cast<int8_t>(PrimitiveTypes::I8);
        
            str->count = value.size();

            str->data = new std::vector<int8_t>(value.size());
            str->size += (value.size());

            int16_t iter = 0;
            Core::template encode(str->data, &iter, value);

            return str;
        }

        void pack(std::vector<int8_t> *, int16_t *) override;
    };
}