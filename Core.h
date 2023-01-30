#pragma once

#include <fstream>
#include <vector>

#include "Root.h"

namespace Core
{
    namespace Util
    {
        inline void save(const char *file, const std::vector<int8_t> &buffer)
        {
            std::ofstream out;

            // std::ios::binary : remember the number 10 issue
            // 10 in ascii is \n, text stream translated \n to \r\n
            // on Windows, so 000a became 0d0a after writing to the file
            out.open(file, std::ios::binary);
            for(int i = 0; i < buffer.size(); ++i)
            {
                out << buffer[i];
            }
            out.close();
        }

        // to extract the name
        // pack stuff to buffer
        // save to file
        inline void retrieveAndSave(ObjectModel::Root *r)
        {
            int16_t iterator = 0;
            std::vector<int8_t> buffer(r->getSize());
            std::string name = r->getName() + ".abc";
            r->pack(&buffer, &iterator);
            save(name.c_str(), buffer);
        }
    }

    // Has likely NOTHING to do with endianness, 
    // but with binary representation of integral types

    // 0000 0000 0000 0001 0010 1011 1100 1001
    
    // 0000 0000 0000 0000 0000 0000 0000 0000
    // 0000 0000 0000 0000 0000 0000 0000 0001
    // 0000 0000 0000 0000 0000 0001 0010 1011
    
    // 1 byte 8 bit
    // 0000 0000
    // 1 byte 2 hexadecimal digit
    // 1 hexadecimal digit 4 binary digits
    // 0000 0000

    template <typename T>
    void encode(std::vector<int8_t> *buffer, int16_t *iterator, T value)
    {
        if (buffer == nullptr || iterator == nullptr)
        {
            return;
        }
        for (size_t i = 0; i < sizeof(T); ++i)
        {
            // (*buffer)[(*iterator)++] = value >> ((sizeof(T) * 8 - 8) - ((i==0) ? j : j += 8) ); 
            (*buffer)[(*iterator)++] = value >> ((sizeof(T) * 8 - 8) - i*8); 
        }
    }

    template <>
    inline void encode<float>(std::vector<int8_t> *buffer, int16_t *iterator, float value)
    {
        int32_t result = *reinterpret_cast<int32_t*>(&value);
        encode<int32_t>(buffer, iterator, result);
    }

    template <>
    inline void encode<double>(std::vector<int8_t> *buffer, int16_t *iterator, double value)
    {
        int64_t result = *reinterpret_cast<int64_t*>(&value);
        encode<int64_t>(buffer, iterator, result);
    }

    template <>
    inline void encode<std::string>(std::vector<int8_t> *buffer, int16_t *iterator, std::string value)
    {
        for (int i = 0; i < value.size(); i++)
        {
            encode<int8_t>(buffer, iterator, value[i]);
        }
    }

    template<typename T>
    void encode(std::vector<int8_t> *buffer, int16_t *iterator, std::vector<T> &value)
    {
        for (int i = 0; i < value.size(); i++)
        {
            encode<T>(buffer, iterator, value[i]);
        }        
    }
}
