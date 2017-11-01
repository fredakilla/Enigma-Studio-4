#include "system.hpp"

eBitArray::Bit::Bit (int i, char *data)
    : i(i), data(data) {}

eBitArray::Bit::operator bool () const
{
    return ((*data) & (1 << i));
}

eBitArray::Bit eBitArray::Bit::operator = (bool val)
{
    if (val) {
        *data |= 1 << i;
    }
    else {
        *data &= ~(char)(1 << i);
    }
    return *this;
}

eBitArray::eBitArray (int num_bits) : len_ (num_bits) {
    this->array = new char [num_bits]();
}

eBitArray::~eBitArray ()
{
    delete [] this->array;
}

eBitArray::Bit eBitArray::operator [] (int i)
{
    eASSERT(i/8 < len_);

    int byte_idx = i / 8;
    int bit_idx = i % 8;
    Bit bit (bit_idx, &array[byte_idx]);
    return bit;
}

int eBitArray::len () const
{
    return len_;
}
