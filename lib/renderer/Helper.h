#pragma once

namespace data {
    // Converts the fist 4 chars from the given arr to an float.
    float charsToFloat(const char* inp);

    // Converts the fist 4 chars from the given arr to an float.
    // With in offset.
    float charsToFloat(const char* inp, int offset);

    // Converts the fist 4 chars from the given arr to an int.
    int charsToInt(const char* inp);
}
