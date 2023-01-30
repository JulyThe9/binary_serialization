#pragma once

#include <string>
#include <vector>
#include <iostream>

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
        virtual ~Root() = 0;
    public:
        int32_t getSize();
        void setName(std::string);
        std::string getName();

        virtual void pack(std::vector<int8_t>*, int16_t*) = 0;
    };
}