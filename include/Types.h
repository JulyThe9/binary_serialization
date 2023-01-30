#pragma once

#include <cstdint>

namespace ObjectModel
{
    enum class WrapperTypes : int8_t
    {
        PRIMITIVE = 1,
        ARRAY,
        STRING,
        OBJECT
    };

    enum class PrimitiveTypes: int8_t
    {
        I8 = 1,
        I16,
        I32,
        I64,

        U8,
        U16,
        U32,
        U64,

        FLOAT,
        DOUBLE,

        BOOL
    };
}