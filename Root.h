#pragma once

#include <string>
#include <vector>

namespace ObjectModel
{
    class Root
    {
    protected:
        std::string name;
        int16_t nameLength;
        int8_t wrapperType;
        int32_t size;
    protected:
        Root();
    public:
        int32_t getSize();
        void setName(std::string);
        std::string getName();

        // when you add a virtual keyword to any method in a class,
        // the size of the class become exactly 4 bytes larger,
        // because it gets appended a pointer to the v-table
        virtual void pack(std::vector<int8_t>*, int16_t*);
    };
}