#pragma once

#include <vector>
#include <iostream>
#include <assert.h>

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
        const int32_t getSize() const;
        void setName(const std::string &);
        const std::string &getName() const;

        virtual void pack(std::vector<int8_t> *, int16_t *) = 0;
    };
}